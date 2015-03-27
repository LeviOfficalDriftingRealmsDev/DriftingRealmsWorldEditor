#include "ElementBrowser.hpp"


//ElementBrowser::ElementBrowser(clan::GUIComponent *parent,Rect f , clan::Vec2f *static_cursor_pos, clan::Callback_1<bool, const clan::InputEvent &> *call_back) : GUIComponent(parent, GUITopLevelDescription(f,true), ""){
ElementBrowser::ElementBrowser(clan::GUIComponent *parent,Rect f , clan::Vec2f *static_cursor_pos, clan::Callback_1<bool, const clan::InputEvent &> *call_back) : GUIComponent(parent){
    set_geometry(f);
    currently_seleceted_item = 0;

    StaticCursorPosition = static_cursor_pos;


    int start_x = 0;
    int start_y = 0;
//    this->func_input().set(this,&ElementBrowser::on_input);

    SelectionButtonField = new clan::GUIComponent(this);
    SelectionButtonField->set_geometry(Rectf(0,0,248,35));
//    SelectionButtonField->func_render().set(this,&ElementBrowser::draw_box_bg);

    button_t_1 = new clan::PushButton(SelectionButtonField);
    button_t_1->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));
    button_t_1->set_focus_policy(focus_refuse);

    start_x += 34;
    button_t_2 = new clan::PushButton(SelectionButtonField);
    button_t_2->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));
    button_t_2->set_focus_policy(focus_refuse);

    start_x += 34;
    button_t_3 = new clan::PushButton(SelectionButtonField);
    button_t_3->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));
    button_t_3->set_focus_policy(focus_refuse);

    start_x += 34;
    button_t_4 = new clan::PushButton(SelectionButtonField);
    button_t_4->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));
    button_t_4->set_focus_policy(focus_refuse);

    start_x += 34;
    button_t_5 = new clan::PushButton(SelectionButtonField);
    button_t_5->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));
    button_t_5->set_focus_policy(focus_refuse);

    start_x += 46;
    refresh_elements_button = new clan::PushButton(SelectionButtonField);
//    refresh_elements_button->set_geometry(Rectf(start_x,start_y,start_x+58,start_y+32));
    refresh_elements_button->set_geometry(Rectf(get_geometry().right-58-5,start_y,get_geometry().right-5,start_y+32));
    refresh_elements_button->set_text("Reload List");
    refresh_elements_button->func_clicked().set(this,&ElementBrowser::on_reload_components_list);
    refresh_elements_button->set_focus_policy(focus_refuse);

    start_x = 0;
    start_y = 35;

    ComponentListContainer = new clan::GUIComponent(this);
    ComponentListContainer->set_geometry(Rectf(start_x,start_y,get_geometry().right-25,get_geometry().bottom-66));
//    ComponentListContainer->func_render().set(this,&ElementBrowser::draw_comp_box_bg);
    ComponentListContainer->func_input().set(this,&ElementBrowser::on_input);
    ComponentListContainer->set_focus_policy(focus_refuse);

    start_x = 6;
    start_y = this->get_height()-2;

    add_element_button = new clan::PushButton(this);
    add_element_button->set_geometry(Rectf(start_x,start_y-30,start_x+70,start_y-6));
    add_element_button->set_text("Place");
    add_element_button->func_clicked().set(this,&ElementBrowser::add_element);
    add_element_button->set_focus_policy(focus_refuse);

    start_x = 100;
    start_y = this->get_height()-2;

    center_check_box = new clan::CheckBox(this);
    center_check_box->set_geometry(Rectf(start_x,start_y-22,start_x+14,start_y-10));
    center_check_box->set_focus_policy(focus_refuse);

    start_x = 120;
    center_label = new clan::Label(this);
    center_label->set_geometry(Rectf(start_x,start_y-22,start_x+34,start_y-10));
    center_label->set_text("Center");
    center_label->set_focus_policy(focus_refuse);

    start_x = this->get_width()-20;
    start_y = 35;


    scroll_bar = new clan::ScrollBar(this);
    scroll_bar->set_vertical();
    scroll_bar->set_geometry(Rectf(start_x,start_y,start_x+20,get_geometry().bottom));
    scroll_bar->set_max(100);
    scroll_bar->set_focus_policy(focus_refuse);

    clan::Canvas can(this->get_top_level_component()->get_canvas());

    // load the stock components into the editor
    DriftingRealmsXMLReader::reload_element_templates(GLOBAL_VARS::element_template_list,can);

    for(unsigned int i=0; i<GLOBAL_VARS::element_template_list.size(); ++i){
        element_list.push_back( new ElementListItem(ComponentListContainer,i,GLOBAL_VARS::element_template_list[i]) );
    }

    this->set_visible(false);
    this->call_back = call_back;


    func_resized().set(this,&ElementBrowser::on_resize);
    set_focus_policy(focus_refuse);
}

void ElementBrowser::on_resize(){
    int start_x = 0;
    int start_y = 0;

    SelectionButtonField->set_geometry(Rectf(0,0,248,35));

    button_t_1->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));

    start_x += 34;
    button_t_2->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));

    start_x += 34;
    button_t_3->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));

    start_x += 34;
    button_t_4->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));

    start_x += 34;
    button_t_5->set_geometry(Rectf(start_x,start_y,start_x+32,start_y+32));

    start_x += 46;

    //!fix the weird dissaperance
//    std::cout << Rectf(get_geometry().right-58-5,start_y,get_geometry().right-5,start_y+32) << "\n";
    refresh_elements_button->set_geometry(Rectf(get_geometry().right-58-5,start_y,get_geometry().right-5,start_y+32));

    start_x = 0;
    start_y = 35;

    ComponentListContainer->set_geometry(Rectf(start_x,start_y,get_geometry().right-25,get_geometry().bottom-66));

    start_x = 6;
    start_y = this->get_height()-2;

    add_element_button->set_geometry(Rectf(start_x,start_y-30,start_x+70,start_y-6));

    start_x = 100;
    start_y = this->get_height()-2;

    center_check_box->set_geometry(Rectf(start_x,start_y-22,start_x+14,start_y-10));

    center_label->set_geometry(Rectf(start_x,start_y-22,start_x+34,start_y-10));

    start_x = this->get_width()-20;
    start_y = 35;

    scroll_bar->set_geometry(Rectf(start_x,start_y,start_x+20,get_geometry().bottom));

    for(unsigned int i=0; i<GLOBAL_VARS::element_template_list.size(); ++i){
        element_list[i]->resize();
    }
}


bool ElementBrowser::on_input(const clan::InputEvent &e){

    if(IsPressed(e) && e.id == mouse_left){
        for(unsigned int i=0; i<element_list.size(); ++i){
//            if(element_list[i]->get_geometry().contains(Vec2f(e.mouse_pos-ComponentListContainer->get_geometry().get_top_left()))){
            if(element_list[i]->get_geometry().contains(Vec2f(e.mouse_pos))){
                currently_seleceted_item = element_list[i];
                element_list[i]->set_selected(true);
            }else{
                element_list[i]->set_selected(false);
            }
        }

        // this needs testing
        request_repaint();
    }else{
//        call_back->invoke(e);
    }

    for(unsigned int i=0; i<element_list.size(); ++i){
        if(element_list[i]->delete_me){
            DriftingRealmsXMLWriter::remove_element_template_from_xml_file(*element_list[i]->list_template);

            delete element_list[i];
            element_list.erase(element_list.begin()+i);
        }
    }

    return 0;
}


void ElementBrowser::on_reload_components_list(){

    for(unsigned int i=0; i<element_list.size(); ++i){
        delete element_list[i];
    }

    element_list.clear();

    clan::Canvas can(this->get_top_level_component()->get_canvas());

    // load the stock components into the editor
    DriftingRealmsXMLReader::reload_element_templates(GLOBAL_VARS::element_template_list,can);

    for(unsigned int i=0; i<GLOBAL_VARS::element_template_list.size(); ++i){
        ElementTemplate *ct = GLOBAL_VARS::element_template_list[i];
        element_list.push_back( new ElementListItem(ComponentListContainer,i,ct) );
    }
}



void ElementBrowser::add_element(){
    if(currently_seleceted_item){
        StaticElement *new_elem = new StaticElement;

        new_elem->element_template_data = currently_seleceted_item->list_template;
        new_elem->chain_points = new_elem->element_template_data->chain_points;


        new_elem->CreateOutline();
        new_elem->SetPosition(*StaticCursorPosition);


        if(center_check_box->is_checked()){
            new_elem->Move(Vec2f(-new_elem->element_template_data->texture->get_width()/2,-new_elem->element_template_data->texture->get_height()/2));
//            new_elem->element_position.y-=;
        }

        GLOBAL_VARS::element_list.push_back(new_elem);
        GLOBAL_VARS::nav_map->GeneratePiece();
        GLOBAL_VARS::angle_map->GenerateShape();

        this->get_top_level_component()->request_repaint();
    }
}


//void ElementBrowser::draw_comp_box_bg(Canvas &c,const Rect &clip){
//    c.fill_rect(
//        0,
//        0,
//        ComponentListContainer->get_width(),
//        ComponentListContainer->get_height(),
//        Colorf(0.1f,0.1f,0.1f,1.0f)
//    );
//}
