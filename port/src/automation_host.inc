// Included inside the player host namespace after Session and UI definitions.
// Input goes through SDL and its normal handlers; snapshots are read-only.
template<class Values>
void automation_numbers(std::ostream& out, const Values& values) {
    out << '[';
    bool comma = false;
    for (const auto value : values) { if (comma) out << ','; comma = true; out << +value; }
    out << ']';
}
void automation_entry_key(std::ostream& out, const byzantine::ConversationEntryKey& key) {
    out << '[' << +key.list << ',' << key.group << ',' << key.hop << ',' << key.sub << ']';
}
// Read-only discovery uses the same inverse panorama map and first-hit rule
// as real input. Never execute a label to discover where it can be clicked.
void automation_exploration(std::ostream& out, const Session& session) {
    const auto& view=session.view;
    const auto& state=view.state;
    const auto str=[&](const std::string& value){byzantine::automation_json_string(out,value);};
    out<<",\"exploration\":{\"schema\":1,\"candidates\":[";
    bool comma=false;
    const auto candidate=[&](const std::string& id,const std::string& category,
                             const std::vector<std::string>& commands) {
        if(comma)out<<',';
        comma=true;
        out<<"{\"id\":";str(id);out<<",\"category\":";str(category);
        out<<",\"commands\":[";bool sep=false;
        for(const auto& command:commands){if(sep)out<<',';sep=true;str(command);}
        out<<"]}";
    };
    if(!session.reload_available) {
        struct Region { int x0=640,y0=480,x1=-1,y1=-1;std::vector<std::pair<int,int>> points; };
        std::map<int,Region> regions;
        for(int y=20;y<420;++y)for(int x=0;x<640;++x) {
            const auto hit=hotspot_at(view,x,y-kScreenPlayfieldTop);
            if(!hit.slot)continue;
            auto& region=regions[*hit.slot];
            region.x0=std::min(region.x0,x);region.x1=std::max(region.x1,x);
            region.y0=std::min(region.y0,y);region.y1=std::max(region.y1,y);
            // Store a sparse interior sample, retaining thin/one-pixel targets.
            if(region.points.empty() || (x%4==0 && y%4==0))region.points.emplace_back(x,y);
        }
        for(const auto& pair:regions) {
            const auto object=state.objects.find(pair.first);
            if(state.current_dialogue_record && object!=state.objects.end() && !byzantine::item_use_compatible(
                    state,object->second,state.current_dialogue_record))continue;
            const auto& r=pair.second;
            const int cx=(r.x0+r.x1)/2,cy=(r.y0+r.y1)/2;
            const auto point=*std::min_element(r.points.begin(),r.points.end(),[&](auto a,auto b){
                const auto distance=[&](auto p){return (p.first-cx)*(p.first-cx)+(p.second-cy)*(p.second-cy);};
                return distance(a)<distance(b);
            });
            candidate("hotspot_"+std::to_string(pair.first),"hotspot",
                      {"click "+std::to_string(point.first)+" "+std::to_string(point.second)});
        }
        if(state.dialogue_visible) {
            const auto page=byzantine::inventory_page(state);
            if(state.current_dialogue_record)candidate("examine","examine",{"click 24 450"});
            for(std::size_t i=0;i<page.items.size();++i)
                candidate("item_"+std::to_string(page.items[i]),"inventory",
                          {"click "+std::to_string(190+60*i)+" 450"});
            if(page.scroll>0)candidate("inventory_previous","inventory",{"click 618 450"});
            const auto total=state.dialogue_channels[page.page%state.dialogue_channels.size()].size();
            if(page.scroll+page.items.size()<total)candidate("inventory_next","inventory",{"click 586 450"});
        }
        if(view.panorama && panorama_layer(view)) {
            candidate("turn_left","camera",std::vector<std::string>(6,"key Left"));
            candidate("turn_right","camera",std::vector<std::string>(6,"key Right"));
            candidate("look_up","camera",std::vector<std::string>(3,"key Up"));
            candidate("look_down","camera",std::vector<std::string>(3,"key Down"));
        }
        candidate("wait","wait",{"wait 1000"});
    }
    out<<"],\"record_identity\":{";comma=false;
    for(const auto& pair:state.records) {
        if(comma)out<<',';
        comma=true;str(std::to_string(pair.first));
        const auto& record=pair.second;
        out<<":["<<record.x<<','<<record.y<<','<<+record.kind<<','<<+record.flags
           <<','<<record.frame_count<<','<<record.field04<<','<<record.field_2c<<','<<record.field_30<<']';
    }
    // Only script-bearing animation phases enter discovery state. Idle frame
    // counters must not turn a looping background actor into endless novelty.
    out<<"},\"record_callbacks\":{";comma=false;
    for (const auto& [id,record] : state.records) {
        if(record.field_2c==0xffffffffu && record.extra.empty())continue;
        if(comma)out<<',';
        comma=true;str(std::to_string(id));
        out<<":["<<(record.current_frame ? static_cast<unsigned>(*record.current_frame) : 0xffffu)
           <<','<<record.field06<<','<<record.field_2c<<",[";
        bool separator=false;
        for(const auto& extra:record.extra){
            if(separator)out<<',';
            separator=true;out<<'['<<extra.first<<','<<extra.second<<']';
        }
        out<<"]]";
    }
    out<<"},\"return_context\":";
    if(session.return_armed) {
        const auto& parent=session.return_view;
        out<<"{\"scene\":";str(lower_ascii(parent.name));
        out<<",\"background\":";str(lower_ascii(parent.committed_background));
        out<<",\"layer\":"<<static_cast<int>(parent.layer)<<",\"panorama\":";
        if(panorama_layer(parent) && parent.panorama)out<<'['<<parent.panorama->yaw_degrees()<<','<<parent.panorama->pitch_degrees()<<']';
        else out<<"null";
        out<<",\"pool\":";automation_numbers(out,parent.state.variable_pool);
        out<<",\"variables\":{";bool separator=false;
        for(const auto& v:parent.state.variables){if(separator)out<<',';separator=true;str(std::to_string(v.first));out<<':'<<v.second;}
        out<<"},\"objects\":{";separator=false;
        for(const auto& pair:parent.state.objects) {
            if(separator)out<<',';
            separator=true;str(std::to_string(pair.first));const auto& object=pair.second;
            out<<":["<<object.x<<','<<object.y<<','<<object.width<<','<<object.height<<','
               <<+object.active<<','<<+object.kind<<','<<object.label<<','<<object.name_id<<','<<object.argument<<']';
        }
        out<<"}}";
    } else out<<"null";
    out<<",\"decisions\":[";comma=false;
    for(const auto& decision:session.automation_decisions) {
        if(comma)out<<',';
        comma=true;
        out<<"{\"scene\":";str(decision.scene);
        out<<",\"record\":"<<decision.record<<",\"group\":"<<decision.group
           <<",\"action\":"<<decision.action<<",\"choice\":"<<decision.choice<<",\"lines\":";
        automation_numbers(out,decision.lines);out<<'}';
    }
    out<<"],\"blocker\":";str(byzantine::trace_blocker(view.trace));
    out<<",\"script_end\":"<<(view.trace.script_end_requested?"true":"false")
       <<",\"scene_switch_pending\":"<<(view.trace.scene_switch_requested?"true":"false")
       <<",\"hover_preview\":"<<(session.hover_highlight_active?"true":"false")<<'}';
}

void automation_settings(std::ostream& out,const Session& session) {
    const auto& s=session.options.settings;
    const auto str=[&](const auto& value){byzantine::automation_json_string(out,value);};
    out<<",\"host_settings\":{\"window_mode\":";str(byzantine::window_mode_name(s.window_mode));
    out<<",\"scale_mode\":";str(byzantine::scale_mode_name(s.scale_mode));
    out<<",\"vsync\":"<<(s.vsync?"true":"false")<<",\"cursor_scale\":"<<s.cursor_scale;
    const auto flag=[&](const char* name,bool value){out<<",\""<<name<<"\":"<<(value?"true":"false");};
    flag("mute_on_focus_loss",s.mute_on_focus_loss);flag("subtitles",s.subtitles);
    flag("hotspots",s.hotspots);flag("hotspots_all",s.hotspots_all);flag("autosave",s.autosave);
    flag("skip_seen",s.skip_seen);flag("gamepad",s.gamepad);
    out<<",\"master_volume\":"<<s.master_volume<<",\"speech_volume\":"<<s.speech_volume
       <<",\"ambient_volume\":"<<s.ambient_volume<<",\"effects_volume\":"<<s.effects_volume;
    const auto field=[&](const char* name,const std::string& value){out<<",\""<<name<<"\":";str(value);};
    field("text_language",s.text_language);field("voice_language",s.voice_language);
    field("key_settings",s.key_settings);field("key_backlog",s.key_backlog);field("key_skip",s.key_skip);
    field("key_quicksave",s.key_quicksave);field("key_quickload",s.key_quickload);
    field("key_autoload",s.key_autoload);field("key_screenshot",s.key_screenshot);field("key_hotspots",s.key_hotspots);
    out<<"},\"gameprefs\":";
    if(session.view.state.gameprefs)automation_numbers(out,*session.view.state.gameprefs);else out<<"null";
    out<<",\"host_runtime\":{\"window_mode\":";str(byzantine::window_mode_name(session.display.window_mode));
    out<<",\"scale_mode\":";str(byzantine::scale_mode_name(session.display.requested_scale));
    int w=0,h=0,lw=0,lh=0;SDL_GetWindowSize(session.display.window,&w,&h);
    SDL_RenderGetLogicalSize(session.display.renderer,&lw,&lh);
    out<<",\"window\":["<<w<<','<<h<<"],\"logical\":["<<lw<<','<<lh<<']';
    int ow=0,oh=0,x0=0,y0=0,x1=0,y1=0;
    SDL_GetRendererOutputSize(session.display.renderer,&ow,&oh);
    SDL_RenderLogicalToWindow(session.display.renderer,0,0,&x0,&y0);
    SDL_RenderLogicalToWindow(session.display.renderer,640,480,&x1,&y1);
    out<<",\"output\":["<<ow<<','<<oh<<"],\"content_rect\":["<<x0<<','<<y0<<','<<x1-x0<<','<<y1-y0<<']';
    SDL_RendererInfo info{};SDL_GetRendererInfo(session.display.renderer,&info);
    out<<",\"vsync_supported\":"<<(session.display.vsync_supported?"true":"false");
    out<<",\"last_content\":";str(screen_content_name(session.display.last_presented_content));
    out<<",\"renderer_flags\":"<<info.flags<<",\"integer_scale\":"<<(SDL_RenderGetIntegerScale(session.display.renderer)?"true":"false");
    out<<",\"overlay_enabled\":"<<(session.options.overlay?"true":"false")
       <<",\"overlay_all\":"<<(session.options.hotspots_all?"true":"false")
       <<",\"controller_open\":"<<(session.display.gamepad?"true":"false")
       <<",\"audio_ready\":"<<(session.audio.ready()?"true":"false")
       <<",\"audio_muted\":"<<(session.audio.muted()?"true":"false")<<",\"audio_gains\":";
    automation_numbers(out,session.audio.gains());
    out<<",\"text_resolved\":";str(session.options.language.text_code_or_base());
    out<<",\"voice_resolved\":";str(session.options.language.voice_code_or_base());
    out<<",\"languages\":[\"auto\"";
    for(const auto& pack:session.options.language.packs){out<<',';str(pack.code);}
    out<<"]}";
}

void automation_snapshot(Session& session, const std::filesystem::path& directory,
                         const std::string& name, bool menu, bool settings) {
    const auto& view = session.view;
    const auto& state = view.state;
    std::ofstream out(directory / (name + ".json"));
    if (!out) throw std::runtime_error("cannot write automation snapshot");
    const auto str = [&](const std::string& value) { byzantine::automation_json_string(out,value); };
    out << "{\"schema\":1,\"scene\":"; str(lower_ascii(view.name));
    out << ",\"background\":"; str(lower_ascii(view.background_name));
    out << ",\"layer\":" << static_cast<int>(view.layer)
        << ",\"session_ended\":" << (session.reload_available ? "true" : "false")
        << ",\"menu\":" << (menu ? "true" : "false") << ",\"settings\":" << (settings ? "true" : "false");
    out << ",\"active_slots\":[";
    bool comma = false;
    for (const auto& h : view.trace.active_hotspots) { if(comma){out<<',';}comma=true;out<<h.slot; }
    out << "],\"variables\":{";comma=false;
    for(const auto& v:state.variables){if(comma){out<<',';}comma=true;str(std::to_string(v.first));out<<':'<<v.second;}
    out << "},\"variable_pool\":";automation_numbers(out,state.variable_pool);
    out<<",\"effective_pool\":";
    if(session.metadata)automation_numbers(out,byzantine::export_variable_pool(
        state,byzantine::scene_variable_layout(*session.metadata,view.name)));
    else automation_numbers(out,state.variable_pool);
    out << ",\"inventory\":{";comma=false;
    for(const auto& pair:state.dialogue_records){if(comma){out<<',';}comma=true;str(std::to_string(pair.first));const auto& item=pair.second;out<<":["<<+item.state<<','<<(item.inventory_loaded?1:0)<<']';}
    out << "},\"channels\":[";comma=false;
    for(const auto& channel:state.dialogue_channels){if(comma){out<<',';}comma=true;automation_numbers(out,channel);}
    out << "],\"channel\":" << state.current_dialogue_channel << ",\"selected\":";
    if(state.current_dialogue_record)out<<*state.current_dialogue_record;else out<<"null";
    out << ",\"objects\":{";comma=false;
    for(const auto& pair:state.objects){
        if(comma){out<<',';}comma=true;str(std::to_string(pair.first));const auto& o=pair.second;
        out<<":["<<o.x<<','<<o.y<<','<<o.width<<','<<o.height<<','<<+o.active<<','<<+o.kind<<','<<o.label<<','<<o.name_id<<','<<o.argument<<']';
    }
    out << "},\"enabled\":{";comma=false;
    for(const auto& pair:state.hotspot_enabled){if(comma){out<<',';}comma=true;str(std::to_string(pair.first));out<<':'<<(pair.second?"true":"false");}
    out << "},\"sounds\":{";comma=false;
    for(const auto& pair:state.sounds){
        if(comma){out<<',';}comma=true;str(std::to_string(pair.first));const auto& a=pair.second;
        out<<":["<<a.defined<<','<<a.enabled<<','<<a.active<<','<<a.field1c<<','<<a.field20<<','<<a.field24<<','<<a.field28<<','<<a.field2c<<']';
    }
    out << "},\"conversation\":{";comma=false;
    for(const auto& pair:state.conversation_memory){
        if(comma){out<<',';}comma=true;str(std::to_string(pair.first));const auto& memory=pair.second;
        out<<":{\"done\":"<<(memory.done?"true":"false")<<",\"default_line\":"<<+memory.default_line
           <<",\"default_reply\":"<<+memory.default_reply<<",\"unspeak\":[";
        bool sep=false;for(const auto& k:memory.unspeak){if(sep){out<<',';}sep=true;automation_entry_key(out,k);}
        out<<"],\"disabled\":[";sep=false;
        for(const auto& k:memory.disabled){if(sep){out<<',';}sep=true;automation_entry_key(out,k);}
        out<<"],\"skip_a\":";automation_numbers(out,memory.skip_a);
        out<<",\"skip_b\":";automation_numbers(out,memory.skip_b);
        out<<",\"skip_c\":";automation_numbers(out,memory.skip_c);
        out<<",\"override_flags\":";automation_numbers(out,memory.line_override_flag);
        out<<",\"overrides\":[";sep=false;
        for(const auto& k:memory.line_override){if(sep){out<<',';}sep=true;if(k)automation_entry_key(out,*k);else out<<"null";}
        out<<"],\"default_reply_pending\":"<<(memory.default_reply_pending?"true":"false");
        const auto slots = [&](const char* label, const auto& values) {
            out<<",\""<<label<<"\":[";bool separator=false;
            for(const auto& slot:values){
                if(separator)out<<',';
                separator=true;
                if(slot){out<<'['<<+slot->byte<<',';automation_entry_key(out,slot->entry);out<<']';}
                else out<<"null";
            }
            out<<']';
        };
        slots("slot_2a",memory.slot_2a);slots("slot_5c",memory.slot_5c);slots("slot_8e",memory.slot_8e);
        out<<'}';
    }
    out << "},\"panorama\":";
    if(panorama_layer(view) && view.panorama)out<<'['<<view.panorama->yaw_degrees()<<','<<view.panorama->pitch_degrees()<<']';else out<<"null";
    out << ",\"media_disc\":"<<session.options.active_media_disc;
    out<<",\"random_state\":";
    if(state.random_state)out<<*state.random_state;else out<<"null";
    out<<",\"morris\":{\"initialized\":"<<(state.morris.initialized?"true":"false")
       <<",\"turns\":"<<state.morris.turns<<",\"nodes\":[";
    for(std::size_t i=0;i<state.morris.nodes.size();++i) {
        if(i)out<<',';
        out<<'['<<state.morris.nodes[i].score<<','<<+state.morris.nodes[i].flags<<']';
    }
    out<<"]}";
    out<<",\"inventory_ui\":{\"visible\":"<<(state.dialogue_visible?"true":"false")
       <<",\"scroll\":"<<state.dialogue_scroll<<",\"return_armed\":"<<(session.return_armed?"true":"false")
       <<",\"parent_scene\":";str(lower_ascii(session.return_view.name));
    out<<",\"using\":";
    if(state.current_use_item)out<<*state.current_use_item;else out<<"null";
    out<<'}';
    if(session.options.automation_explore)automation_exploration(out,session);
    if(session.options.automation_inspect_settings)automation_settings(out,session);
    out << ",\"media_profile\":{\"audio\":"<<session.options.audio<<",\"video\":"<<session.options.video<<"}}\n";
    out.close();
    if(!out)throw std::runtime_error("automation state write failed");
    const auto& image=session.display.last_presented_image;
    if(image.pixels.empty() || !byzantine::write_ppm(image,(directory/(name+".ppm")).string()))
        throw std::runtime_error("automation image write failed");
    std::cout << "automation_snapshot name="<<name<<" scene="<<view.name<<'\n';
}
struct AutomationRuntime {
    std::vector<byzantine::AutomationAction> actions;
    std::filesystem::path output;
    std::size_t next = 0;
    std::uint64_t due = 0;
    explicit AutomationRuntime(const Options& options) {
        if(options.automation_script.empty())return;
        std::ifstream input(options.automation_script);
        if(!input)throw std::runtime_error("cannot open automation script");
        actions=byzantine::parse_automation(input);
        output=options.automation_output;
        std::filesystem::create_directories(output);
    }
    void tick(Session& session, bool menu, bool settings) {
        if(actions.empty() || SDL_GetTicks64()<due || next>=actions.size())return;
        const auto& a=actions[next++];
        session.automation_action_index=next;
        std::cout<<"automation_action index="<<next<<" kind="<<a.kind<<" name="<<a.name<<'\n';
        if(a.kind=="wait"){due=SDL_GetTicks64()+a.x;return;}
        if(a.kind=="snapshot"){automation_snapshot(session,output,a.name,menu,settings);return;}
        SDL_Event e{};
        const auto window=SDL_GetWindowID(session.display.window);
        const auto push=[&](SDL_Event& event){
            event.common.timestamp=SDL_GetTicks();
            if(SDL_PushEvent(&event)!=1)throw std::runtime_error("automation SDL input was refused");
        };
        if(a.kind=="resize"){
            SDL_SetWindowSize(session.display.window,a.x,a.y);
            return;
        }
        if(a.kind=="focus"){
            e.type=SDL_WINDOWEVENT;e.window.windowID=window;
            e.window.event=a.name=="lost"?SDL_WINDOWEVENT_FOCUS_LOST:SDL_WINDOWEVENT_FOCUS_GAINED;
            push(e);return;
        }
        if(a.kind=="quit"){e.type=SDL_QUIT;push(e);return;}
        if(a.kind=="key"){
            const auto key=SDL_GetKeyFromName(a.name.c_str());
            if(key==SDLK_UNKNOWN)throw std::runtime_error("unknown automation key: "+a.name);
            e.type=SDL_KEYDOWN;e.key.windowID=window;e.key.keysym.sym=key;push(e);
            e.type=SDL_KEYUP;push(e);return;
        }
        int x=0,y=0;
        SDL_RenderLogicalToWindow(session.display.renderer,a.x+0.75f,a.y+0.75f,&x,&y);
        std::cout << "automation_pointer requested="<<a.x<<","<<a.y<<" window="<<x<<","<<y<<"\n";
        e.type=SDL_MOUSEMOTION;e.motion.windowID=window;e.motion.x=x;e.motion.y=y;push(e);
        if(a.kind=="click"){
            e={};e.type=SDL_MOUSEBUTTONDOWN;e.button.windowID=window;e.button.button=SDL_BUTTON_LEFT;
            e.button.x=x;e.button.y=y;e.button.clicks=1;push(e);
            e.type=SDL_MOUSEBUTTONUP;e.button.x=x;e.button.y=y;push(e);
        }
    }
    void finish() const {
        if(actions.empty())return;
        if(next!=actions.size())throw std::runtime_error("automation ended before all actions completed");
        std::cout<<"automation_complete actions="<<next<<'\n';
    }
};
