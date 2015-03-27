#include "SmartWindow.hpp"


SmartWindow::SmartWindow(clan::GUIComponent* p) : clan::Window(p){
    parent=p;
    old_window_pos = this->get_geometry().get_top_left();
    this->func_input_released().set(this,&SmartWindow::on_input_released);
    this->func_input_pressed().set(this,&SmartWindow::on_input_pressed);
    this->func_close().set(this,&SmartWindow::close_this_win);

    if(this->get_parent_component() != this->get_top_level_component()){
        set_draggable(false);
    }
}

bool SmartWindow::on_input_released(const clan::InputEvent &e){
    if(e.type == e.released && e.id == clan::InputCode::mouse_left){

        if(old_window_pos != this->get_geometry().get_top_left()){

            old_window_pos = this->get_geometry().get_top_left();

            if( this->get_geometry().get_top_left().x < 0 ){
                int fix = -this->get_geometry().get_top_left().x+1;
                this->set_geometry(this->get_geometry().translate(fix,0));
            }

            if( this->get_geometry().get_top_right().x > this->get_display_window().get_viewport().get_width() ){
                int fix = this->get_display_window().get_viewport().get_width()-this->get_geometry().get_top_right().x;
                this->set_geometry(this->get_geometry().translate(fix,0));
            }

            if( this->get_geometry().get_top_left().y < 0 ){
                int fix = -this->get_geometry().get_top_left().y+1;
                this->set_geometry(this->get_geometry().translate(0,fix));
            }

            if( this->get_geometry().get_bottom_left().y > this->get_display_window().get_viewport().get_height()){
                int fix = this->get_display_window().get_viewport().get_height()-this->get_geometry().get_bottom_right().y;
                this->set_geometry(this->get_geometry().translate(0,fix));
            }
        }
    }
    return 0;
}

bool SmartWindow::on_input_pressed(const clan::InputEvent &e){
    this->set_parent_component(this);
    this->set_parent_component(parent);
    this->request_repaint();
    return 0;
}

bool SmartWindow::close_this_win(){
    set_visible(false);
    return 0;
}
