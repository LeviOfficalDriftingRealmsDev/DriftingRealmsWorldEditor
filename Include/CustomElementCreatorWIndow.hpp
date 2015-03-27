#pragma once

#include "Precomp.hpp"
#include "GlobalVars.hpp"
#include "World.hpp"
#include "XMLReader.hpp"
#include "SmartWindow.hpp"
#include "FileOpenDialogWindow.hpp"
#include "SimpleOneLineDialogWindow.hpp"
#include "ToolBar.hpp"


//class CustomElementCreatorWindow : public SmartWindow{
class CustomElementCreatorWindow : public GUIComponent{
    public:
        CustomElementCreatorWindow(clan::GUIComponent *parent, Rectf f);
        ~CustomElementCreatorWindow(){
            delete my_file_getter;
            delete quality_combo_box;
        }
        bool is_closed = true;
    private:
        void on_close();
        void draw_element(Canvas &c, const Rect &clip);
        void draw_element_control(Canvas &c, const Rect &clip);
        void draw_element_status(Canvas &c, const Rect &clip);
        clan::GUIComponent *creation_area;
        clan::GUIComponent *control_area;
        clan::GUIComponent *status_area;
        void LoadBitMap(bool v);

        void on_save_custom_dialog();
        void on_open_save_custom_dialog();

    private:
        void on_enter_point_mode();
        void on_enter_point_delete_mode();
        void on_close_current_points();
        void open_file_opener();
    private:
        clan::PushButton *add_new_bitmap_button;
        clan::PushButton *remove_bitmap_button;
        clan::PushButton *add_collision_points_buttons;
        clan::PushButton *close_collision_points_buttons;
        clan::PushButton *move_collision_points_button;
        clan::PushButton *remove_collision_points_buttons;
        clan::Spin *tolerance_spin;
        clan::Label *tolerance_label;
        clan::PopupMenu quality_menu;
        clan::ComboBox *quality_combo_box;
        clan::PushButton *generate_button;
        clan::PushButton *save_button;
        clan::PushButton *close;


        clan::Label *x_pos_label;
        clan::Label *y_pos_label;



//        CustomOpenFileDialog *my_file_getter;

//        SaveElementDialog * save_dialog;

        clan::Image *base_bitmap;

        OpenFileDialogWindow *my_file_getter;
        SimpleOneLineDialogWindow * save_dialog;

        int scroll_cam_x;
        int scroll_cam_y;
        float zoom = 1;
    private:
        bool on_creation_area_input(const InputEvent &e);
        bool in_point_edit_mode, in_point_remove_mode;
        bool moving_current_point;
        int current_point_index;
        std::vector<Vec2f> points;

        ElementTemplate current_editor_template;

        bool on_input(const clan::InputEvent &e);
        void generate_collision();
};
