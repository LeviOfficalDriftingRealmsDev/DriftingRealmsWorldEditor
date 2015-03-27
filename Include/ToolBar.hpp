#pragma once
#include "Precomp.hpp"
#include "ElementBrowser.hpp"
#include "TabHeader.hpp"
#include "TriggerTool.hpp"
#include "GameElementsTool.hpp"
#include "NavigationMeshTool.hpp"


class ToolBar : public clan::GUIComponent{
public:
    ToolBar(GUIComponent *owner, Rect f, clan::Callback_1<bool, const clan::InputEvent &> *call_back) : GUIComponent (owner){
        this->set_geometry(f);
        func_render().set(this,&ToolBar::on_render);
//        header = new ::TabHeader(this,Rect(f.left+8,get_geometry().top,f.right-2,get_geometry().top+35));
        rect = f;
        header = new ::TabHeader(this,Rect(6,0,rect.get_width()-5,35));
        func_input().set(this,&ToolBar::on_input);
        parent = owner;
        this->call_back = call_back;
        set_focus_policy(focus_refuse);
    }
    ~ToolBar(){
    }
    void add_game_elements_tool(Vec2f *vec){
        header->AddTab("game elements", new GameElementsTool(this,Rectf(6,42,rect.get_width()-4,rect.get_height()-46),vec));
    }
    void add_trigger_builder(Vec2f *vec){
//        header->AddTab("triggers builder", new TriggerToolBrowser(this,Rectf(rect.left + 6,rect.top + 42,rect.right-4,rect.bottom-46),vec,call_back));
        header->AddTab("triggers builder", new TriggerToolBrowser(this,Rectf(6,42,rect.get_width()-4,rect.get_height()-46),vec,call_back));
    }
    void add_component_builder(Vec2f *vec){
//        header->AddTab("component builder", new ElementBrowser(this,Rectf(rect.left + 6,rect.top + 42,rect.right-4,rect.bottom-46),vec,call_back));
        header->AddTab("component builder", new ElementBrowser(this,Rectf(6,42,rect.get_width()-4,rect.get_height()-46),vec,call_back));
    }
    void add_navigation_mesh_tool(Vec2f *vec){
        header->AddTab("navigation", new NavigationMeshTool(this,Rectf(6,42,rect.get_width()-4,rect.get_height()-46),vec));
    }
    void on_render(Canvas &canvas, const Rect &clip_rect)
    {
        const int width = rect.get_width();
        const int height = rect.get_height();

        canvas.fill_rect(Rect(0,0,width,height),Colorf(20,20,20,255));
        canvas.fill_rect(Rect(0,0,1,height),GLOBAL_VARS::green);
        canvas.fill_rect(Rect(4,0,5,height),GLOBAL_VARS::green);

        canvas.fill_rect(Rect(5,37,width,38),GLOBAL_VARS::green);
        canvas.fill_rect(Rect(5,40,width,41),GLOBAL_VARS::green);
        header->request_repaint();

        //! the header may need to tell the tool bar to expand if it needs more room
        if(header->expand_by != 0){
            rect = Rectf(rect.left - header->expand_by,rect.top,rect.right,rect.bottom);
            set_geometry(rect);
//            header->resize(Rect(rect.left+6,10,rect.get_width()-7,10+35));
            header->resize(Rect(6,0,rect.get_width()-5,35),rect.get_height()-46);
            header->expand_by = 0;
        }
    }
    void resize(Rect f){
        rect = f;
        set_geometry(f);
    }
    bool on_input(const clan::InputEvent &e){
        static bool is_dragging = false;
        if(IsPressed(e) && e.id == InputCode::mouse_left && e.mouse_pos.x < 8){
            is_dragging = true;
            capture_mouse(true);
        }else if(e.type == InputEvent::Type::released && e.id == InputCode::mouse_left){
            is_dragging = false;
            capture_mouse(false);
        }else{
//            call_back->invoke(e);
        }
        if(is_dragging){
            rect = Rectf(rect.left + e.mouse_pos.x - 4,rect.top,rect.right,rect.bottom);
            set_geometry(rect);
//            header->resize(Rect(rect.left+6,10,rect.get_width()-7,10+35));
            header->resize(Rect(6,0,rect.get_width()-5,35),rect.get_height()-46);
        }
        return true;
    }
    ::TabHeader *header;
    Rect rect;
    clan::GUIComponent *parent;
    Callback_1<bool, const clan::InputEvent &> *call_back;
};
