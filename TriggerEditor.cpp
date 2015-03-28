#include "TriggerEditor.hpp"

void TriggerEditor::Draw(Canvas &c, const Rect &clip){
    c.clear(Colorf::dimgrey);

    const int space = 100;
    const int bar_width = 1;
    const Colorf bar_colors = Colorf(50,50,50,255);
    for(int i = 0;i < clip.bottom;i+=space)
        c.fill_rect(Rect(0,i,clip.right,i+bar_width),bar_colors);
    for(int i = 0;i < clip.right;i+=space)
        c.fill_rect(Rect(i,0,i+bar_width,clip.bottom),bar_colors);
}
bool TriggerEditor::on_input(const InputEvent &e){
    std::vector<int> selected;
    static int last_selected = -1;
    static bool grabbing = false;
    static bool action = false;
    static Vec2f scroll_last = Vec2f(0,0);
    static bool scrolling = false;

    if(e.id == InputCode::mouse_right && IsPressed(e) && !action){
        //! I know I could do a normal for loop, but I think this looks better
        int iter = 0;
        for(auto &elem : nodes){
            if(elem->PointInBound(Vec2f(e.mouse_pos)))
                selected.push_back(iter);
            if(!e.shift)
                elem->selected = false;
            iter++;
        }
        if(selected.size() == 1){
            nodes[selected[0]]->selected = true;
            last_selected = selected[0];
        }else if(selected.size() > 1){
            if(last_selected == -1){
                nodes[selected[0]]->selected = true;
                last_selected = selected[0];
            }else{
                //! if there arn't any further in the vector we need to cycle back
                bool are_any_ahead = false;
                for(unsigned int i = 0;i < selected.size();i++){
                    if(selected[i] > last_selected && !are_any_ahead){
                        nodes[selected[i]]->selected = true;
                        last_selected = selected[i];
                        are_any_ahead = true;
                    }
                }
                if(!are_any_ahead){
                    nodes[selected[0]]->selected = true;
                    last_selected = selected[0];
                }
            }
        }
    }


    if(e.id == InputCode::mouse_middle && IsPressed(e)){
        scroll_last = Vec2f(e.mouse_pos);
        scrolling = true;
    }
    if(e.id == InputCode::mouse_middle && e.type == InputEvent::Type::released){
        scroll_last = Vec2f(e.mouse_pos);
        scrolling = false;
    }
    if(scrolling){
        cam_position -= (Vec2f(e.mouse_pos)-scroll_last)*zoom;
        scroll_last = Vec2f(e.mouse_pos);
    }

    if(e.id == InputCode::mouse_left && IsPressed(e)){
        grabbing = false;
        action = false;
    }
    if(e.id == keycode_g && IsPressed(e)){
        for(auto &elem : nodes){
            if(elem->selected){
                elem->StartGrab(Vec2f(e.mouse_pos));
            }
        }
        grabbing = true;
        action = true;
    }
    if(grabbing){
        for(auto &elem : nodes){
            if(elem->selected){
                elem->Grabbing(Vec2f(e.mouse_pos));
            }
        }
    }

    //! zooming is a later feature
//    if(e.id == clan::mouse_wheel_down){
//        zoom *= 1.1;
//        if(zoom > 20) zoom = 20;
//    }
//    else if(e.id == clan::mouse_wheel_up){
//        zoom /= 1.1;
//        if(zoom < 1) zoom = 1;
//    }
}
