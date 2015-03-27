#pragma once

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include "./GameDisplayWindow.hpp"
#include "../TrollGameEngine/TrollMath/TrollMath.hpp"

using namespace clan;

///! This will get moved to a differnt class and will become the game launcher.
class GameInstance{
    public:
        static int main(const std::vector<std::string> &args);
    private:
};
