#include "FileOpenDialogFileLabel.hpp"

FileLabel::FileLabel(GUIComponent *parent, int pos_index, const std::string file_name, bool dir) : clan::Label(parent){

    current_background = Colorf(0.0f,0.0f,0.0f,1.0f);
    current_foreground = Colorf(0.0f,1.0f,0.0f,1.0f);

    this->dir_str = "DIR | ";

    this->file_name = file_name;
    this->func_pointer_enter().set(this,&FileLabel::on_hover);
    this->func_pointer_exit().set(this,&FileLabel::on_leave);
    this->func_render().set(this,&FileLabel::on_draw);

    int sh = 10;
    int sw = 270;

    int sx = 0;
    int sy = pos_index*sh;

    is_dir = dir;

    set_geometry(Rectf(sx,sy,sx+sw,sy+sh));
    if(dir){
        set_text_color(current_foreground);
        set_text(dir_str+file_name);
    }else{
        set_text(file_name);
        current_foreground = Colorf(0.4f,0.4f,1.0f,1.0f);
        set_text_color(current_foreground);
    }
}

bool FileLabel::on_hover(){
    current_background = Colorf(0.0f,1.0f,0.0f,1.0f);
    current_foreground = Colorf(0.0f,0.0f,0.0f,1.0f);
    set_text_color(current_foreground);

    return 0;
}

bool FileLabel::on_leave(){
    current_background = Colorf(0.0f,0.0f,0.0f,1.0f);
    if(!is_dir){
        current_foreground = Colorf(0.4f,0.4f,1.0f,1.0f);
    }else{
        current_foreground = Colorf(0.0f,1.0f,0.0f,1.0f);
    }
    set_text_color(current_foreground);
    return 0;
}

void FileLabel::on_draw(clan::Canvas &c, const Rect &r){
    c.fill_rect(Pointf(0,0),Pointf(this->get_width(),this->get_height()),current_background);
    this->render_text(c,this->get_text());
}
