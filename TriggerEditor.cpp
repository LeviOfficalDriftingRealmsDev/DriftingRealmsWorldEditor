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

    if(e.id == InputCode::mouse_right){
        //! I know I could do a normal for loop, but I think this looks better
        int iter = 0;
        for(auto &elem : nodes){
            if(elem->get_geometry().left < e.mouse_pos.x && elem->get_geometry().right > e.mouse_pos.x && elem->get_geometry().top < e.mouse_pos.y && elem->get_geometry().bottom > e.mouse_pos.y)
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
}
