#pragma once
#include "WorldElement.hpp"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "../TrollGameEngine/TrollMath/TrollMath.hpp"
#include "NavigationMesh.hpp"
#include "AngleEditor.hpp"
#include <algorithm>

using namespace clan;



enum TOOL_TIP_OFFSET{
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

enum TOOLS{
    MOVE,
    ROTATE,
    PLACE,
    ERASE
};

class NavigationMesh;

struct GLOBAL_VARS{
    static std::vector<ElementTemplate*> element_template_list;
    static std::vector<WorldElement*> element_list;
    static std::vector<Image*> image_list;
    static NavigationMesh *nav_map;
    static AngleEditor *angle_map;
    static clan::Font *dfont;

    static Colorf green;
};



void RemoveDoublesFromVectorInt(std::vector<int> &vec);


bool IsPressed(const clan::InputEvent &e);
