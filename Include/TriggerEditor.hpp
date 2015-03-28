#pragma once
//#include "Precomp.hpp"
#include "GlobalVars.hpp"
#include "TriggerElementTemplates.hpp"

using namespace clan;

class TriggerNode : public GUIComponent{
private:
    Vec2f last_position;
public:
    TriggerNode(clan::GUIComponent *parent,Vec2f position,Sizef size,clan::Font *font,TriggerElementTemplate *data) : GUIComponent(parent){
//        set_geometry(Rectf(position.x,position.y,position.x+size.width,position.y+size.height));
        set_geometry(Rectf(0,0,size.width,size.height));
        func_render().set(this,&TriggerNode::Draw);
        this->size = size;
        this->position = position;
        this->font = font;
        this->data = data;
        text_color = ColorFromType(data->type);
    }
    ~TriggerNode(){
    }
//    void Draw(Canvas &c, const Rect &clip){
//        const float zoom = *zoompntr;
//        Rect drawing_rect = Rectf((position.x-cam_pos->x)/zoom,
//                                  (position.y-cam_pos->y)/zoom,
//                                  (position.x-cam_pos->x+size.width)/zoom,
//                                  (position.y-cam_pos->y+size.height)/zoom);
//
//        const int height = size.height/zoom;
//        const int width = size.width/zoom;
//
//
//        const float radius = 20;
//        //! ---------- Draw inputs ----------
//        const int i_c = data->input_cnt;
//        if(i_c % 2 == 0){//even
//            for(int i = 0;i < i_c; i++){
//                int neg = -1;
//                if(i % 2 == 0) neg = 1;
//                c.fill_circle(drawing_rect.left,
//                          drawing_rect.top+(height/2 + (radius+5)/2) + (i+1)*(radius+5)*neg - 10*neg,
//                          radius,ColorFromType(data->input_types[i]));
//            }
//        }else{//odd
//            c.fill_circle(drawing_rect.left,
//                      drawing_rect.top+(height/2),
//                      radius,ColorFromType(data->input_types[0]));
//            for(int i = 1;i < i_c; i++){
//                int neg = -1;
//                int offset = -25;
//                if(i % 2 == 0){ neg = 1;offset = 0;}
//                c.fill_circle(drawing_rect.left,
//                          drawing_rect.top+(height/2) + i*(radius+5)*neg + offset,
//                          radius,ColorFromType(data->input_types[i]));
//            }
//        }
//        //! ---------- Draw inputs ----------
//
//        //! ---------- Draw outputs ----------
//        const int o_c = data->output_cnt;
//        if(o_c % 2 == 0){//even
//            for(int i = 0;i < o_c; i++){
//                int neg = -1;
//                if(i % 2 == 0) neg = 1;
//                c.fill_circle(drawing_rect.left+width,
//                          drawing_rect.top+(height/2 + (radius+5)/2) + (i+1)*(radius+5)*neg - 10*neg,
//                          radius,ColorFromType(data->output_types[i]));
//            }
//        }else{//odd
//            c.fill_circle(drawing_rect.left+width,
//                      drawing_rect.top+(height/2),
//                      radius,ColorFromType(data->output_types[0]));
//            for(int i = 1;i < o_c; i++){
//                int neg = -1;
//                int offset = -25;
//                if(i % 2 == 0){ neg = 1;offset = 0;}
//                c.fill_circle(drawing_rect.left+width,
//                          drawing_rect.top+(height/2) + i*(radius+5)*neg + offset,
//                          radius,ColorFromType(data->output_types[i]));
//            }
//        }
//        //! ---------- Draw outputs ----------
//
//
////        c.fill_rect(clip,Colorf(25,25,25,255));
//        c.fill_rect(Rectf(drawing_rect.left,drawing_rect.top,drawing_rect.left+width,drawing_rect.top+height),Colorf(25,25,25,255));
//        c.fill_rect(Rectf(drawing_rect.left+2,drawing_rect.top+50,drawing_rect.left+width-2,drawing_rect.top+height-2),Colorf::darkgrey);
//        const int text_width = font->get_text_size(c,data->title).width;
//        font->draw_text(c,drawing_rect.left+get_geometry().get_width()/2-text_width/2,drawing_rect.top+35,data->title,text_color);
//
//        if(selected)
//            c.draw_box(Rect(drawing_rect.left+1,drawing_rect.top,drawing_rect.left+width,drawing_rect.top+height),Colorf(255,255,255,255));
//    }
    void Draw(Canvas &c, const Rect &clip){
        const float zoom = *zoompntr;
        Rect drawing_rect = Rectf((position.x-cam_pos->x)/zoom,
                                  (position.y-cam_pos->y)/zoom,
                                  (position.x-cam_pos->x+size.width)/zoom,
                                  (position.y-cam_pos->y+size.height)/zoom);

        const int height = size.height/zoom;
        const int width = size.width/zoom;


        const float radius = 20/zoom;
        //! ---------- Draw inputs ----------
        const int i_c = data->input_cnt;
        if(i_c % 2 == 0){//even
            for(int i = 0;i < i_c; i++){
                int neg = -1;
                if(i % 2 == 0) neg = 1;
                c.fill_circle(drawing_rect.left,
                          drawing_rect.top+(height/2 + (radius+5)/2) + (i+1)*(radius+5)*neg - 10*neg,
                          radius,ColorFromType(data->input_types[i]));
            }
        }else{//odd
            c.fill_circle(drawing_rect.left,
                      drawing_rect.top+(height/2),
                      radius,ColorFromType(data->input_types[0]));
            for(int i = 1;i < i_c; i++){
                int neg = -1;
                int offset = -25;
                if(i % 2 == 0){ neg = 1;offset = 0;}
                c.fill_circle(drawing_rect.left,
                          drawing_rect.top+(height/2) + i*(radius+5)*neg + offset,
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
                c.fill_circle(drawing_rect.left+width,
                          drawing_rect.top+(height/2 + (radius+5)/2) + (i+1)*(radius+5)*neg - 10*neg,
                          radius,ColorFromType(data->output_types[i]));
            }
        }else{//odd
            c.fill_circle(drawing_rect.left+width,
                      drawing_rect.top+(height/2),
                      radius,ColorFromType(data->output_types[0]));
            for(int i = 1;i < o_c; i++){
                int neg = -1;
                int offset = -25;
                if(i % 2 == 0){ neg = 1;offset = 0;}
                c.fill_circle(drawing_rect.left+width,
                          drawing_rect.top+(height/2) + i*(radius+5)*neg + offset,
                          radius,ColorFromType(data->output_types[i]));
            }
        }
        //! ---------- Draw outputs ----------


        c.fill_rect(Rectf(drawing_rect.left,drawing_rect.top,drawing_rect.left+width,drawing_rect.top+height),Colorf(25,25,25,255));
        c.fill_rect(Rectf(drawing_rect.left+2,drawing_rect.top+50,drawing_rect.left+width-2,drawing_rect.top+height-2),Colorf::darkgrey);
        const int text_width = font->get_text_size(c,data->title).width;
        font->draw_text(c,drawing_rect.left+get_geometry().get_width()/2-text_width/2,drawing_rect.top+35,data->title,text_color);

        if(selected)
            c.draw_box(Rect(drawing_rect.left+1,drawing_rect.top,drawing_rect.left+width,drawing_rect.top+height),Colorf(255,255,255,255));
    }

    bool PointInBound(Vec2f point) const{
        const int height = size.height / *zoompntr;
        const int width = size.width / *zoompntr;
        return position.x-cam_pos->x < point.x && position.x+width-cam_pos->x > point.x && position.y-cam_pos->y < point.y && position.y+height-cam_pos->y > point.y;
    }

    void Grabbing(Vec2f mouse_pos){
        position = last_position + mouse_pos;
    }
    void StartGrab(Vec2f mouse_pos){
        last_position = position - mouse_pos;
    }

    Vec2f position;
    Sizef size;
    clan::Font *font;
    TriggerElementTemplate *data;
    Colorf text_color;
    Vec2f *cam_pos;
    float *zoompntr;
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
        zoom = 1;
        set_geometry(f);
        this->root_canvas = root_canvas;
        font = new clan::Font(*root_canvas,"./GUIResources/Tahoma.ttf", 34);
        this->set_visible(false);
        func_render().set(this,&TriggerEditor::Draw);
        //! call this from parent instead because of weird bug
//        func_input().set(this,&TriggerEditor::on_input);
        TriggerElementTemplate *data = new TriggerElementTemplate(/*inputs*/new TriggerTypes[4]{TriggerTypes::LogicGates,TriggerTypes::Filter,TriggerTypes::Intiger,TriggerTypes::Intiger},4,/*outputs*/new TriggerTypes{TriggerTypes::Filter},1,TriggerTypes::Filter);
        nodes.push_back(new TriggerNode(this,Vec2f(220,220),Sizef(300,200),font,data));

        nodes.back()->cam_pos = &cam_position;
        nodes.back()->zoompntr = &zoom;

//        TriggerElementTemplate *data2 = new TriggerElementTemplate(/*inputs*/new TriggerTypes[1]{TriggerTypes::Filter},1,/*outputs*/new TriggerTypes{TriggerTypes::Filter},1,TriggerTypes::Event);
//        nodes.push_back(new TriggerNode(this,Vec2f(220,220),Sizef(300,200),font,data2));
//
//        nodes.back()->cam_pos = &cam_position;
//        nodes.back()->zoompntr = &zoom;

        toolbar = new TriggerEvents(this,Rect(get_geometry().get_width()-100,0,get_geometry().get_width(),get_geometry().get_height()));
    }
    ~TriggerEditor(){
        for(auto &node : nodes)
            delete node;
        nodes.clear();
        delete font;
        delete toolbar;
    }
    bool on_input(const InputEvent &);
    std::vector<TriggerNode *> nodes;
    void Draw(Canvas &c, const Rect &clip);
    bool is_closed = true;
    Canvas *root_canvas;
    clan::Font *font;
    TriggerEvents *toolbar = 0;
    Vec2f cam_position;
    float zoom;
};

