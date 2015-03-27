#include "FileOpenDialogWindow.hpp"

OpenFileDialogWindow::OpenFileDialogWindow(clan::GUIComponent *parent) : SmartWindow(parent){
    this->set_title("Open A File...");
    this->set_visible(false);
    this->set_geometry(Rectf(0,0,300,200));

    file_box = new clan::GUIComponent(this);
    file_box->set_geometry(Rectf(8,54,get_width()-8,get_height()-8));
    file_box->set_clip_children();
    file_box->func_input().set(this,&OpenFileDialogWindow::on_click);

    int sx = 8;
    int sy = 28;

    go_to_parent_dir = new clan::PushButton(this);
    go_to_parent_dir->set_text("Parent Dir...");
    go_to_parent_dir->set_geometry(Rectf(sx,sy,sx+60,sy+20));
    go_to_parent_dir->func_clicked().set(this,&OpenFileDialogWindow::on_go_to_parent_dir);

    sx = 72;
    sy = 28;

    only_show_images = new clan::PushButton(this);
    only_show_images->set_text("Only Show Images");
    only_show_images->set_geometry(Rectf(sx,sy,sx+100,sy+20));

    fs = new clan::FileSystem("./");

    current_string = "";
    current_dir = "";
    parent_dir = "";

    last_dir = "";

    file_to_return="";

    populate_menu("GameResources");

}

void OpenFileDialogWindow::on_go_to_parent_dir(){
    populate_menu( ".." );
}

void OpenFileDialogWindow::populate_menu(const std::string &dir){

    std::string final_path;

    if(dir == ".."){

        final_path = current_string.substr(0,current_string.find_last_of("/"));

    }else{
        final_path = dir;
    }

    if(fs->has_directory(final_path) ){

        current_string = final_path;

        file_box->delete_child_components();
        file_labels.clear();

        dir_l = fs->get_directory_listing(current_string);

        unsigned int i=0;

        while(dir_l.next()){

            bool is_parent = (dir_l.get_filename() == "..");
            bool is_this = (dir_l.get_filename() == ".");

            if( !is_parent && !is_this ){
                file_labels.push_back(new FileLabel(file_box,i,dir_l.get_filename(), dir_l.is_directory()));
                ++i;
            }
        }
    }
    else{
        //clan::Console::write("problem!");
    }
}

bool OpenFileDialogWindow::on_click(const clan::InputEvent &e){
    for(unsigned int i=0; i<file_labels.size(); ++i){

        unsigned int propper_index = ( (e.mouse_pos.y/10));

        if(propper_index < file_labels.size()){

            if(e.type == clan::InputEvent::released){

                if(!file_labels[propper_index]->is_dir){

                    std::string current_file_ex = file_labels[propper_index]->file_name;

                    current_file_ex = current_file_ex.substr(current_file_ex.length()-4,current_file_ex.length());

                    if(current_file_ex == ".png" || current_file_ex == ".jpg"){

                        file_to_return = current_string + "/" + file_labels[propper_index]->file_name;
                        set_visible(false,true);
                    }

                    break;
                }else{
                    populate_menu( current_string + "/" + file_labels[propper_index]->file_name );
                    break;
                }
            }
        }
    }

    return 0;
}

void OpenFileDialogWindow::LoadImage(){
    ;
}

