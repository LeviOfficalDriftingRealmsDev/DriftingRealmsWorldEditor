#pragma once
#include "Precomp.hpp"
#include "GlobalVars.hpp"

class TabHeader;
class Tab : public clan::GUIComponent{
public:
    Tab(::TabHeader *owner, Rect f, std::string name);

    ~Tab(){
        delete button;
        delete label;
        delete image;
    }
    void on_render(Canvas &canvas, const Rect &clip_rect){
    }
    bool on_input(const clan::InputEvent &e);
//    void resize(Rect f){
//        button->set_geometry(Rectf(f));
//    }
    void on_resize(){
        button->set_geometry(Rectf(0,0,get_geometry().get_width(),35));
        label->set_geometry(Rectf(40,0,get_geometry().get_width(),35));
        label->set_text(name);
        if(label->get_preferred_content_width()+40 > get_geometry().get_width()-4){
            label->set_text("");
        }
    }
    clan::PushButton *button;
    clan::Label *label;
    clan::Image *image;
    Rectf f;
    bool dead = false;
    int id = 0;
    ::TabHeader* parent = 0;
    std::string name = "";
};


class TabHeader : public clan::GUIComponent{
public:
//    TabHeader(GUIComponent *owner, Rect f) : GUIComponent (owner, GUITopLevelDescription(f,true), "window"){
    TabHeader(GUIComponent *owner, Rect f) : GUIComponent (owner){
        set_geometry(f);
        offset_x = f.left;
        offset_y = f.top;
        func_render().set(this,&::TabHeader::on_render);
        this->f = f;
    }
    ~TabHeader(){
        for(::Tab* tab : tabs){
            delete tab;
        }
    }
    void on_render(Canvas &canvas, const Rect &clip_rect)
    {
        for(unsigned int i = 0;i < tabs.size();i++){
            if(tabs[i]->dead){
                delete comps[tabs[i]->id];
                comps.erase(comps.begin() + tabs[i]->id);
                delete tabs[i];
                tabs.erase(tabs.begin() + i);
                i--;
                count--;
                RecalculateTabs();
            }
        }
    }
    void RecalculateTabs(){
        const int c = tabs.size();
        for(unsigned int i = 0;i < tabs.size();i++){
//            tabs[i]->set_window_geometry(Rectf(
//                get_geometry().right/c*i+offset_x,
//                get_geometry().top+offset_y,
//                get_geometry().right/c*(i+1)+offset_x,
//                get_geometry().top+35+offset_y),
//                false);

//            tabs[i]->set_geometry(Rectf(
//                get_geometry().right/c*i,
//                get_geometry().top,
//                get_geometry().right/c*(i+1),
//                get_geometry().top+35));
            tabs[i]->set_geometry(Rectf(
                get_geometry().get_width()/c*i,
                get_geometry().top,
                get_geometry().get_width()/c*(i+1),
                get_geometry().top+35));

//            std::cout << f.left  << " l\n";
//            tabs[i]->resize(Rectf(
//                f.right/c*i+offset_x,
//                f.top+offset_y,
//                f.right/c*(i+1)+offset_x,
//                f.top+35+offset_y));
//            tabs[i]->resize(Rectf(
//                0,
//                0,
//                f.get_width(),
//                f.get_height()));
            tabs[i]->id = i;
        }
    }
    void TabClicked(int loc){
        for(clan::GUIComponent *comp : comps){
            comp->set_visible(false,true);
        }
        comps[loc]->set_visible(true,true);
    }
    void AddTab(std::string name, clan::GUIComponent *comp){
        static int cnt = 0;
        const int min_tab_width = 45;

        //! make shure all tabs can fit
        if(get_geometry().right/(count+1) > min_tab_width){
        }else{
            //! later add scrolling instead of toolbar expansion
            expand_by += 45;
        }
        tabs.push_back(new ::Tab(this,Rectf(0,0,10,10),name + StringHelp::int_to_text(cnt++)));
        comps.push_back(comp);
//        tabs.insert(tabs.begin(),new ::Tab(this,Rectf(0,0,10,10),name + StringHelp::int_to_text(cnt++)));
//        comps.insert(comps.begin(),comp);
        count++;
        RecalculateTabs();
    }
    void resize(Rectf f, int bottom){
        set_geometry(f);
        this->f = f;
        offset_x = f.left;
        RecalculateTabs();
        for(auto &comp : comps){
            comp->set_geometry(Rectf(f.left,f.top + 42,f.right+1,bottom));
        }
    }
    std::vector<::Tab*> tabs;
    std::vector<clan::GUIComponent*> comps;
    int offset_x;
    int offset_y;
    Rectf f;
    int count = 0;
    int expand_by = 0;
};
