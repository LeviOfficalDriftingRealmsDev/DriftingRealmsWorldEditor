#pragma once
#include "Precomp.hpp"
#include "TriggerElementTemplates.hpp"

using namespace clan;

class TriggerNode : public GUIComponent{
public:
    TriggerNode(clan::GUIComponent *parent,Vec2f position,Sizef size,clan::Font *font,TriggerElementTemplate *data) : GUIComponent(parent){
        set_geometry(Rectf(position.x,position.y,position.x+size.width,position.y+size.height));
        func_render().set(this,&TriggerNode::Draw);
        this->size = size;
        this->position = position;
        this->font = font;
        this->data = data;
        text_color = ColorFromType(data->type);
    }
    ~TriggerNode(){
    }
    void Draw(Canvas &c, const Rect &clip){

        const float radius = 20;
        //! ---------- Draw inputs ----------
        const int i_c = data->input_cnt;
        if(i_c % 2 == 0){//even
            for(int i = 0;i < i_c; i++){
                int neg = -1;
                if(i % 2 == 0) neg = 1;
                c.fill_circle(clip.left,
                          clip.top+(clip.get_height()/2 + (radius+5)/2) + (i+1)*(radius+5)*neg - 10*neg,
                          radius,ColorFromType(data->input_types[i]));
            }
        }else{//odd
            c.fill_circle(clip.left,
                      clip.top+(clip.get_height()/2),
                      radius,ColorFromType(data->input_types[0]));
            for(int i = 1;i < i_c; i++){
                int neg = -1;
                int offset = -25;
                if(i % 2 == 0){ neg = 1;offset = 0;}
                c.fill_circle(clip.left,
                          clip.top+(clip.get_height()/2) + i*(radius+5)*neg + offset,
                          radius,ColorFromType(data->input_types[i]));
            }
        }
        //! ---------- Draw inputs ----------

        //! ---------- Draw outputs ----------
        const int o_c = data->output_cnt;
        if(o_c % 2 == 0){//even
            for(int i = 0;i < o_c; i++){
                int neg = -1;
                if(i % 2 == 0) neg = 1;
                c.fill_circle(clip.right,
                          clip.top+(clip.get_height()/2 + (radius+5)/2) + (i+1)*(radius+5)*neg - 10*neg,
                          radius,ColorFromType(data->output_types[i]));
            }
        }else{//odd
            c.fill_circle(clip.right,
                      clip.top+(clip.get_height()/2),
                      radius,ColorFromType(data->output_types[0]));
            for(int i = 1;i < o_c; i++){
                int neg = -1;
                int offset = -25;
                if(i % 2 == 0){ neg = 1;offset = 0;}
                c.fill_circle(clip.right,
                          clip.top+(clip.get_height()/2) + i*(radius+5)*neg + offset,
                          radius,ColorFromType(data->output_types[i]));
            }
        }
        //! ---------- Draw outputs ----------


        c.fill_rect(clip,Colorf(25,25,25,255));
        c.fill_rect(Rectf(clip.left+2,clip.top+50,clip.right-2,clip.bottom-2),Colorf::darkgrey);
        const int width = font->get_text_size(c,data->title).width;
        font->draw_text(c,get_geometry().get_width()/2-width/2,35,data->title,text_color);

        if(selected)
            c.draw_box(Rect(clip.left+1,clip.top,clip.right,clip.bottom),Colorf(255,255,255,255));
    }

    Vec2f position;
    Sizef size;
    clan::Font *font;
    TriggerElementTemplate *data;
    Colorf text_color;
    bool selected = false;
};


class TriggerEvents : public GUIComponent{
public:
    TriggerEvents(clan::GUIComponent *parent,Rect f) : GUIComponent(parent){
        set_geometry(f);
        func_render().set(this,&TriggerEvents::Draw);
        func_input().set(this,&TriggerEvents::on_input);
    }
    void Draw(Canvas &c, const Rect &clip){
        c.fill_rect(clip,Colorf::black);
    }
    bool on_input(const InputEvent &e){
        ;
        return true;
    }
};

class TriggerEditor : public GUIComponent{
public:
    TriggerEditor(clan::GUIComponent *parent,Rect f,Canvas *root_canvas) : GUIComponent(parent){
        set_geometry(f);
        this->root_canvas = root_canvas;
        font = new clan::Font(*root_canvas,"./GUIResources/Tahoma.ttf", 34);
        this->set_visible(false);
        func_render().set(this,&TriggerEditor::Draw);
        func_input().set(this,&TriggerEditor::on_input);
        TriggerElementTemplate *data = new TriggerElementTemplate(/*inputs*/new TriggerTypes[4]{TriggerTypes::LogicGates,TriggerTypes::Filter,TriggerTypes::Intiger,TriggerTypes::Intiger},4,/*outputs*/new TriggerTypes{TriggerTypes::Filter},1,TriggerTypes::Filter);
        nodes.push_back(new TriggerNode(this,Vec2f(220,220),Sizef(300,200),font,data));
        TriggerEvents *toolbar = new TriggerEvents(this,Rect(get_geometry().get_width()-100,0,get_geometry().get_width(),get_geometry().get_height()));
    }
    ~TriggerEditor(){
        for(auto &node : nodes)
            delete node;
        nodes.clear();
        delete font;
    }
    bool on_input(const InputEvent &);
    std::vector<TriggerNode *> nodes;
    void Draw(Canvas &c, const Rect &clip);
    bool is_closed = true;
    Canvas *root_canvas;
    clan::Font *font;

};

