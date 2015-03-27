#pragma once

#include "Precomp.hpp"
#include "SmartWindow.hpp"
#include "FileOpenDialogFileLabel.hpp"

class OpenFileDialogWindow : public SmartWindow{
    public:
        OpenFileDialogWindow(clan::GUIComponent *parent);
        std::string file_to_return;

    private:
        void on_go_to_parent_dir();
        void populate_menu(const std::string &dir);
        bool on_click(const clan::InputEvent &e);
        void LoadImage();

    private:
        std::string current_string;
        std::string current_dir;
        std::string parent_dir;
        std::string last_dir;
        std::vector<FileLabel*> file_labels;
        clan::GUIComponent *file_box;
        DirectoryListing dir_l;
        clan::FileSystem *fs;
        clan::PushButton *go_to_parent_dir;
        clan::PushButton *only_show_images;
};
