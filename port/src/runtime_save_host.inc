// Port companion integration: original BTB remains independently readable.
byzantine::RuntimeViewSave capture_runtime_view(const SceneView& view) {
    byzantine::RuntimeViewSave saved;
    saved.scene=view.name;saved.background=view.background_name;
    saved.committed_background=view.committed_background;
    saved.layer=static_cast<std::uint8_t>(view.layer);
    if(panorama_layer(view) && view.panorama)saved.camera={{view.panorama->yaw_degrees(),view.panorama->pitch_degrees()}};
    saved.examine_image=view.trace.examine_image;
    const auto& state=view.state;
    saved.variables=state.variables;saved.variable_writes=state.variable_writes;
    saved.enabled=state.hotspot_enabled;saved.suspended=state.suspended_hotspots;
    saved.conversation=state.conversation_memory;saved.using_item=state.current_use_item;
    saved.selected=state.current_dialogue_record;saved.random=state.random_state;
    saved.morris_seed=state.morris_seed;saved.morris=state.morris;
    saved.input_active=state.dialogue_input_active;
    return saved;
}

void apply_runtime_view(SceneView& view,const byzantine::RuntimeViewSave& saved) {
    if(lower_ascii(view.name)!=lower_ascii(saved.scene) || saved.layer>2)
        throw std::runtime_error("runtime save scene/layer mismatch");
    auto& state=view.state;
    state.variables=saved.variables;state.variable_writes=saved.variable_writes;
    state.hotspot_enabled=saved.enabled;state.suspended_hotspots=saved.suspended;
    state.conversation_memory=saved.conversation;state.current_use_item=saved.using_item;
    state.current_dialogue_record=saved.selected;state.random_state=saved.random;
    state.morris_seed=saved.morris_seed;state.morris=saved.morris;
    state.dialogue_input_active=saved.input_active;
    view.layer=static_cast<SceneLayer>(saved.layer);
    view.committed_background=saved.committed_background;
    view.background_name=saved.background;
    view.trace.examine_image=saved.examine_image;
    view.trace.active_hotspots=byzantine::active_hotspots_for_state(view.script,state);
    if(saved.camera && view.panorama) {
        if(!std::isfinite(saved.camera->first) || !std::isfinite(saved.camera->second))
            throw std::runtime_error("runtime save invalid camera");
        view.panorama->advance(saved.camera->first-view.panorama->yaw_degrees(),
                               view.panorama->pitch_degrees()-saved.camera->second,1.0);
    }
    view.entry_state=state;
}

void write_runtime_context(const Session& session,const std::filesystem::path& path,
                           const std::vector<std::uint8_t>& btb) {
    byzantine::RuntimeSave saved;
    saved.btb=btb;saved.view=capture_runtime_view(session.view);
    if(session.return_armed && session.closeup_index && session.metadata) {
        saved.closeup=session.closeup_index;
        const auto& parent=session.return_view;
        std::optional<std::uint16_t> index;
        for(std::size_t i=0;i<session.metadata->scenes.size();++i)
            if(lower_ascii(session.metadata->scenes[i].scene)==lower_ascii(parent.name))index=static_cast<std::uint16_t>(i);
        if(!index)throw std::runtime_error("runtime save parent scene missing");
        const auto parent_save=build_save_from_state(session,nullptr,"Port examine parent",*index,
            byzantine::parse_save(btb).cd_number,&parent);
        saved.parent_btb=byzantine::write_save(parent_save);
        saved.parent=capture_runtime_view(parent);
    }
    const auto bytes=byzantine::write_runtime_save(std::move(saved));
    const auto destination=std::filesystem::path(path.string()+".byzport");
    const auto temporary=std::filesystem::path(destination.string()+".tmp");
    std::ofstream out(temporary,std::ios::binary|std::ios::trunc);
    out.write(reinterpret_cast<const char*>(bytes.data()),static_cast<std::streamsize>(bytes.size()));
    out.close();if(!out)throw std::runtime_error("runtime companion write failed");
    std::error_code error;std::filesystem::remove(destination,error);
    std::filesystem::rename(temporary,destination);
    std::cout<<"runtime_save file="<<destination<<" parent="<<bool(session.return_armed)<<'\n';
}

void restore_runtime_context(Session& session,const std::filesystem::path& path) {
    const auto companion=std::filesystem::path(path.string()+".byzport");
    if(!std::filesystem::is_regular_file(companion))return;
    auto saved=byzantine::read_runtime_save(read_file(companion));
    if(saved.btb!=read_file(path)) {
        std::cout<<"runtime_load rejected=btb_identity_mismatch file="<<companion<<'\n';return;
    }
    // Prepare the parent without running labels or presenting an intermediate
    // scene. Its tables come from the chapter metadata beside the current files.
    SceneView parent;
    if(saved.closeup) {
        const auto image=byzantine::parse_save(saved.parent_btb);
        if(!session.metadata || image.scene_index>=session.metadata->scenes.size())
            throw std::runtime_error("runtime save parent index outside chapter");
        const auto& name=session.metadata->scenes[image.scene_index].scene;
        auto cc=sibling_scene_file(session.view.cc_path,name);
        auto cb=sibling_scene_file(session.view.cb_path,name);
        if(!cc || !cb)throw std::runtime_error("runtime save parent tables missing");
        auto options=session.options.scene;
        options.erase("--save");
        for(const char* key:{"--ci","--cy","--cf","--cs","--ca","--co","--ce","--cd"}) {
            auto it=options.find(key);if(it==options.end())continue;
            auto sibling=sibling_scene_file(it->second,name);
            if(sibling)it->second=sibling->string();else options.erase(it);
        }
        parent.name=name;parent.cc_path=cc->string();parent.cb_path=cb->string();
        parent.script=byzantine::parse_cc(read_file(*cc));
        parent.entries=byzantine::parse_cb(read_file(*cb));
        auto setup=byzantine::load_scene_setup(parent.cc_path,options,&image);
        parent.state=std::move(setup.state);parent.pool_on_arrival=image.node_values;
        restore_saved_layer(session.options,parent,image,false);
        apply_runtime_view(parent,saved.parent);
        refresh_background(session.options,parent);
    }
    apply_runtime_view(session.view,saved.view);
    session.return_armed=bool(saved.closeup);
    session.closeup_index=saved.closeup;session.return_view=std::move(parent);
    std::cout<<"runtime_load file="<<companion<<" parent="<<session.return_armed<<'\n';
}
