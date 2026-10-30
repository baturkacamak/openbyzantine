# Coverage-balanced random exploration (30 October2026)

User challenged the concentration of the still-running campaign on one NB
branch and requested genuinely varied random travel/object interactions.
Pinned active run: `campaign-20261030T165906-3524352`. Its logic process remains
running with its originally loaded policy; it was not interrupted or changed.

## Diagnosis and correction

Observed last200 discovery jobs:100 from save34/MAP. The old mixed policy's
50% deep lane chose a global deepest frontier, so a mature branch monopolized
that share. The other lanes did diversify, but that did not justify calling the
overall run continually different-place exploration.

Policyv3 now selects the least-used eligible **deep starting context first**,
then favors depth within that context. Balanced and random lanes remain.
All weighted lanes normalize by intent/category before choosing a target;
the random lane chooses seed -> intent -> target. A large inventory menu no
longer multiplies its chance over a small set of exits/hotspots. Contextual
repeat and view-tail discounts remain positive; no game state is fabricated,
no queued route is pruned by ranking and no scene-name branch is introduced.

The existing observed kind0x17 map-destination rule supplies a travel intent.
This is a scheduling hint, not proof of travel. Successful snapshots separately
record actual from/to scene changes. Reports expose deep-seed selections,
intent counts, distinct hotspot targets attempted and observed transitions.
Dialogue branches retain observed scene metadata; all metadata stays outside
job identity and exact replay inputs. Legacy BFS/random semantics are retained.

The tradeoff is deliberate: distributing deep work across49 contexts grows
long chains more slowly than repeatedly extending one tree. Depth24 remains
a ceiling, not a promised depth per budget. Required puzzle conditions still
matter; no random policy guarantees solving all puzzles/reaching every scene.

## Verification

Final actual run: `port/output/coverage-policy/final/`, same random seed
2891583293 as the active user campaign, all49 genuine saved starts, mixed,
max-depth24, max-frontier20000,160-launch cap. Results:

-160 launches;134 detailed states,109 conservative progress/context signatures.
-26 scene identifiers, including examine screens; no findings.
-All49 starts receive deep work, each1–2 deep selections at this snapshot.
-Last100 discovery jobs: most-used start only4 selections, compared with50/100
 in the active campaign's earlier observed window. This is an observed
 distribution check, not a claim of universal statistical performance.
-27 travel-intent selections; actual transitions include MAP->AM/AS/EA/FT/HL/
 HSLOWER/IU/MR/NB/SM/SP/TK, IU->VC, several map returns and examine entries.
-The small-budget maximum depth is3, honestly showing the breadth/depth tradeoff.

The earlier `actual/` prototype had the default512-job frontier and is not the
final coverage measurement; its deferred work remains separately recorded.
`summary.json` consolidates final metrics and last100 seed counts.

Unit coverage includes a branch-rich24-step tree competing with48 other starts:
all49 receive deep opportunities, with counts differing by at most1 while all
remain eligible. Separate tests show neither random nor weighted category
selection is overwhelmed by100 inventory jobs; travel intent remains scheduling
metadata, not replay/job identity. Existing depth24 synthetic test still passes
when only one context is intentionally explored.

`make -C port -j2 all test` passed; Python discovery54 checks and selected
CMake exploration/policy/matrix3/3 pass. Evidence:
`port/output/coverage-policy/final-build-test.log`, `python-discovery.log`,
`unit-final.log`, `ctest.log`, `final/report.json`, `summary.json`.
No C++ player behavior changed. User run/LATEST and original assets preserved;
no new full2000-run campaign or commit/push was initiated.
