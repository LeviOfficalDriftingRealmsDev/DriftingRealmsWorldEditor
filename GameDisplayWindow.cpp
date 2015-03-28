#include "./Include/GameDisplayWindow.hpp"
ResourceLoader::ResourceLoader(){
    ;
}

void ResourceLoader::LoadDefaultElements(clan::Canvas &c){

/*
    WorldElementTemplate *new_element = new WorldElementTemplate(
        ELEMENT_TYPES::DYNAMIC_OBSTACLE,
        new clan::Image(c,"Media/DefaultElements/StaticObstacles/generaic_shape_1.png")
    );

    p_element_templates.push_back(new_element);
*/ /// FIX LATER!
}



GameDisplayWindow::GameDisplayWindow(clan::GUIManager *manager, clan::DisplayWindow *p_dw){
//    GLOBAL_VARS::nav_map.amount_of_unit_radii = 4;
//    GLOBAL_VARS::nav_map.radii_list = new unsigned int[GLOBAL_VARS::nav_map.amount_of_unit_radii]{ 10, 30, 50, 70};

    display_window = p_dw;
    gui_manager = manager;

    drawing_path_data = false;
    draw_static_cursor = true;

    run_game = true;

    keyboard = &p_dw->get_ic().get_keyboard();

    clan::Slot key_slot = keyboard->sig_key_down().connect(this,&GameDisplayWindow::on_key);

    clan::GUITopLevelDescription comp_desc;
//    comp_desc.set_size(clan::Size(p_dw->get_geometry().get_width()/2, p_dw->get_geometry().get_height()/2), false);
    comp_desc.set_title("root comp");



    root_comp = new GUIComponent(manager,comp_desc);
//    root_comp = new GUIComponent(manager);
    root_comp->func_render().set(this, &GameDisplayWindow::render_root_comp);
    root_comp->func_input().set(this,&GameDisplayWindow::on_input);
    root_comp->func_close().set(this,&GameDisplayWindow::on_close);
    root_comp->set_window_geometry(Rect(0,0,p_dw->get_geometry().get_width(),p_dw->get_geometry().get_height()));

    clan::Slot resize = display_window->sig_resize().connect(this, &GameDisplayWindow::on_resize);

    root_canvas = new Canvas(root_comp->get_canvas());

    StaticCursorPosition = new Vec2f(root_comp->get_width()/2,root_comp->get_height()/2);
    StaticCursor = new clan::Image(*root_canvas,"GUIResources/static_cursor.png");

    build_window_tool_bar();

    custom_element_creator = new CustomElementCreatorWindow(root_comp,Rect(0,0,p_dw->get_geometry().get_width(),p_dw->get_geometry().get_height()));
    trigger_editor = new TriggerEditor(root_comp,Rect(0,0,p_dw->get_geometry().get_width(),p_dw->get_geometry().get_height()),root_canvas);

    cam_position = Vec2f(0,0);

    tool_bar = new ::ToolBar(root_comp,Rect(p_dw->get_geometry().get_width()-300,20,p_dw->get_geometry().get_width(),p_dw->get_geometry().get_height()),&root_comp->func_input());


    //! the debug draw font
    GLOBAL_VARS::dfont = new clan::Font(*root_canvas, "GUIResources/Tahoma.ttf", 14);

//    GLOBAL_VARS::element_list.push_back(new AreaElement());

    while(run_game){
        Loop();
    }

}
void GameDisplayWindow::on_resize(int x, int y){
    root_comp->set_window_geometry(Rect(0,0,x,y));
    tool_bar->resize(Rect(x-300,20,x,y));
    main_menu_bar->set_geometry(Rectf(0,0,x,20));
    trigger_editor->set_geometry(Rect(0,0,x,y));
}

GameDisplayWindow::~GameDisplayWindow(){
    delete StaticCursorPosition;
//!already deleted in GameInstance.cpp
    delete gui_manager;
    delete root_canvas;
//    delete StaticCursor;
//    delete custom_element_creator;
//    delete trigger_editor;
//    delete tool_bar;
}

void GameDisplayWindow::on_key(const clan::InputEvent &e){
    if(e.id == clan::keycode_escape){
        run_game=false;
    }

    if(visible){
        const int scroll_speed = 100*zoom;

        bool moved_cam = false;
        if(e.id == clan::keycode_up){
            cam_position.y -= scroll_speed;
            moved_cam = true;
        }
        if(e.id == clan::keycode_right){
            cam_position.x += scroll_speed;
            moved_cam = true;
        }
        if(e.id == clan::keycode_down){
            cam_position.y += scroll_speed;
            moved_cam = true;
        }
        if(e.id == clan::keycode_left){
            cam_position.x -= scroll_speed;
            moved_cam = true;
        }
        if(moved_cam){
            root_comp->request_repaint();
        }
//        if(cam_position.y < 0) cam_position.y = 0;
//        if(cam_position.x < 0) cam_position.x = 0;

    }
}

bool GameDisplayWindow::on_input(const clan::InputEvent &e){
    if(visible){
        if(NavMeshEditMode) GLOBAL_VARS::nav_map->Input(e,cam_position.x,cam_position.y,zoom);
        if(AngleEditMode) GLOBAL_VARS::angle_map->Input(e,cam_position.x,cam_position.y,zoom);
        static bool grabbing = false;
        static bool scaling = false;
        static bool action = false;
        static Vec2f scroll_last = Vec2f(0,0);
        static bool scrolling = false;
        if(!is_cursor_over_gui()){
            if(e.id == InputCode::mouse_middle && IsPressed(e)){
                scroll_last = Vec2f(e.mouse_pos);
                scrolling = true;
            }
            if(e.id == InputCode::mouse_middle && e.type == InputEvent::Type::released){
                scroll_last = Vec2f(e.mouse_pos);
                scrolling = false;
            }
            if(e.id == InputCode::mouse_left){
                if(IsPressed(e)){
                    if(action == true){
                        //! place actions that need a mouse click to finish here
                        grabbing = false;
                        scaling = false;
                        action = false;
                    }else if(!NavMeshEditMode && !AngleEditMode){
                        StaticCursorPosition->x = (e.mouse_pos.x)*zoom + cam_position.x;
                        StaticCursorPosition->y = (e.mouse_pos.y)*zoom + cam_position.y;
                        root_comp->request_repaint();
                    }
                }
            }
            if(IsPressed(e) && e.id == keycode_t){
                if(tool_bar->is_visible()){
                    tool_bar->set_visible(false,true);
                }
                else{
                    tool_bar->set_visible(true,true);
                }
            }


            //! selection
            if(IsPressed(e) && e.id == mouse_right && !action){
                if(!NavMeshEditMode && !AngleEditMode){
                    if(Selection(e))
                        root_comp->request_repaint();
                }
            }
        }



        if(e.type == e.pointer_moved){
            CursorPosition.x = e.mouse_pos.x;
            CursorPosition.y = e.mouse_pos.y;
            if(grabbing){
                for(unsigned int i = 0;i < GLOBAL_VARS::element_list.size();i++){
                    auto &elem = GLOBAL_VARS::element_list[i];
                    if(elem->selected){
                        elem->Grabbing(Vec2f(e.mouse_pos));
                        if(elem->type == WORLD_EDITOR_STATIC_OBSTACLE)
                            GLOBAL_VARS::nav_map->MovedPiece(i);
                    }
                }
            }
            if(scaling){
                for(auto &elem : GLOBAL_VARS::element_list){
                    if(elem->selected){
                        elem->Scaling(Vec2f(e.mouse_pos));
                    }
                }
            }

            if(scrolling){
                cam_position -= (Vec2f(e.mouse_pos)-scroll_last)*zoom;
                scroll_last = Vec2f(e.mouse_pos);
            }


            root_comp->request_repaint();
        }
        if(e.id == keycode_a && IsPressed(e) && !NavMeshEditMode && !AngleEditMode){
            for(auto &elem : GLOBAL_VARS::element_list){
                elem->selected = true;
            }
        }
        //! 91 = [ key
        //! 93 = ] key
        if(e.id == 91 && IsPressed(e)){
            int i = 0;
            for(auto &elem : GLOBAL_VARS::element_list){
                if(elem->type == WORLD_EDITOR_STATIC_OBSTACLE && elem->selected){
                    StaticElement *my_element = static_cast<StaticElement *>(elem);
                    if(my_element->is_flipped_horizontal){
                        my_element->is_flipped_horizontal = false;
//                        my_element->SetPosition(Vec2f(my_element->GetX()-my_element->element_template_data->texture->get_width(),my_element->GetY()));
                        GLOBAL_VARS::angle_map->Flipped_x(i);
                    }else{
                        my_element->is_flipped_horizontal = true;
//                        my_element->SetPosition(Vec2f(my_element->GetX()+my_element->element_template_data->texture->get_width(),my_element->GetY()));
                        GLOBAL_VARS::angle_map->Flipped_x(i);
                    }
                    my_element->FlipCollisionHorizontal();
                    my_element->ScaleCollision(zoom,cam_position);
                    GLOBAL_VARS::nav_map->MovedPiece(i);
                }
                i++;
            }
        }
        if(e.id == 93 && IsPressed(e)){
            int i = 0;
            for(auto &elem : GLOBAL_VARS::element_list){
                if(elem->type == WORLD_EDITOR_STATIC_OBSTACLE && elem->selected){

                    StaticElement *my_element = static_cast<StaticElement *>(elem);
                    if(my_element->is_flipped_vertical){
                        my_element->is_flipped_vertical = false;
                        my_element->SetPosition(Vec2f(my_element->GetX(),my_element->GetY()-my_element->element_template_data->texture->get_height()));
                        GLOBAL_VARS::angle_map->Flipped_y(i);
                    }else{
                        my_element->is_flipped_vertical = true;
                        my_element->SetPosition(Vec2f(my_element->GetX(),my_element->GetY()+my_element->element_template_data->texture->get_height()));
                        GLOBAL_VARS::angle_map->Flipped_y(i);
                    }
                    my_element->FlipCollisionVertical();
                    my_element->ScaleCollision(zoom,cam_position);
                    GLOBAL_VARS::nav_map->MovedPiece(i);
                }
                i++;
            }
        }
        if(e.id == keycode_g && IsPressed(e)){
            for(auto &elem : GLOBAL_VARS::element_list){
                if(elem->selected){
                    elem->StartGrab(Vec2f(e.mouse_pos));

                    //! not preferable to call for everything selected, but if not then will try to grab when nothing is selected
                    grabbing = true;
                    action = true;
                }
            }
        }
        if(e.id == keycode_s && IsPressed(e)){
            for(auto &elem : GLOBAL_VARS::element_list){
                if(elem->selected){
                    elem->StartScale(Vec2f(e.mouse_pos));

                    //! not preferable to call for everything selected, but if not then will try to grab when nothing is selected
                    scaling = true;
                    action = true;
                }
            }
        }


        if(e.id == keycode_delete && IsPressed(e)){
            for(unsigned int i = 0;i < GLOBAL_VARS::element_list.size();i++){
                if(GLOBAL_VARS::element_list[i]->selected){
                    GLOBAL_VARS::element_list[i]->Delete();
                    GLOBAL_VARS::element_list.erase(GLOBAL_VARS::element_list.begin() + i);
                    i--;
                }
            }
        }

        if(!GLOBAL_VARS::nav_map->circle_select){
            if(e.id == clan::mouse_wheel_down){
                zoom *= 1.1;
                if(zoom > 20) zoom = 20;
                UpdateCollisionsFromZoom();
            }
            else if(e.id == clan::mouse_wheel_up){
                zoom /= 1.1;
                if(zoom < 1) zoom = 1;
                UpdateCollisionsFromZoom();
            }
        }
    }else if(!trigger_editor->is_closed){
        trigger_editor->on_input(e);
    }

    return 0;
}

bool GameDisplayWindow::IsAltEditor(){
    return NavMeshEditMode || AngleEditMode || !visible;
}

void GameDisplayWindow::UpdateCollisionsFromZoom(){
    for(unsigned int i = 0;i < GLOBAL_VARS::element_list.size();i++){
        GLOBAL_VARS::element_list[i]->ScaleCollision(zoom,cam_position);
    }
}
bool GameDisplayWindow::Selection(const clan::InputEvent &e){
    static int test = 0;
    static int last_selected = -1;
    std::vector<int> selected;

    //! I know I could do a normal for loop, but I think this looks better
    int iter = 0;
    for(auto &elem : GLOBAL_VARS::element_list){
        elem->ScaleCollision(zoom,cam_position);
        if(elem->IsPointInside(Pointf(e.mouse_pos))){
            selected.push_back(iter);
        }if(!e.shift)
            elem->selected = false;
        iter++;
    }
    if(selected.size() == 1){
        GLOBAL_VARS::element_list[selected[0]]->selected = true;
        last_selected = selected[0];
    }else if(selected.size() > 1){
        if(last_selected == -1){
            GLOBAL_VARS::element_list[selected[0]]->selected = true;
            last_selected = selected[0];
        }else{
            //! if there arn't any further in the vector we need to cycle back
            bool are_any_ahead = false;
            for(unsigned int i = 0;i < selected.size();i++){
                if(selected[i] > last_selected && !are_any_ahead){
                    GLOBAL_VARS::element_list[selected[i]]->selected = true;
                    last_selected = selected[i];
                    are_any_ahead = true;
                }
            }
            if(!are_any_ahead){
                GLOBAL_VARS::element_list[selected[0]]->selected = true;
                last_selected = selected[0];
            }
        }
    }else{
        return false;
    }
    return true;
}


bool GameDisplayWindow::is_cursor_over_gui(){
    for(unsigned int i=0; i<root_comp->get_child_components().size(); ++i){
        if(root_comp->get_child_components()[i]->is_visible()){
            if(root_comp->get_child_components()[i]->get_geometry().contains(CursorPosition)){
                return true;
            }
        }
   }

    return false;
}

void GameDisplayWindow::render_root_comp(clan::Canvas &c, const clan::Rect &clip_rect){
    c.clear(clan::Colorf(0.0f,0.0f,0.0f,1.0f));

    c.fill_rect(
        Rectf(0,root_comp->get_height()/2,root_comp->get_width(),root_comp->get_height()),
        clan::Gradient(
                Colorf(0.0f,0.0f,0.0f,1.0f),
                Colorf(0.0f,0.0f,0.0f,1.0f),
                Colorf(0.2f,0.2f,0.2f,1.0f),
                Colorf(0.2f,0.2f,0.2f,1.0f)
        )
    );

    if(visible){



        for(auto &elem : GLOBAL_VARS::element_list){
            elem->Draw(c, cam_position,zoom);
        }


        c.draw_line(-cam_position.x/zoom,0,-cam_position.x/zoom,root_comp->get_height(),Colorf::green);
        c.draw_line(0,-cam_position.y/zoom,root_comp->get_width(),-cam_position.y/zoom,Colorf::green);

        if(AngleEditMode)
            GLOBAL_VARS::angle_map->Draw(c,cam_position.x,cam_position.y,zoom);
        if(NavMeshEditMode)
            GLOBAL_VARS::nav_map->Draw(c,cam_position.x,cam_position.y,zoom);

        if(draw_static_cursor && !NavMeshEditMode){
            StaticCursor->draw(c,(StaticCursorPosition->x-(StaticCursor->get_width()/2) - cam_position.x)/zoom,(StaticCursorPosition->y-(StaticCursor->get_height()/2) - cam_position.y)/zoom);
        }
        tool_bar->request_repaint();
    }else{
        visible = custom_element_creator->is_closed && trigger_editor->is_closed;
        if(custom_element_creator->is_closed && trigger_editor->is_closed)
            tool_bar->set_visible(true);
    }
}

void GameDisplayWindow::Loop(){
    gui_manager->process_messages(-1);
//    gui_manager->exec();
}

void GameDisplayWindow::build_window_tool_bar(){

    main_menu_bar = new MenuBar(root_comp);

    /// Add the file menu ---------------------------------------->
    clan::PopupMenu file_menu;
    file_menu.insert_item("New");
    file_menu.insert_item("Import");
    file_menu.insert_item("Export");
    file_menu.insert_item("").set_separator(true);
    file_menu.insert_item("Settings");

    file_menu.get_item_at(0).func_clicked().set(this,&GameDisplayWindow::action_open_world_builder);
    file_menu.get_item_at(1).func_clicked().set(this,&GameDisplayWindow::Import);
    file_menu.get_item_at(2).func_clicked().set(this,&GameDisplayWindow::Export);

    main_menu_bar->add_menu("File", file_menu);
    /// Add the file menu ---------------------------------------->


    /// Add the tools menu ---------------------------------------->
    clan::PopupMenu tools_menu;
    tools_menu.insert_item("Open Tool Box");
    tools_menu.insert_item("Open Components Box");
    tools_menu.insert_item("Open Triggers Box");
    tools_menu.insert_item("Open Game Element Box");
    tools_menu.insert_item("Open Component Builder");
    tools_menu.insert_item("Open Trigger Editor");
    tools_menu.insert_separator();
    tools_menu.insert_item("Open Navigation Map Editor");
    tools_menu.insert_item("Open Navigation Angle Editor");
    tools_menu.insert_item("Generate Outside Nav Points");
    tools_menu.insert_separator();
    tools_menu.insert_item("Mirror Map X");
    tools_menu.insert_item("Mirror Map Y");

    tools_menu.get_item_at(0).func_clicked().set(this,&GameDisplayWindow::action_open_tool_box);
    tools_menu.get_item_at(1).func_clicked().set(this,&GameDisplayWindow::action_open_components_box);
    tools_menu.get_item_at(2).func_clicked().set(this,&GameDisplayWindow::action_open_triggers_box);
    tools_menu.get_item_at(3).func_clicked().set(this,&GameDisplayWindow::action_open_game_elements_tool_box);
    tools_menu.get_item_at(4).func_clicked().set(this,&GameDisplayWindow::action_open_element_builder);
    tools_menu.get_item_at(5).func_clicked().set(this,&GameDisplayWindow::action_open_trigger_editor);
    tools_menu.get_item_at(7).func_clicked().set(this,&GameDisplayWindow::action_open_navigation_map_tool);
    tools_menu.get_item_at(8).func_clicked().set(this,&GameDisplayWindow::action_open_navigation_map_angle_tool);
    tools_menu.get_item_at(9).func_clicked().set(this,&GameDisplayWindow::action_generate_nav_map_start);
    tools_menu.get_item_at(11).func_clicked().set(this,&GameDisplayWindow::action_mirror_map_x);
    tools_menu.get_item_at(12).func_clicked().set(this,&GameDisplayWindow::action_mirror_map_y);

    main_menu_bar->add_menu("Tools", tools_menu);
    /// Add the tools menu ---------------------------------------->

    /// Add the view menu ---------------------------------------->
    clan::PopupMenu view_menu;
    view_menu.insert_item("Toggle 2D Cursor");
    view_menu.insert_item("Toggle Chain Shapes");

    view_menu.get_item_at(0).func_clicked().set(this,&GameDisplayWindow::action_toggle_static_cursor);
    view_menu.get_item_at(1).func_clicked().set(this,&GameDisplayWindow::action_toggle_draw_path_data);

    main_menu_bar->add_menu("View", view_menu);

    /// Add the view menu ---------------------------------------->
    main_menu_bar->set_geometry(Rectf(0,0,root_comp->get_geometry().get_width(),20));

    main_menu_bar->set_constant_repaint(true);
}


void GameDisplayWindow::action_toggle_draw_path_data(){
    drawing_path_data = !drawing_path_data;
    root_comp->request_repaint();
}

void GameDisplayWindow::action_toggle_static_cursor(){
    draw_static_cursor = !draw_static_cursor;
    root_comp->request_repaint();
}

void GameDisplayWindow::make_a_new_world(){

//    const int new_world_width = new_world_width_field->get_text_int();
//    const int new_world_height= new_world_height_field->get_text_int();
}

void GameDisplayWindow::action_open_game_elements_tool_box(){
    tool_bar->add_game_elements_tool(StaticCursorPosition);
}


void GameDisplayWindow::action_open_tool_box(){
//    tool_box->set_visible(true);//
}

void GameDisplayWindow::action_open_components_box(){
    tool_bar->add_component_builder(StaticCursorPosition);
}
void GameDisplayWindow::action_open_triggers_box(){
    tool_bar->add_trigger_builder(StaticCursorPosition);
}
void GameDisplayWindow::action_mirror_map_x(){
    //! check to see if we even have a map size(default is zero as to indicate we have none)
    if(abs(map_rect.get_width()) > 0 && abs(map_rect.get_height()) > 0){
        //! we will be expanding this so it's important to not iterate over the ones we just made
        const int elem_size = GLOBAL_VARS::element_list.size();
        std::cout << elem_size << "\n";
        for(int i = 0;i < elem_size;i++){
            GLOBAL_VARS::element_list.push_back(GLOBAL_VARS::element_list[i]->MirrorX(map_rect));
        }

//        GLOBAL_VARS::angle_map->MirrorX();
        GLOBAL_VARS::nav_map->MirrorX(map_rect);
    }
}
void GameDisplayWindow::action_mirror_map_y(){
//    for(int i = 0;i < )

}



void GameDisplayWindow::Import(){

    WorldData *data = FileHelper::read_world_data("6s.DRM");
//    for(int i = 0; i < data->nav_point_list.size();i++){
//        GLOBAL_VARS::nav_maps[i].SetNavPointList(data->nav_point_list[i]);
//    }
    for(unsigned int i = 0;i < data->areas.size();i++){
        auto *area = new AreaElement();
        area->SetRect(data->areas[i]);
        GLOBAL_VARS::element_list.push_back(area);
    }

    GLOBAL_VARS::nav_map->LoadFrom(data->mesh_load);

    std::vector<std::vector<Vec2f> > chains;
    for(unsigned int i = 0;i < data->mesh_load.list_of_chain_points.size();i++){
        std::vector<Vec2f> temp;
        for(unsigned int j = 0;j < data->mesh_load.list_of_chain_points[i].size();j++){
            temp.push_back(Vec2f(GLOBAL_VARS::nav_map->points[data->mesh_load.list_of_chain_points[i][j]].x() - data->save_load_static_mesh[i]->position.x,
                                 GLOBAL_VARS::nav_map->points[data->mesh_load.list_of_chain_points[i][j]].y() - data->save_load_static_mesh[i]->position.y));
            GLOBAL_VARS::nav_map->points[data->mesh_load.list_of_chain_points[i][j]].is_mutable = false;
            GLOBAL_VARS::nav_map->points[data->mesh_load.list_of_chain_points[i][j]].shape_attached_to = i;
            GLOBAL_VARS::nav_map->points[data->mesh_load.list_of_chain_points[i][j]].chain_attached_to = j;
        }
        chains.push_back(temp);
    }

    for(int i = 0; i < data->game_element_locations.size();i++){
        GLOBAL_VARS::element_list.push_back(new GameElement(data->game_element_locations[i],data->game_element_types[i]));
    }

    GLOBAL_VARS::angle_map->LoadFrom(data->mesh_load);

    for(unsigned int i = 0;i < data->save_load_static_mesh.size();i++){


        //! create element
        ElementTemplate *element_template_data = new ElementTemplate();
        StaticElement *new_elem = new StaticElement();
        new_elem->element_template_data = element_template_data;

        //! get template_name and texture
        element_template_data->texture_name = data->save_load_static_mesh[i]->texture_name;

        element_template_data->chain_points = chains[i];



        new_elem->element_template_data->points_list_cnt = new_elem->element_template_data->chain_points.size();
        new_elem->element_template_data->points_list = new TMath::Point[new_elem->element_template_data->points_list_cnt];
        for(unsigned int i = 0;i < new_elem->element_template_data->chain_points.size();i++){
            new_elem->element_template_data->points_list[i] = TMath::Point(new_elem->element_template_data->chain_points[i].x,new_elem->element_template_data->chain_points[i].y);
        }



        element_template_data->texture = new clan::Sprite(*root_canvas,element_template_data->texture_name);
        new_elem->chain_points = element_template_data->chain_points;

//        //! we save the center of the points for Josh, but we use upper left coordinates. Convert back.
//        for(int i = 0;i < new_elem->chain_points.size();i++){
//            new_elem->chain_points[i] += Vec2f(element_template_data->texture->get_width()/2,element_template_data->texture->get_height()/2);
//        }

        new_elem->CreateOutline();

//        new_elem->SetPosition(data->save_load_static_mesh[i]->position - Vec2f(element_template_data->texture->get_width()/2,element_template_data->texture->get_height()/2));
        new_elem->SetPosition(data->save_load_static_mesh[i]->position);

        new_elem->is_flipped_vertical = data->save_load_static_mesh[i]->is_flipped_vertical;
        new_elem->is_flipped_horizontal = data->save_load_static_mesh[i]->is_flipped_horizontal;

//        for(int i = 0;i < data->nav_maps.size();i++)
//        std::cout << data->texture_names.size() << "\n";
//        std::cout << data->positions.size() << "\n";

        GLOBAL_VARS::element_list.push_back(new_elem);
    }

}
void GameDisplayWindow::Export(){
    File file("6s.DRM",File::OpenMode::create_always,File::AccessFlags::access_write);

    //! write out format type for checking
    FileHelper::write_str("Drifting Realms Map File", file);
    //! write out version
    file.write_uint32(VERSION);

    GLOBAL_VARS::nav_map->Save(file);
    GLOBAL_VARS::angle_map->Save(file);

    //! write out all the units
    file.write_uint32(GLOBAL_VARS::element_list.size());
    for(auto &element : GLOBAL_VARS::element_list){
        element->Save(file);
    }
}



void GameDisplayWindow::action_open_world_builder(){

//    int window_height = 250;
//    int window_width = 400;

//    int start_x = (root_comp->get_geometry().get_width() - window_width)/2;
//    int start_y = (root_comp->get_geometry().get_height() - window_height)/2;

}

void GameDisplayWindow::action_open_element_builder(){
    custom_element_creator->set_visible(true);
    custom_element_creator->is_closed = false;
    tool_bar->set_visible(false);
    visible = false;
}
void GameDisplayWindow::action_open_trigger_editor(){
    trigger_editor->set_visible(true);
    trigger_editor->is_closed = false;
    tool_bar->set_visible(false);
    visible = false;
}
void GameDisplayWindow::action_generate_nav_map_start(){
    map_rect = GLOBAL_VARS::nav_map->GenerateBoundery();
}
void GameDisplayWindow::action_open_navigation_map_tool(){
//    tool_bar->add_navigation_mesh_tool(StaticCursorPosition);
    AngleEditMode = false;
    NavMeshEditMode = !NavMeshEditMode;
    if(NavMeshEditMode){
        for(unsigned int i = 0;i < GLOBAL_VARS::element_list.size();i++){
            GLOBAL_VARS::element_list[i]->selected = false;
        }
    }
}
void GameDisplayWindow::action_open_navigation_map_angle_tool(){
    NavMeshEditMode = false;

    AngleEditMode = !AngleEditMode;
    if(AngleEditMode){
        for(unsigned int i = 0;i < GLOBAL_VARS::element_list.size();i++){
            GLOBAL_VARS::element_list[i]->selected = false;
        }
    }
}

bool GameDisplayWindow::on_close(){
//    exit_with_code(0);
    delete GLOBAL_VARS::dfont;
    return 0;
}
