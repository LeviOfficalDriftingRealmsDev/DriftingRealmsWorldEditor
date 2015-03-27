#include "./TabHeader.hpp"
//::Tab::Tab(::TabHeader *owner, Rect f, std::string name) : GUIComponent ((GUIComponent *)(owner), GUITopLevelDescription(f,true), name){
::Tab::Tab(::TabHeader *owner, Rect f, std::string name) : GUIComponent ((GUIComponent *)(owner)){
        set_geometry(f);
        button = new clan::PushButton(this);

        this->name = name;

        label = new clan::Label(this);
        label->set_text(name);

        if(label->get_preferred_content_width()+40 > get_geometry().right-4){
            label->set_text("");
        }

        clan::Canvas *canvas = new clan::Canvas(owner->get_canvas());
        image = new Image(*canvas, "./GUIResources/ToolBar/component_selector.png",ImageImportDescription());

        button->set_icon(*image);

        func_render().set(this,&::Tab::on_render);
        func_input().set(this,&Tab::on_input);
        func_resized().set(this,&::Tab::on_resize);

        parent = owner;
        this->f = f;
        set_focus_policy(focus_refuse);
        button->set_focus_policy(focus_refuse);
}


bool ::Tab::on_input(const clan::InputEvent &e){
    if(IsPressed(e)){
        if(e.id == mouse_middle)
            dead = true;
        else if(e.id == mouse_left)
            parent->TabClicked(id);
//        else
//            parent->on_input(e);
    }
}
