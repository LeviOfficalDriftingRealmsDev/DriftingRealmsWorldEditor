#include "GlobalVars.hpp"

std::vector<ElementTemplate*> GLOBAL_VARS::element_template_list;
std::vector<WorldElement*> GLOBAL_VARS::element_list;
std::vector<Image*> GLOBAL_VARS::image_list;
NavigationMesh *GLOBAL_VARS::nav_map = new NavigationMesh();
AngleEditor *GLOBAL_VARS::angle_map = new AngleEditor();
clan::Font *GLOBAL_VARS::dfont = 0;

Colorf GLOBAL_VARS::green = Colorf(0.f,1.0f,0.f);

bool IsPressed(const clan::InputEvent &e){
    return e.type == InputEvent::Type::pressed || e.type == InputEvent::Type::doubleclick;
}



void RemoveDoublesFromVectorInt(std::vector<int> &vec){
    std::sort(vec.begin(),vec.end());
    vec.erase( unique( vec.begin(), vec.end() ), vec.end() );
}
