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


class GameElementsTool : public GUIComponent{
    public:
//        NavigationMeshTool(clan::GUIComponent *parent, Rect f, clan::Vec2f *static_cursor_pos, clan::Callback_1<bool, const clan::InputEvent &> *call_back) : GUIComponent(parent, GUITopLevelDescription(f,true), ""){
        GameElementsTool(clan::GUIComponent *parent, Rect f, clan::Vec2f *static_cursor_pos) : GUIComponent(parent){
            set_geometry(f);
            clan::Canvas *canvas = new clan::Canvas(parent->get_canvas());
            image = new Image(*canvas, "./GameResources/Resources/red_diamond_icon.png",ImageImportDescription());

            const int border = 5;

            StaticCursorPosition = static_cursor_pos;

            add_red_resource = new clan::PushButton(this);
            add_red_resource->set_geometry(Rectf(0,0,f.get_width()/2-border,f.get_width()/2-border));
            add_red_resource->set_icon(*image);
            add_red_resource->func_clicked().set(this,&GameElementsTool::AddRed);
            add_red_resource->set_focus_policy(focus_refuse);


            image = new Image(*canvas, "./GameResources/Resources/purple_diamond_icon.png",ImageImportDescription());
            add_purple_resource = new clan::PushButton(this);
            add_purple_resource->set_geometry(Rectf(f.get_width()/2+border,0,f.get_width(),f.get_width()/2-border));
            add_purple_resource->set_icon(*image);
            add_purple_resource->func_clicked().set(this,&GameElementsTool::AddPurple);
            add_purple_resource->set_focus_policy(focus_refuse);

            image = new Image(*canvas, "./GameResources/Resources/blue_diamond_icon.png",ImageImportDescription());
            add_blue_resource = new clan::PushButton(this);
            add_blue_resource->set_geometry(Rectf(0,f.get_width()/2+border,f.get_width()/2-border,f.get_width()));
            add_blue_resource->set_icon(*image);
            add_blue_resource->func_clicked().set(this,&GameElementsTool::AddBlue);
            add_blue_resource->set_focus_policy(focus_refuse);

            add_start_point = new clan::PushButton(this);
            add_start_point->set_geometry(Rectf(f.get_width()/2+border,f.get_width()/2+border,f.get_width(),f.get_width()));
            add_start_point->set_icon(*image);
            add_start_point->set_focus_policy(focus_refuse);

            func_resized().set(this,&GameElementsTool::on_resize);
        }
        ~GameElementsTool(){
            delete image;
        }
        void AddRed(){
            GLOBAL_VARS::element_list.push_back(new GameElement(*StaticCursorPosition,GAME_TYPES::RESOURCE_RED));
        }
        void AddPurple(){
            GLOBAL_VARS::element_list.push_back(new GameElement(*StaticCursorPosition,GAME_TYPES::RESOURCE_PURPLE));
        }
        void AddBlue(){
            GLOBAL_VARS::element_list.push_back(new GameElement(*StaticCursorPosition,GAME_TYPES::RESOURCE_BLUE));
        }

        void on_resize(){
            const int border = 5;
            add_red_resource->set_geometry(Rectf(0,0,get_geometry().get_width()/2-border,get_geometry().get_width()/2-border));
            add_purple_resource->set_geometry(Rectf(get_geometry().get_width()/2+border,0,get_geometry().get_width(),get_geometry().get_width()/2-border));
            add_blue_resource->set_geometry(Rectf(0,get_geometry().get_width()/2+border,get_geometry().get_width()/2-border,get_geometry().get_width()));
            add_start_point->set_geometry(Rectf(get_geometry().get_width()/2+border,get_geometry().get_width()/2+border,get_geometry().get_width(),get_geometry().get_width()));
        }

    private:
        Vec2f *StaticCursorPosition;

    /// Child components
    private:
        clan::PushButton *add_red_resource;
        clan::PushButton *add_purple_resource;
        clan::PushButton *add_blue_resource;
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

