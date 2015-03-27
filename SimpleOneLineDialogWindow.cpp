#include "SimpleOneLineDialogWindow.hpp"

SimpleOneLineDialogWindow::SimpleOneLineDialogWindow(clan::GUIComponent *parent) : SmartWindow(parent){
    this->set_geometry(Rectf(0,0,200,120));
    this->set_title("New Element Name");
    this->func_close().set(this,&SimpleOneLineDialogWindow::on_close);
    this->func_visibility_change().set(this,&SimpleOneLineDialogWindow::on_open);

    int px = 10;
    int py = 35;

    new_name = new clan::LineEdit(this);
    new_name->set_geometry(Rectf(px,py,px+180,py+25));

    px = 10;
    py = 75;

    set_new_name_button = new clan::PushButton(this);
    set_new_name_button->set_geometry(Rectf(px,py,px+180,py+25));
    set_new_name_button->set_text("Save New Element");
}

bool SimpleOneLineDialogWindow::on_close(){
    this->set_visible(false);
    return 0;
}

void SimpleOneLineDialogWindow::on_open(bool v){
    ;
}
