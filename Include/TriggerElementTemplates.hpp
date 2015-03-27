#pragma once
#include "Precomp.hpp"
enum TriggerTypes{
    Intiger = 0,
    Elements,
    LogicGates,
    Condition,
    Event,
    Filter,
    Region
};
Colorf ColorFromType(TriggerTypes loc);


class TriggerElementTemplate{
public:
    TriggerElementTemplate(){
        ;
    }
    TriggerElementTemplate(TriggerTypes *input_types,int input_cnt,TriggerTypes *output_types,int output_cnt,TriggerTypes type){
        this->input_types  = input_types ;
        this->output_types = output_types;
        this->type         = type        ;
        this->input_cnt    = input_cnt   ;
        this->output_cnt   = output_cnt  ;
        if(type == TriggerTypes::Filter){
            title = "Filter";
        }else{
            title = "unknown";
        }
    }
    std::string title; //! needed in the editor for display; can be used in game for debugging. (eg. (title here)error: can't do that)
    TriggerTypes type;
    TriggerTypes *input_types;
    TriggerTypes *output_types;
//    TriggerElementTemplate *inputs;
//    TriggerElementTemplate *outputs;
    int input_cnt;
    int output_cnt;
};
