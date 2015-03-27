#pragma once

#include "Precomp.hpp"

#include "GlobalVars.hpp"
#include "World.hpp"

using namespace clan;

class TriggerToolBrowser : public GUIComponent{
    public:
//        TriggerToolBrowser(clan::GUIComponent *parent, Rect f, clan::Vec2f *static_cursor_pos, clan::Callback_1<bool, const clan::InputEvent &> *call_back) : GUIComponent(parent, GUITopLevelDescription(f,true), ""){
        TriggerToolBrowser(clan::GUIComponent *parent, Rect f, clan::Vec2f *static_cursor_pos, clan::Callback_1<bool, const clan::InputEvent &> *call_back) : GUIComponent(parent){
            set_geometry(f);
            clan::Canvas *canvas = new clan::Canvas(parent->get_canvas());
            image = new Image(*canvas, "./GUIResources/ToolBar/Region.png",ImageImportDescription());

            const int border = 5;

            StaticCursorPosition = static_cursor_pos;

            std::cout << f << "\n";

            add_area = new clan::PushButton(this);
            add_area->set_geometry(Rectf(0,0,f.get_width()/2-border,f.get_width()/2-border));
            add_area->set_icon(*image);
            add_area->func_clicked().set(this,&TriggerToolBrowser::AddRegion);
            add_area->set_focus_policy(focus_refuse);


            image = new Image(*canvas, "./GUIResources/ToolBar/Point.png",ImageImportDescription());
            add_point = new clan::PushButton(this);
            add_point->set_geometry(Rectf(f.get_width()/2+border,0,f.get_width(),f.get_width()/2-border));
            add_point->set_icon(*image);
            add_point->set_focus_policy(focus_refuse);

            add_camera_point = new clan::PushButton(this);
            add_camera_point->set_geometry(Rectf(0,f.get_width()/2+border,f.get_width()/2-border,f.get_width()));
            add_camera_point->set_icon(*image);
            add_camera_point->set_focus_policy(focus_refuse);

            add_start_point = new clan::PushButton(this);
            add_start_point->set_geometry(Rectf(f.get_width()/2+border,f.get_width()/2+border,f.get_width(),f.get_width()));
            add_start_point->set_icon(*image);
            add_start_point->set_focus_policy(focus_refuse);

            func_resized().set(this,&TriggerToolBrowser::on_resize);
        }
        ~TriggerToolBrowser(){
            delete image;
        }
        void AddRegion(){
            GLOBAL_VARS::element_list.push_back(new AreaElement(Vec2f(StaticCursorPosition->x,StaticCursorPosition->y)));
        }

        void on_resize(){
            const int border = 5;
            add_area->set_geometry(Rectf(0,0,get_geometry().get_width()/2-border,get_geometry().get_width()/2-border));
            add_point->set_geometry(Rectf(get_geometry().get_width()/2+border,0,get_geometry().get_width(),get_geometry().get_width()/2-border));
            add_camera_point->set_geometry(Rectf(0,get_geometry().get_width()/2+border,get_geometry().get_width()/2-border,get_geometry().get_width()));
            add_start_point->set_geometry(Rectf(get_geometry().get_width()/2+border,get_geometry().get_width()/2+border,get_geometry().get_width(),get_geometry().get_width()));
        }

    private:
        Vec2f *StaticCursorPosition;

    /// Child components
    private:
        clan::PushButton *add_area;
        clan::PushButton *add_point;
        clan::PushButton *add_camera_point;
        clan::PushButton *add_start_point;
        clan::Image *image;
    /// Callbacks
    private:
        bool on_close();
        bool on_input(const clan::InputEvent &e);
        void draw_box_bg(Canvas &c,const Rect &clip);
        void draw_comp_box_bg(Canvas &c,const Rect &clip);
        void on_reload_components_list();
        void add_element();
        clan::Callback_1<bool, const clan::InputEvent &> *call_back;
};
