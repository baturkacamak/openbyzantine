"""Seeded mixed exploration; scheduling metadata never prunes detailed states."""
from collections import Counter
import math


def action_key(action):
    return (action.id, action.category, tuple(action.commands))


class MixedPolicy:
    """Root coverage, then 50% rotating deep / 25% balanced / 25% random intent.

    Quality discounts repeated inputs in the same progress context and recent
    view/selection cycles. Random intent/target choices and positive weights preserve access to
    every queued route; finite budgets still limit what can actually run.
    """
    def __init__(self, rng):
        self.rng=rng;self.tick=0;self.lanes=Counter();self.attempts=Counter()
        self.categories=Counter();self.progress_categories=Counter();self.seed_runs=Counter()
        self.max_depth=0;self.depths=Counter();self.last_lane='root'
        self.deep_seed_runs=Counter();self.goals=Counter();self.transitions=Counter();self.targets=Counter()

    def quality(self, plan):
        if not plan.actions:return 1.0
        action=plan.actions[-1]
        count=self.attempts[(plan.seed,plan.origin_progress,action_key(action))]
        recent=sum(action_key(a)==action_key(action) for a in plan.actions[-7:-1])
        base={'hotspot':4.0,'examine':3.0,'inventory':1.5,'camera':1.0,'wait':0.5}[action.category]
        if plan.goal=='travel':base*=2
        return base*(2 if plan.progress_priority else 1)/((1+count)*(1+recent))

    def depth_score(self, plan):
        # A long series of selecting/scanning views must not monopolize the
        # deep lane. Keep every route available to balanced/uniform draws.
        tail=0
        for action in reversed(plan.actions):
            if action.category not in ('inventory','camera'):break
            tail+=1
        return len(plan.actions)-2*max(0,tail-3)

    def weighted(self, entries):
        # Normalize by intent before picking a target. Thirty inventory jobs
        # must not buy thirty tickets against one scene exit or talk hotspot.
        groups={}
        for entry in entries:groups.setdefault(self.goal(entry[1]),[]).append(entry)
        def race(weight):return -math.log(max(self.rng.random(),1e-15))/weight
        goal=min(sorted(groups),key=lambda g:race(sum(self.quality(p) for _,p in groups[g])/len(groups[g])))
        return min(groups[goal],key=lambda entry:race(self.quality(entry[1])))[0]

    def pop(self, queue):
        roots=[i for i,p in enumerate(queue) if not p.actions and not p.choices]
        if roots:index=roots[0];lane='root'
        else:
            lane=('deep','balanced','deep','uniform')[self.tick%4];self.tick+=1
            entries=list(enumerate(queue))
            if lane=='deep':
                # Choose a starting context before depth. Global maximum depth
                # previously spent half the entire campaign on one mature tree.
                seeds=sorted({p.seed for _,p in entries})
                minimum=min(self.deep_seed_runs[seed] for seed in seeds)
                seed=self.rng.choice([seed for seed in seeds if self.deep_seed_runs[seed]==minimum])
                entries=[entry for entry in entries if entry[1].seed==seed]
                self.deep_seed_runs[seed]+=1
                depth=max(self.depth_score(p) for _,p in entries)
                entries=[entry for entry in entries if self.depth_score(entry[1])==depth]
                index=self.weighted(entries)
            else:
                seeds=sorted({p.seed for _,p in entries})
                if lane=='balanced':
                    minimum=min(self.seed_runs[seed] for seed in seeds)
                    seeds=[seed for seed in seeds if self.seed_runs[seed]==minimum]
                seed=self.rng.choice(seeds)
                entries=[entry for entry in entries if entry[1].seed==seed]
                if lane=='uniform':
                    # Select an intent first so a large inventory menu does
                    # not drown the few travel/object/camera choices.
                    goals=sorted({self.goal(p) for _,p in entries})
                    goal=self.rng.choice(goals)
                    index=self.rng.choice([entry for entry in entries if self.goal(entry[1])==goal])[0]
                else:index=self.weighted(entries)
        queue.rotate(-index);plan=queue.popleft();queue.rotate(index)
        self.last_lane=lane;self.lanes[lane]+=1;self.seed_runs[plan.seed]+=1
        self.goals[self.goal(plan)]+=1
        self.max_depth=max(self.max_depth,len(plan.actions));self.depths[len(plan.actions)]+=1
        return plan

    @staticmethod
    def goal(plan):
        return plan.goal or (plan.actions[-1].category if plan.actions else 'root')

    def observe(self, plan, origin_progress, changed_progress, before_scene='', after_scene=''):
        if not plan.actions:return
        action=plan.actions[-1]
        self.attempts[(plan.seed,origin_progress,action_key(action))]+=1
        self.categories[action.category]+=1
        if changed_progress:self.progress_categories[action.category]+=1
        if before_scene and after_scene and before_scene!=after_scene:
            self.transitions[(before_scene,after_scene)]+=1
        if before_scene and action.category=='hotspot':self.targets[(before_scene,action.id)]+=1

    def report(self):
        return {'version':3,'lanes':dict(self.lanes),'max_depth':self.max_depth,
                'deep_seed_runs':dict(self.deep_seed_runs),'goal_selections':dict(self.goals),
                'distinct_hotspot_targets_attempted':len(self.targets),
                'observed_scene_transitions':[{'from':a,'to':b,'count':count} for (a,b),count in sorted(self.transitions.items())],
                'depths':dict(self.depths),'last_action_categories':dict(self.categories),
                'progress_changes_by_category':dict(self.progress_categories),
                'policy':'roots first; 50% seed-rotating deep, 25% balanced, 25% random intent/target; goal-normalized weights; saturated-view/repeat discounts; positive weights'}
