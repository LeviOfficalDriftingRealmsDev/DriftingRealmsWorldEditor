#pragma once

#include "Precomp.hpp"
#include "SmartWindow.hpp"

class SimpleOneLineDialogWindow : public SmartWindow{
    public:
        SimpleOneLineDialogWindow(clan::GUIComponent *parent);
    public:
        clan::PushButton *set_new_name_button;
        clan::LineEdit *new_name;
    private:
        bool on_close();
        void on_open(bool v);
};
