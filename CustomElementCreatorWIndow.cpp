#include "CustomElementCreatorWIndow.hpp"
#include "ToolBar.hpp"

CustomElementCreatorWindow::CustomElementCreatorWindow(clan::GUIComponent *parent, Rectf f) : GUIComponent(parent){

    int window_height = f.get_height();
    int window_width = f.get_width();

    int start_x = 0;
    int start_y = 0;
    this->set_geometry(Rectf(start_x,start_y,start_x+window_width,start_y+window_height));
//    this->set_geometry(f);
//    this->set_title("Component Builder - Make custom components");
    this->func_input().set(this,&CustomElementCreatorWindow::on_input);
    this->set_visible(false);

    int button_spacing = 27;
    int button_group_spacing = button_spacing + 10;
    start_x = 0;
    start_y = 0;

    //! put this before control_area so it is put first in the array and thus drawn first
    creation_area = new clan::GUIComponent(this);

    control_area = new clan::GUIComponent(this);
    control_area->set_geometry(Rectf(start_x,start_y+0,start_x+95,window_height-20));
    control_area->func_render().set(this,&CustomElementCreatorWindow::draw_element_control);

    start_x = 7;
    start_y = 7;

    add_new_bitmap_button = new clan::PushButton(control_area);
    add_new_bitmap_button->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    add_new_bitmap_button->set_text("Load Bitmap");
    add_new_bitmap_button->func_clicked().set(this,&CustomElementCreatorWindow::open_file_opener);
    start_y += button_spacing;

    remove_bitmap_button = new clan::PushButton(control_area);
    remove_bitmap_button->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    remove_bitmap_button->set_text("Remove Bitmap");
    start_y += button_group_spacing;

    /// ---------------------------------------------------------------------------------------------------------
    /// collision button controls

    add_collision_points_buttons = new clan::PushButton(control_area);
    add_collision_points_buttons->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    add_collision_points_buttons->set_text("Add Points");
    add_collision_points_buttons->func_clicked().set(this,&CustomElementCreatorWindow::on_enter_point_mode);
    start_y += button_spacing;

    close_collision_points_buttons = new clan::PushButton(control_area);
    close_collision_points_buttons->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    close_collision_points_buttons->set_text("Close Points");
    close_collision_points_buttons->func_clicked().set(this,&CustomElementCreatorWindow::on_close_current_points);
    start_y += button_spacing;

    /*move_collision_points_button = new clan::PushButton(control_area);
    move_collision_points_button->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    move_collision_points_button->set_text("Move Points");
    */
//    start_y += button_spacing;

    remove_collision_points_buttons = new clan::PushButton(control_area);
    remove_collision_points_buttons->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    remove_collision_points_buttons->set_text("Remove Points");
    remove_collision_points_buttons->func_clicked().set(this,&CustomElementCreatorWindow::on_enter_point_delete_mode);
    start_y += button_group_spacing;


    quality_menu.insert_item("select quality", -1, -1);
    quality_menu.insert_separator(-1);
    quality_menu.insert_item("poor", accuracy_medium, -1);//! for some reasom medium gives the worst quality
    quality_menu.insert_item("low", accuracy_poor, -1);//! poor and low both seem to do the same thing
    quality_menu.insert_item("medium", accuracy_low, -1);
    quality_menu.insert_item("high", accuracy_high, -1);//! high and raw both seem to do the same thing
    quality_menu.insert_item("raw", accuracy_raw, -1);

    quality_combo_box = new clan::ComboBox(control_area);
    quality_combo_box->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    quality_combo_box->set_text("select quality");
    quality_combo_box->set_popup_menu(quality_menu);
    start_y += button_group_spacing;

    tolerance_label = new clan::Label(control_area);
    tolerance_label->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+5));
    tolerance_label->set_text("input tolerance");
    start_y += button_group_spacing - 25;

    tolerance_spin = new clan::Spin(control_area);
    tolerance_spin->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+15));
    tolerance_spin->set_value(128);
    tolerance_spin->set_ranges(0,255);
    start_y += button_spacing - 5; //spins are thinner

    generate_button = new clan::PushButton(control_area);
    generate_button->set_geometry(Rectf(start_x,start_y,start_x+80,start_y+20));
    generate_button->set_text("Generate Points");
    generate_button->func_clicked().set(this,&CustomElementCreatorWindow::generate_collision);

    start_y += button_group_spacing;

    /// collision button controls
    /// ---------------------------------------------------------------------------------------------------------

    save_button = new clan::PushButton(control_area);
    save_button->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    save_button->set_text("Save Component");
    save_button->func_clicked().set(this,&CustomElementCreatorWindow::on_open_save_custom_dialog);

    start_y += button_group_spacing;

    close = new clan::PushButton(control_area);
    close->set_geometry(Rectf(start_x,start_y+0,start_x+80,start_y+20));
    close->set_text("Close");
    close->func_clicked().set(this,&CustomElementCreatorWindow::on_close);

    start_x = 95;
    start_y = 0;

    creation_area->set_geometry(Rectf(start_x,start_y+0,window_width,window_height));
    creation_area->func_input().set(this,&CustomElementCreatorWindow::on_creation_area_input);
    creation_area->func_render().set(this,&CustomElementCreatorWindow::draw_element);
    creation_area->set_clip_children(true);

    start_x = control_area->get_width()+2;
    start_y = this->get_height()-42;

    status_area = new clan::GUIComponent(this);
    status_area->set_geometry(Rectf(start_x,start_y+0,start_x+creation_area->get_width()-2,start_y+20));
    status_area->func_render().set(this,&CustomElementCreatorWindow::draw_element_status);

    start_x = 10;
    start_y = 0;

    x_pos_label = new clan::Label(status_area);
    x_pos_label->set_geometry(Rectf(start_x,start_y,start_x+60,start_y+15));
    x_pos_label->set_text("mouse x :");
    x_pos_label->set_text_color(Colorf(1.0f,1.0f,1.0f,1.0f));

    start_x = x_pos_label->get_width()+10;
    start_y = 0;

    y_pos_label = new clan::Label(status_area);
    y_pos_label->set_geometry(Rectf(start_x,start_y,start_x+60,start_y+15));
    y_pos_label->set_text("mouse y :");
    y_pos_label->set_text_color(Colorf(1.0f,1.0f,1.0f,1.0f));

    base_bitmap = 0;

    my_file_getter = new OpenFileDialogWindow(creation_area);
    my_file_getter->func_visibility_change().set(this,&CustomElementCreatorWindow::LoadBitMap);

    scroll_cam_x = 0;
    scroll_cam_y = 0;

    moving_current_point = true;
    in_point_edit_mode = false;
    in_point_remove_mode = false;
    current_point_index = 0;

    save_dialog = new SimpleOneLineDialogWindow(creation_area);
    save_dialog->set_new_name_button->func_clicked().set(this,&CustomElementCreatorWindow::on_save_custom_dialog);
    save_dialog->set_visible(false);

    points.push_back(Vec2f());

}
void CustomElementCreatorWindow::generate_collision(){
    CollisionOutline outline(current_editor_template.texture_name,tolerance_spin->get_value(),clan::OutlineAccuracy(quality_menu.get_item_at(quality_combo_box->get_selected_item()).get_id()));
    //! remove all other points first
    points.clear();
    for(unsigned int y = 0;y < outline.get_contours()[0].get_points().size(); y++){
        points.push_back(Vec2f(outline.get_contours()[0].get_points()[y].x,outline.get_contours()[0].get_points()[y].y));
    }
}

void CustomElementCreatorWindow::on_save_custom_dialog(){
    save_dialog->set_visible(false);

    // check to see if the element is valid, esp if the name is valid and unique

    current_editor_template.chain_points = points;
//    current_editor_template.type = ELEMENT_TYPES::STATIC_OBSTACLE;
    current_editor_template.template_name = save_dialog->new_name->get_text();

    DriftingRealmsXMLWriter::write_element_template_to_xml_file(current_editor_template);
}

void CustomElementCreatorWindow::on_open_save_custom_dialog(){
    save_dialog->set_visible(true);
}

void CustomElementCreatorWindow::open_file_opener(){
    my_file_getter->set_visible(true);
}

void CustomElementCreatorWindow::on_close(){
    this->set_visible(false);
    is_closed = true;
    //++current_point_index;
}

void CustomElementCreatorWindow::draw_element_status(Canvas &c, const Rect &clip){
    c.fill_rect(Rectf(0,0,status_area->get_width(),status_area->get_height()),Colorf(0.4f,0.4f,0.4f,1.0f));
}

void CustomElementCreatorWindow::draw_element(Canvas &c, const Rect &clip){
    c.fill_rect(Rectf(0,0,creation_area->get_width(),creation_area->get_height()),Colorf(0.2f,0.2f,0.2f,1.0f));

    if(base_bitmap){
        int draw_width  = creation_area->get_width();
        int draw_height = creation_area->get_height();

        if(draw_width > base_bitmap->get_width() ){
            draw_width = base_bitmap->get_width();
        }

        if(draw_height > base_bitmap->get_height()){
            draw_height = base_bitmap->get_height();
        }

        float dest_width = draw_width;
        float dest_height = draw_height;

        float source_width = draw_width;
        float source_height = draw_height;

        dest_height *= zoom;
        dest_width *= zoom;

        base_bitmap->draw(c,
            Rectf(0,0,source_width,source_height),
            Rectf(-scroll_cam_x,-scroll_cam_y,dest_width-scroll_cam_x,dest_height-scroll_cam_y)
        );
    }

    /// draw the outline points
    for(unsigned int i=0; i<points.size(); ++i){
        c.fill_circle((points[i]-Vec2f(scroll_cam_x,scroll_cam_y))*zoom,3,Colorf(0.0f,0.0f,1.0f,1.0f));
    }

    for(unsigned int i=1; i<points.size(); ++i){
        c.draw_line((points[i]-Vec2f(scroll_cam_x,scroll_cam_y))*zoom,(points[i-1]-Vec2f(scroll_cam_x,scroll_cam_y))*zoom,Colorf(0.0f,1.0f,1.0f,1.0f));
    }
}

void CustomElementCreatorWindow::draw_element_control(Canvas &c, const Rect &clip){
    c.fill_rect(
        Rectf(0,0,control_area->get_width(),control_area->get_height()),
        clan::Gradient(
                Colorf(0.0f,0.0f,0.0f,1.0f),
                Colorf(0.0f,0.0f,0.0f,1.0f),
                Colorf(0.0f,1.0f,0.0f,1.0f),
                Colorf(0.0f,1.0f,0.0f,1.0f)
        )
    );
}


/// Does not delete up that previous image!

void CustomElementCreatorWindow::LoadBitMap(bool v){

    in_point_edit_mode = false;

    if(!v){
        if(my_file_getter->file_to_return != ""){
            clan::Canvas mycan = (this->get_canvas());

            base_bitmap = new clan::Image(mycan ,my_file_getter->file_to_return );

            current_editor_template.texture_name = my_file_getter->file_to_return;

            my_file_getter->file_to_return = "";
        }
    }
}




void CustomElementCreatorWindow::on_enter_point_mode(){
    in_point_edit_mode = true;
    in_point_remove_mode = false;

    if(points.empty()){
        current_point_index=0;
    }
}


void CustomElementCreatorWindow::on_enter_point_delete_mode(){
    in_point_edit_mode = false;
    in_point_remove_mode = true;
}



void CustomElementCreatorWindow::on_close_current_points(){

    in_point_edit_mode = false;
    points.push_back(points[0]);
    this->request_repaint();
}

bool CustomElementCreatorWindow::on_creation_area_input(const clan::InputEvent &e){
    static Vec2f scroll_last = Vec2f(0,0);
    static bool scrolling = false;

    if(e.id == InputCode::mouse_middle && IsPressed(e)){
        scroll_last = Vec2f(e.mouse_pos);
        scrolling = true;
    }
    if(e.id == InputCode::mouse_middle && e.type == InputEvent::Type::released){
        scroll_last = Vec2f(e.mouse_pos);
        scrolling = false;
    }


    if(e.pointer_moved){
        x_pos_label->set_text("mouse x: "+StringHelp::int_to_text(e.mouse_pos.x));
        y_pos_label->set_text("mouse y: "+StringHelp::int_to_text(e.mouse_pos.y));


        if(scrolling){
            scroll_cam_x -= (e.mouse_pos.x-scroll_last.x)*zoom;
            scroll_cam_y -= (e.mouse_pos.y-scroll_last.y)*zoom;
            scroll_last = Vec2f(e.mouse_pos);
        }
    }

    if(e.id == clan::mouse_wheel_up && zoom < 20){
        zoom += .2;
    }else if(e.id == clan::mouse_wheel_down && zoom > 1){
        zoom -= .2;
    }
    if(zoom > 20) zoom = 20;
    if(zoom < 1) zoom = 1;


    /// There is a bug that creates doubles points if the pointer leaves the area.
    if(in_point_edit_mode){

//        if(points.size() == current_point_index){

            if( (e.id == clan::mouse_left) && IsPressed(e) ){
                    std::cout << "test\n";
                points.push_back(Vec2f(e.mouse_pos.x/zoom+scroll_cam_x,e.mouse_pos.y/zoom+scroll_cam_y));
                moving_current_point = true;
                ++current_point_index;
            }

            else if(e.id == clan::mouse_right ){
                moving_current_point=false;
            }
//        }

        if(moving_current_point){
            if(e.type == InputEvent::pointer_moved){

                if(points.size()>1){
                    if(e.shift){
                        points.back().x = e.mouse_pos.x/zoom + scroll_cam_x;
                        points.back().y = points[points.size()-2].y;
                    }else if(e.ctrl){
                        points.back().x = points[points.size()-2].x;
                        points.back().y = e.mouse_pos.y/zoom + scroll_cam_y;
                    }else{
                        points.back().x = e.mouse_pos.x/zoom + scroll_cam_x;
                        points.back().y = e.mouse_pos.y/zoom + scroll_cam_y;
                    }
                }
                else{
                    if(points.size()){
                        points.back().x = e.mouse_pos.x/zoom + scroll_cam_x;
                        points.back().y = e.mouse_pos.y/zoom + scroll_cam_y;
                    }
                }
            }
        }

        this->request_repaint();
    }

    else if(in_point_remove_mode){
        if(this->get_display_window().get_ic().get_mouse().get_keycode(clan::mouse_left)){
            for(unsigned int i=0; i<points.size(); ++i){
                if(Point(Vec2f(e.mouse_pos)/zoom + Vec2f(Point(scroll_cam_x,scroll_cam_y))).distance(points[i]) < 3/zoom){
                    points.erase( points.begin() + i );
                    --current_point_index;
                }
            }
        }
        this->request_repaint();
    }
    return false;
}


bool CustomElementCreatorWindow::on_input(const clan::InputEvent &e){
    return 0;
}
