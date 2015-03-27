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


class ElementBrowser : public GUIComponent{
    public:
        ElementBrowser(clan::GUIComponent *parent, Rect f, clan::Vec2f *static_cursor_pos, clan::Callback_1<bool, const clan::InputEvent &> *call_back);

    ///
    private:
        Vec2f *StaticCursorPosition;
        std::vector<ElementListItem*> element_list;
        ElementListItem *currently_seleceted_item;

    /// Child components
    private:
        clan::PushButton *add_element_button, *refresh_elements_button;
        clan::CheckBox *center_check_box;
        clan::GUIComponent *SelectionButtonField, *ComponentListContainer;
        void on_resize();

        clan::PushButton *button_t_1;
        clan::PushButton *button_t_2;
        clan::PushButton *button_t_3;
        clan::PushButton *button_t_4;
        clan::PushButton *button_t_5;
        clan::ScrollBar *scroll_bar;
        clan::Label *center_label;
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
