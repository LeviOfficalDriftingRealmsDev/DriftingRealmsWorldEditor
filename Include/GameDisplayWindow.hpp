#pragma once

#include "Precomp.hpp"
#include "World.hpp"
#include "ElementBrowser.hpp"
#include "CustomElementCreatorWIndow.hpp"
#include "../TrollGameEngine/TrollFileSystem/FileHelp.hpp"
#include "TriggerEditor.hpp"
struct ResourceLoader{
    ResourceLoader();
    std::vector<ElementTemplate*> p_element_templates;
    void LoadDefaultElements(clan::Canvas &c);
};



class GameDisplayWindow{
    public:
        GameDisplayWindow(clan::GUIManager *manager, DisplayWindow *p_dw);
        ~GameDisplayWindow();
    private:
        void Loop();
        void render_root_comp(clan::Canvas &c, const clan::Rect &clip_rect);
        bool on_close();
        bool visible = true;
        float zoom = 1;
        bool NavMeshEditMode = false;
        bool AngleEditMode = false;
    private:
        CustomElementCreatorWindow *custom_element_creator;
        TriggerEditor *trigger_editor;

        //! map size
        Rectf map_rect = Rectf(0,0,0,0);
    private:
        clan::Canvas *root_canvas;
        clan::GUIManager *gui_manager;
        DisplayWindow *display_window;
        clan::MenuBar *main_menu_bar;
        clan::GUIComponent *root_comp;
        clan::PushButton *make_world_button;
        clan::LineEdit *new_world_height_field, *new_world_width_field;
        Vec2f cam_position;
        bool run_game;
        ::ToolBar *tool_bar;
        void on_resize(int x, int y);
    private:
        inline void build_window_tool_bar();
    private:
        bool Selection(const clan::InputEvent &e);
        void UpdateCollisionsFromZoom();
        void action_open_tool_box();
        void action_open_components_box();
        void action_open_world_builder();
        void action_open_game_elements_tool_box();
        void action_open_element_builder();
        void action_open_triggers_box();
        void action_open_navigation_map_tool();
        void action_open_navigation_map_angle_tool();
        void action_mirror_map_x();
        void action_mirror_map_y();

        void action_toggle_draw_path_data();
        void action_toggle_static_cursor();
        void action_open_trigger_editor();
        void action_generate_nav_map_start();
        void Export();
        void Import();
    private:
        void make_a_new_world();

        clan::Image *StaticCursor;
        clan::Vec2f *StaticCursorPosition;
        clan::Vec2f CursorPosition;

        bool on_input(const clan::InputEvent &e);
        bool is_cursor_over_gui();

        clan::InputDevice *keyboard;

        void on_key(const clan::InputEvent &e);

        bool drawing_path_data;
        bool draw_static_cursor;

        clan::Font debug_font;
};

