#pragma once

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include "GlobalVars.hpp"
#include "World.hpp"
#include "XMLReader.hpp"
#include "ElementListItem.hpp"
using namespace clan;


class NavigationMeshTool : public GUIComponent{
    public:
//        NavigationMeshTool(clan::GUIComponent *parent, Rect f, clan::Vec2f *static_cursor_pos, clan::Callback_1<bool, const clan::InputEvent &> *call_back) : GUIComponent(parent, GUITopLevelDescription(f,true), ""){
        NavigationMeshTool(clan::GUIComponent *parent, Rect f, clan::Vec2f *static_cursor_pos) : GUIComponent(parent){
            set_geometry(f);
            clan::Canvas *canvas = new clan::Canvas(parent->get_canvas());
            image = new Image(*canvas, "./GUIResources/ToolBar/Region.png",ImageImportDescription());

            const int border = 5;

            StaticCursorPosition = static_cursor_pos;

            std::cout << f << "\n";

            add_area = new clan::PushButton(this);
            add_area->set_geometry(Rectf(0,0,f.get_width()/2-border,f.get_width()/2-border));
            add_area->set_icon(*image);
//            add_area->func_clicked().set(this,&NavigationMeshTool::AddRegion);
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

            func_resized().set(this,&NavigationMeshTool::on_resize);
        }
        ~NavigationMeshTool(){
            delete image;
        }
//        void AddRegion(){
//            GLOBAL_VARS::nav_map->AddQuad(new Rect(StaticCursorPosition->x,StaticCursorPosition->y,StaticCursorPosition->x+100,StaticCursorPosition->y+100));
//        }

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
};

