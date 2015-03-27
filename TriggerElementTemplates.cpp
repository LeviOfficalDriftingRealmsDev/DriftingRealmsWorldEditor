#include "TriggerElementTemplates.hpp"
Colorf ColorFromType(TriggerTypes loc){
    if(loc == TriggerTypes::Intiger)
        return Colorf::blue;
    else if(loc == TriggerTypes::Elements)
        return Colorf::green;
    else if(loc == TriggerTypes::LogicGates)
        return Colorf::purple;
    else if(loc == TriggerTypes::Condition)
        return Colorf::yellow;
    else if(loc == TriggerTypes::Event)
        return Colorf::red;
    else if(loc == TriggerTypes::Filter)
        return Colorf::orange;
    else
        return Colorf::grey;
}

