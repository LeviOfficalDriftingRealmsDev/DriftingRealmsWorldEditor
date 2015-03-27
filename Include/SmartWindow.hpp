#pragma once

#include "Precomp.hpp"

class SmartWindow : public clan::Window{
    public:
        SmartWindow(clan::GUIComponent* p);
    private:
        clan::GUIComponent *parent;
        bool dockable;
        bool tabable;
        bool contain_in_parent;
        Vec2f old_window_pos;
    private:
        bool on_input_released(const clan::InputEvent &e);
        /// this is a hack... there is no method to simply push the window to the top of the
        /// draw stack, so it needs to be removed and replaced to put in on top.
        bool on_input_pressed(const clan::InputEvent &e);
        bool close_this_win();
};
