#include "AngleEditor.hpp"
#include "GlobalVars.hpp"

void AngleEditor::Draw(Canvas &c, const int cam_x,const int cam_y,const float zoom){
    int i = 0;
    int current_pnt = 0;
    for(auto &pnt : GLOBAL_VARS::nav_map->points){
        if(is_viable_point[current_pnt]){
            current_pnt++;
            const float ray_length = 50;
            GLOBAL_VARS::dfont->draw_text(c,Vec2f(pnt.x(),pnt.y()) - Vec2f(cam_x,cam_y),
                                          clan::StringHelp::int_to_text(angles[i]*180/TMath::PI),
                                          Colorf::lightblue);
            GLOBAL_VARS::dfont->draw_text(c,Vec2f(pnt.x(),pnt.y()) - Vec2f(cam_x,cam_y),
                                          "\n" + clan::StringHelp::int_to_text(angle_sizes[i]*180/TMath::PI),
                                          Colorf::red);

            Vec2f projected_point = Vec2f(pnt.x()+cos(angles[i])*ray_length,pnt.y()+sin(angles[i])*ray_length);

            float subtracted_angles = angles[i]-angle_sizes[i]/2.f;
            float added_angles      = angles[i]+angle_sizes[i]/2.f;

            while(subtracted_angles > 2*PI) subtracted_angles -= 2*PI;
            while(subtracted_angles < 0) subtracted_angles += 2*PI;

            while(added_angles > 2*PI) added_angles -= 2*PI;
            while(added_angles < 0) added_angles += 2*PI;

            Vec2f negative_point = Vec2f(pnt.x()+cos(subtracted_angles)*ray_length,pnt.y()+sin(subtracted_angles)*ray_length);

            Vec2f positive_point = Vec2f(pnt.x()+cos(added_angles)*ray_length,pnt.y()+sin(added_angles)*ray_length);


            c.draw_line(Vec2f(pnt.x(),pnt.y()) - Vec2f(cam_x,cam_y), projected_point - Vec2f(cam_x,cam_y),Colorf::green);
            c.draw_line(negative_point         - Vec2f(cam_x,cam_y), Vec2f(pnt.x(),pnt.y()) - Vec2f(cam_x,cam_y),Colorf::white);
            c.draw_line(positive_point         - Vec2f(cam_x,cam_y), Vec2f(pnt.x(),pnt.y()) - Vec2f(cam_x,cam_y),Colorf::white);



            i++;
        }
    }
}
void AngleEditor::Input(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom){
    if(e.id == InputCode::mouse_left && IsPressed(e)){
        int ii = 0;
        for(unsigned int i = 0;i < GLOBAL_VARS::nav_map->points.size();i++){
            if(is_viable_point[i]){
                const Vec2f pos = Vec2f(GLOBAL_VARS::nav_map->points[i].x(),GLOBAL_VARS::nav_map->points[i].y()) - Vec2f(cam_x,cam_y);
                const float len_a = e.mouse_pos.y - pos.y;
                const float len_b = e.mouse_pos.x - pos.x;
                if(sqrt(len_a*len_a+len_b*len_b) < 10){
                    //! i is the index of the point we clicked on
                    angles[ii] += PI;
                    while(angles[ii] > 2*PI) angles[ii] -= 2*PI;
                    while(angles[ii] < 0)    angles[ii] += 2*PI;
                }
                ii++;
            }
        }
    }
    if(e.id == InputCode::mouse_right && IsPressed(e)){
        int ii = 0;
        for(unsigned int i = 0;i < GLOBAL_VARS::nav_map->points.size();i++){
            if(is_viable_point[i]){
                const Vec2f pos = Vec2f(GLOBAL_VARS::nav_map->points[i].x(),GLOBAL_VARS::nav_map->points[i].y()) - Vec2f(cam_x,cam_y);
                const float len_a = e.mouse_pos.y - pos.y;
                const float len_b = e.mouse_pos.x - pos.x;
                if(sqrt(len_a*len_a+len_b*len_b) < 10){
                    //! i is the index of the point we clicked on
                    angle_sizes[ii] = TMath::PI - angle_sizes[ii];
                    while(angle_sizes[ii] > 2*PI) angle_sizes[ii] -= 2*PI;
                    while(angle_sizes[ii] < 0)    angle_sizes[ii] += 2*PI;
                }
                ii++;
            }
        }
    }
    if(e.id == InputCode::mouse_middle && IsPressed(e)){
        int ii = 0;
        for(unsigned int i = 0;i < GLOBAL_VARS::nav_map->points.size();i++){
            if(is_viable_point[i]){
                const Vec2f pos = Vec2f(GLOBAL_VARS::nav_map->points[i].x(),GLOBAL_VARS::nav_map->points[i].y()) - Vec2f(cam_x,cam_y);
                const float len_a = e.mouse_pos.y - pos.y;
                const float len_b = e.mouse_pos.x - pos.x;
                if(sqrt(len_a*len_a+len_b*len_b) < 10){
                    //! i is the index of the point we clicked on
                    angle_sizes[ii] += TMath::PI;
                    while(angle_sizes[ii] > 2*PI) angle_sizes[ii] -= 2*PI;
                    while(angle_sizes[ii] < 0)    angle_sizes[ii] += 2*PI;
                }
                ii++;
            }
        }
    }
}
void AngleEditor::Save(clan::File &file){
    //! no need, we load the nav points first
//    file.write_uint32(is_viable_point.size());

    //! make sure points added after generating a shape get a boolean stating that they are not points that need angle
    while(is_viable_point.size() < GLOBAL_VARS::nav_map->points.size()){
        is_viable_point.push_back(false);
    }

    int x = 0;
    for(int i = 0;i < is_viable_point.size();i++){
        if(is_viable_point[i]){
            file.write_float(angles[x++]);
        }else{
            file.write_float(0.f);
        }
    }

    x = 0;
    for(int i = 0;i < is_viable_point.size();i++){
        if(is_viable_point[i]){
            file.write_float(angle_sizes[x++]);
        }else{
            file.write_float(0.f);
        }
    }
}
void AngleEditor::LoadFrom(TPath::NavSaveLoadData &data){
    is_viable_point.clear();
    angles.clear();
    angle_sizes.clear();
    is_viable_point.clear();
    for(int i = 0;i < data.angles.size();i++){
        if(GLOBAL_VARS::nav_map->points[i].shape_attached_to == -1){
            is_viable_point.push_back(false);
        }else{
            is_viable_point.push_back(true);
            angles.push_back(data.angles[i]);
            angle_sizes.push_back(data.angle_sizes[i]);
        }
    }
}

void AngleEditor::Generate(){

    std::vector<std::vector<TMath::Point> > points;
    std::vector<TMath::Point> points_temp;
    is_viable_point.clear();

    int last = GLOBAL_VARS::nav_map->points[0].shape_attached_to;
    for(int i = 0;i < GLOBAL_VARS::nav_map->points.size(); i++){
        auto &pnt = GLOBAL_VARS::nav_map->points[i];



        if(pnt.shape_attached_to == -1){
            is_viable_point.push_back(false);
        }else{
            if(pnt.shape_attached_to > last){
                last = pnt.shape_attached_to;
                points.push_back(points_temp);
                points_temp.clear();
            }
            points_temp.push_back(pnt.loc);
            is_viable_point.push_back(true);
        }
    }
    points.push_back(points_temp);




     //! itterate through the shapes
    for(unsigned int i=0; i<points.size(); ++i){

        //! itterate through each point in the current shape
        for(unsigned int ii=0; ii<points[i].size(); ++ii){

            unsigned int prev_index = ii-1;
            unsigned int curr_index = ii;
            unsigned int next_index = ii+1;



            if(ii==0){
                prev_index = points[i].size()-1;
            }
            if(ii==points[i].size()-1){
                next_index = 0;
            }

            TMath::Point prev_point(points[i][prev_index]);
            TMath::Point curr_point(points[i][curr_index]);
            TMath::Point next_point(points[i][next_index]);

            const float angle_a = curr_point.get_angle_to_point(&prev_point);
            const float angle_b = curr_point.get_angle_to_point(&next_point);

            float desired_angle = TMath::TrigOps::normalize_angle(( angle_a + angle_b ) / 2.0f);
//            float desired_angle_size = TMath::TrigOps::normalize_angle(angle_b - angle_a);
            float desired_angle_size = TMath::TrigOps::normalize_angle(angle_b + angle_a);

            if(angle_a > angle_b){
                desired_angle -= TMath::PI;
//                desired_angle_size -= TMath::PI;
            }


            while(desired_angle > 2*PI) desired_angle -= 2*PI;
            while(desired_angle < 0) desired_angle += 2*PI;

            while(desired_angle_size > 2*PI) desired_angle_size -= 2*PI;
            while(desired_angle_size < 0) desired_angle_size += 2*PI;

            angles.push_back(desired_angle);
            angle_sizes.push_back(desired_angle_size);
        }
    }

}

int AngleEditor::GetPointRangeForElement(int &first, int &last, int loc){
    first = 0;
    last = 0;
    int last_shape = 0;
    for(int i = 0;i < GLOBAL_VARS::nav_map->points.size(); i++){
        auto &pnt = GLOBAL_VARS::nav_map->points[i];

        if(pnt.shape_attached_to != -1){
            if(pnt.shape_attached_to > last_shape){

                if(i != 0){
                    if(GLOBAL_VARS::nav_map->points[i-1].shape_attached_to == loc){
                        return loc;
                    }
                }

                last_shape = pnt.shape_attached_to;
                first = i;
            }
        }
        last = i+1;
    }
    return -1;
}

void AngleEditor::Flipped_x(int loc){
    int first = 0;
    int last = 0;
    GetPointRangeForElement(first,last,loc);

    for(int i = first; i < last;i++){
        auto &pnt = GLOBAL_VARS::nav_map->points[i];
        if(pnt.shape_attached_to != -1){
            angles[i] = TMath::PI - angles[i];
        }
    }
}

void AngleEditor::Flipped_y(int loc){
    int first = 0;
    int last = 0;
    GetPointRangeForElement(first,last,loc);

    for(int i = first; i < last;i++){
        auto &pnt = GLOBAL_VARS::nav_map->points[i];
        if(pnt.shape_attached_to != -1){
            angles[i] = TMath::PI - angles[i] + TMath::PI;
        }
    }
}

void AngleEditor::GenerateShape(){
    std::vector<std::vector<TMath::Point> > points;
    std::vector<TMath::Point> points_temp;
    is_viable_point.clear();

    int last = GLOBAL_VARS::nav_map->points[0].shape_attached_to;

    for(int i = 0;i < GLOBAL_VARS::nav_map->points.size(); i++){
        auto &pnt = GLOBAL_VARS::nav_map->points[i];

        if(pnt.shape_attached_to == -1){
            is_viable_point.push_back(false);
        }else{
            if(pnt.shape_attached_to > last){
                last = pnt.shape_attached_to;
                points.push_back(points_temp);
                points_temp.clear();
            }
            points_temp.push_back(pnt.loc);
            is_viable_point.push_back(true);
        }
    }
    points.push_back(points_temp);



    const int i = points.size()-1;

    //! itterate through each point in the current shape
    for(unsigned int ii=0; ii<points[i].size(); ++ii){

        unsigned int prev_index = ii-1;
        unsigned int curr_index = ii;
        unsigned int next_index = ii+1;



        if(ii==0){
            prev_index = points[i].size()-1;
        }
        if(ii==points[i].size()-1){
            next_index = 0;
        }

        TMath::Point prev_point(points[i][prev_index]);
        TMath::Point curr_point(points[i][curr_index]);
        TMath::Point next_point(points[i][next_index]);

        float angle_a = curr_point.get_angle_to_point(&prev_point);
        float angle_b = curr_point.get_angle_to_point(&next_point);

        float desired_angle = TMath::TrigOps::normalize_angle(( angle_a + angle_b ) / 2.0f);

        if(angle_a > angle_b){
            angle_a = TMath::PI-angle_a;
        }else{
            angle_b = TMath::PI-angle_b;
        }

        float desired_angle_size = TMath::TrigOps::normalize_angle(angle_b + angle_a);

        if(angle_a > angle_b){
            desired_angle -= TMath::PI;
        }


        while(desired_angle > 2*PI) desired_angle -= 2*PI;
        while(desired_angle < 0) desired_angle += 2*PI;

        while(desired_angle_size > 2*PI) desired_angle_size -= 2*PI;
        while(desired_angle_size < 0) desired_angle_size += 2*PI;

        angles.push_back(desired_angle);
        angle_sizes.push_back(desired_angle_size);
    }


}
