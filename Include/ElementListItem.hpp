#pragma once

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include "GlobalVars.hpp"
#include "World.hpp"
#include "XMLReader.hpp"

using namespace clan;

class ElementListItem : public clan::GUIComponent{
    public:
        ElementListItem(GUIComponent *parent, int new_pos, ElementTemplate *new_template);
        ~ElementListItem();
        bool set_selected(bool s);

    public:
        ElementTemplate *list_template;
        bool delete_me;
        void resize();

    private:
        bool is_selected;
        int stack_position;
        Colorf current_bg_color;
        Colorf current_fg_color;
        Image *icon_image;

    /// Child Components
    private:
        clan::Label *line_text;
        clan::PushButton *remove_element_button;

    private:
        // should be changed into a callback
        void rm_self();

    /// Callbacks
    private:
        void draw_box_bg(Canvas &c,const Rect &clip);
        bool on_hover();
        bool on_leave();


};
