#include "ElementListItem.hpp"

ElementListItem::ElementListItem(GUIComponent *parent, int new_pos, ElementTemplate *new_template) : clan::GUIComponent(parent){

    list_template = new_template;

    delete_me = false;

    is_selected = false;

    stack_position =  new_pos;

    const int new_height = 60;

    int new_x = 0;
    int new_y = (new_pos * new_height);

    this->func_render().set(this,&ElementListItem::draw_box_bg);
    this->func_pointer_enter().set(this,&ElementListItem::on_hover);
    this->func_pointer_exit().set(this,&ElementListItem::on_leave);

    this->set_geometry(Rectf(new_x,new_y,new_x+parent->get_width(),new_y+new_height));

    std::string msg = "";
    msg += "Name: ";
    msg += new_template->template_name;
    msg += "\n";
    msg += "Textures: 1\n"; // fix
    msg += ("Points: " + clan::StringHelp::int_to_text(new_template->chain_points.size()));

    const std::string path = new_template->texture_name;

    new_x = 60;
    new_y = -20;

    line_text = new clan::Label(this);
    line_text->set_geometry(Rectf(new_x,new_y,new_x+100,new_y+60));
    line_text->set_text(msg);

    new_x = this->get_width()-20;
    new_y = 4;

    remove_element_button = new clan::PushButton(this);
    remove_element_button->set_geometry(Rectf(new_x,new_y,new_x+16,new_y+16));
    remove_element_button->set_text("X");
    remove_element_button->func_clicked().set(this,&ElementListItem::rm_self);
    remove_element_button->set_focus_policy(focus_refuse);

    Canvas can = Canvas(get_top_level_component()->get_canvas());
    icon_image = new Image(can,path);

    current_bg_color = Colorf(0.0f,0.0f,0.0f,1.0f);
    current_fg_color = Colorf(0.1f,0.1f,0.1f,1.0f);
}

void ElementListItem::resize(){
    const int new_height = 60;
    int new_x = 0;
    int new_y = (stack_position * new_height);

    this->set_geometry(Rectf(new_x,new_y,new_x+get_parent_component()->get_width(),new_y+new_height));

    new_x = 60;
    new_y = -20;

    line_text->set_geometry(Rectf(new_x,new_y,new_x+100,new_y+60));

    new_x = this->get_width()-20;
    new_y = 4;

    remove_element_button->set_geometry(Rectf(new_x,new_y,new_x+16,new_y+16));
}

ElementListItem::~ElementListItem(){

    this->delete_child_components();

//    delete list_template;
//    delete line_text;
}

void ElementListItem::rm_self(){
    delete_me=true;
}

void ElementListItem::draw_box_bg(Canvas &c,const Rect &clip){

    c.fill_rect(0,0,this->get_width(),this->get_height(),current_bg_color);

    c.draw_line(Vec2f(0,this->get_height()),Vec2f(this->get_width(),this->get_height()),Colorf(0.0f,1.0f,0.0f,1.0f));

    icon_image->draw(c,Rectf(2,2,56,56));
}

bool ElementListItem::on_hover(){
    if(!is_selected){
        current_bg_color = Colorf(0.1f,0.1f,0.1f,1.0f);
    }
    return 0;
}

bool ElementListItem::on_leave(){
    if(!is_selected){
        current_fg_color = Colorf(0.0f,1.0f,0.0f,1.0f);
        current_bg_color = Colorf(0.0f,0.0f,0.0f,1.0f);
        line_text->set_text_color(current_fg_color);
    }
    return 0;
}

bool ElementListItem::set_selected(bool s){

    is_selected = s;

    if(s){
        current_fg_color = Colorf(0.0f,0.0f,0.0f,1.0f);
        current_bg_color = Colorf(0.4f,0.4f,0.4f,1.0f);
    }else{
        current_fg_color = Colorf(0.0f,1.0f,0.0f,1.0f);
        current_bg_color = Colorf(0.0f,0.0f,0.0f,1.0f);
    }

    line_text->set_text_color(current_fg_color);
    return 0;
}

