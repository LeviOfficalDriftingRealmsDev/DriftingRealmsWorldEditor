#pragma once

#include "Precomp.hpp"

class FileLabel : public clan::Label{
    public:
        FileLabel(GUIComponent *parent, int pos_index, const std::string file_name, bool dir);
    public:
        std::string file_name;
        std::string dir_str;
        Colorf current_background;
        Colorf current_foreground;
        bool is_dir;
    private:
        bool on_hover();
        bool on_leave();
        void on_draw(clan::Canvas &c, const Rect &r);
};
