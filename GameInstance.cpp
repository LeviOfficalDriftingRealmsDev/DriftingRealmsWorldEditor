#include "./Include/GameInstance.hpp"

int GameInstance::main(const std::vector<std::string> &args){

    SetupCore setup_core;
    SetupDisplay setup_display;
    SetupGL setup_gl;
/*
    clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Drifting Realms - World Editor");
	win_desc.set_size(clan::Size( 1280, 800 ), false);
	win_desc.set_fullscreen(false);
	win_desc.set_layered(true);
	win_desc.show_caption(false);

    DisplayWindow *display_window = new DisplayWindow(win_desc);

    GUIManager *gui_mngr = new GUIManager(*display_window,"GUIResources/GUIThemeAero");

    GameDisplayWindow *main_game_window = new GameDisplayWindow(gui_mngr,display_window);
*/



    clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Drifting Realms - World Editor");
	win_desc.set_size(clan::Size( 800*1.5, 600*1.5 ), false);
	win_desc.set_fullscreen(false);
	win_desc.set_layered(true);
	win_desc.show_caption(false);

    clan::DisplayWindow *display_window = new clan::DisplayWindow(win_desc);

    GUIManager *gui_mngr = new GUIManager(*display_window,"GUIResources/GUIThemeAero");

    GameDisplayWindow *main_game_window = new GameDisplayWindow(gui_mngr,display_window);

    delete display_window;
//    delete gui_mngr;
    delete main_game_window;
    return 0;
}
