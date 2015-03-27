#include "NavigationMesh.hpp"
#include <algorithm>


NavigationMesh::NavigationMesh(){
    //! don't do this as it breaks saving
//    points.push_back(NavSaveLoadPoint(TMath::Point(10,10)));
}
NavigationMesh::~NavigationMesh(){
    ;
}
void NavigationMesh::Draw(Canvas &c, const int cam_x,const int cam_y,const float zoom){
    //! draw points as fat dots

    //! draw a preveiw of the extrusion
    if(is_extruding && edges_selected.size() > 0){
        for(unsigned int i = 0;i < edges_selected.size();i++){
            c.fill_circle((placment_points_a[i].x - cam_x)/zoom,(placment_points_a[i].y - cam_y)/zoom,3,Colorf::red);
            c.fill_circle((placment_points_b[i].x - cam_x)/zoom,(placment_points_b[i].y - cam_y)/zoom,3,Colorf::red);
            c.draw_line(Vec2f(placment_points_a[i].x/zoom,placment_points_a[i].y/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Vec2f(points[edges[edges_selected[i]].a_i].x()/zoom,points[edges[edges_selected[i]].a_i].y()/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Colorf::red);
            c.draw_line(Vec2f(placment_points_b[i].x/zoom,placment_points_b[i].y/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Vec2f(points[edges[edges_selected[i]].b_i].x()/zoom,points[edges[edges_selected[i]].b_i].y()/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Colorf::red);
            c.draw_line(Vec2f(placment_points_b[i].x/zoom,placment_points_b[i].y/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Vec2f(placment_points_a[i].x/zoom,placment_points_a[i].y/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Colorf::red);
        }
    }else if(is_extruding && points_selected.size() > 0){
        int i = 0;
        for(auto &pnt : placment_points){
            c.fill_circle(pnt.x - cam_x,pnt.y - cam_y,3,Colorf::red);
            c.draw_line(Vec2f(pnt.x,pnt.y) - Vec2f(cam_x,cam_y),Vec2f(points[points_selected[i]].x(),points[points_selected[i]].y()) - Vec2f(cam_x,cam_y),Colorf::red);
            i++;
        }
    }

    int y = 0;
    for(auto &face : quads){
        face.Draw(c,cam_x,cam_y,zoom);
        face.DrawExtra(c,cam_x,cam_y,zoom,y++,GLOBAL_VARS::dfont);
    }




    //! draw the edges as gaunt lines
    for(unsigned int i = 0;i < edges.size();i++){
        Colorf line_color = Colorf::red;
        if(edges[i].is_mutable == false)
            line_color = Colorf::yellow;
        for(unsigned int x = 0;x < edges_selected.size();x++)
            if(i == edges_selected[x])
                line_color = Colorf::blue;


        //! debug draw what faces the lines are connected to
        c.draw_line(Vec2f((points[edges[i].a_i].x())/zoom,(points[edges[i].a_i].y())/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Vec2f((points[edges[i].b_i].x())/zoom,(points[edges[i].b_i].y())/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),line_color);


        GLOBAL_VARS::dfont->draw_text(c,(Vec2f(points[edges[i].a_i].x()/zoom,points[edges[i].a_i].y()/zoom) + Vec2f(points[edges[i].b_i].x()/zoom,points[edges[i].b_i].y()/zoom))/Vec2f(2,2) - Vec2f(cam_x/zoom,cam_y/zoom),
                                      clan::StringHelp::int_to_text(edges[i].quad_i_a) + "," + clan::StringHelp::int_to_text(edges[i].quad_i_b),
                                      Colorf::white);

//        //! debug draw the "ids" aka, where they are in the vector
//        GLOBAL_VARS::dfont->draw_text(c,(Vec2f(points[edges[i].a_i].x(),points[edges[i].a_i].y()) + Vec2f(points[edges[i].b_i].x(),points[edges[i].b_i].y()))/Vec2f(2,2) - Vec2f(cam_x,cam_y),
////                                      "\n" +
//                                      clan::StringHelp::int_to_text(i),
//                                      Colorf::white);

    }

    for(unsigned int i = 0;i < points.size();i++){
//        std::cout << points.size() << "\n";
        Colorf color = Colorf::red;
        if(points[i].is_mutable == false)
            color = Colorf::gold;
        if(points[i].is_divided_immutable)
            color = Colorf::purple;
        for(auto &point : points_selected)
            if(point == i)
                color = Colorf::blue;
        c.fill_circle((points[i].x() - cam_x)/zoom,(points[i].y() - cam_y)/zoom,3,color);


        //! debug draw the "ids" aka, where they are in the vector
        GLOBAL_VARS::dfont->draw_text(c,Vec2f(points[i].x()/zoom,points[i].y()/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),
                                      clan::StringHelp::int_to_text(i),
                                      Colorf::white);


        for(unsigned int y = 0;y < points[i].quads.size();y++){
            c.draw_line(Vec2f(points[i].x()/zoom,points[i].y()/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),  Vec2f(quads[points[i].quads[y]].GetCenter().x/zoom,
                                                                                        quads[points[i].quads[y]].GetCenter().y/zoom) - Vec2f(cam_x/zoom,cam_y/zoom),Colorf::green);
        }

    }

    if(circle_select){
        c.fill_circle(CursorLoc.x,CursorLoc.y,circle_select_radius,Colorf(200,200,200,155));
        c.fill_circle(CursorLoc.x,CursorLoc.y,circle_select_radius-1,Colorf(200,200,200,155));
    }
}

bool NavigationMesh::GetShortestTwoLines(const TMath::Point point_a/*points[edges[Line_Start_Id].a_i]*/,const TMath::Point point_b/*points[edges[Line_Start_Id].b_i]*/,const TMath::Point point_c/*points[edges[snap_to_edge].a_i]*/,const TMath::Point point_d/*points[edges[snap_to_edge].b_i]*/){
    const int AtoA_a = point_a.x - point_c.x;
    const int AtoA_b = point_a.y - point_c.y;
    const int AtoA_c = sqrt(AtoA_a*AtoA_a+AtoA_b*AtoA_b);

    const int BtoA_a = point_b.x - point_c.x;
    const int BtoA_b = point_b.y - point_c.y;
    const int BtoA_c = sqrt(BtoA_a*BtoA_a+BtoA_b*BtoA_b);

    const int AtoB_a = point_a.x - point_d.x;
    const int AtoB_b = point_a.y - point_d.y;
    const int AtoB_c = sqrt(AtoB_a*AtoB_a+AtoB_b*AtoB_b);

    const int BtoB_a = point_b.x - point_d.x;
    const int BtoB_b = point_b.y - point_d.y;
    const int BtoB_c = sqrt(BtoB_a*BtoB_a+BtoB_b*BtoB_b);
    return(AtoA_c + BtoB_c < AtoB_c + BtoA_c);
}

bool NavigationMesh::ExtrudeMerge(const int Line_Start_Id, const int snap_to_edge){
//    //! make sure the face isn't already connected to two faces(this is 2d you know...)
//    //! quad_i_a is always set and thus does not need to be checked
//    if(edges[Line_Start_Id].quad_i_b == -1){
//        //! create a new face
//        quads.push_back(NavLoadQuad(edges[Line_Start_Id].a_i,edges[Line_Start_Id].b_i,edges[snap_to_edge].a_i,edges[snap_to_edge].b_i,&points));
//        //! set a the first open faces slot to the new face
////        quads[edges[Line_Start_Id].GetSetQuad()].SetNextNeighborSpot(quads.size()-1);
////        quads[edges[snap_to_edge ].GetSetQuad()].SetNextNeighborSpot(quads.size()-1);
////        quads.back().SetNextNeighborSpot(edges[Line_Start_Id].GetSetQuad());
////        quads.back().SetNextNeighborSpot(edges[snap_to_edge ].GetSetQuad());
//
//        //! point our line to the new quad
//        edges[Line_Start_Id].SetOpenQuad(quads.size()-1);
//        edges[snap_to_edge].SetOpenQuad(quads.size()-1);
//
//        //! the shortest two paths will be the most untangled
//        if(GetShortestTwoLines(points[edges[Line_Start_Id].a_i].loc,points[edges[Line_Start_Id].b_i].loc,points[edges[snap_to_edge].a_i].loc,points[edges[snap_to_edge].b_i].loc)){
//            edges.push_back(NavLoadEdge(edges[Line_Start_Id].a_i,edges[snap_to_edge].a_i,quads.size()-1));
//            edges.push_back(NavLoadEdge(edges[Line_Start_Id].b_i,edges[snap_to_edge].b_i,quads.size()-1));
//        }else{
//            edges.push_back(NavLoadEdge(edges[Line_Start_Id].a_i,edges[snap_to_edge].b_i,quads.size()-1));
//            edges.push_back(NavLoadEdge(edges[Line_Start_Id].b_i,edges[snap_to_edge].a_i,quads.size()-1));
//        }
////        draw_faces.push_back(DrawFace(quads.back(), edges[edges.size()-1], edges[edges.size()-2],edges[snap_to_edge],edges[Line_Start_Id],points));
//
////        edges.push_back(NavLoadEdge(points.size()-1,points.size()-2,quads.size()-1));
//        return true;
//    }else{
//        //! we've already extruded from the line
//        return false;
//    }
}
bool NavigationMesh::Extrude(const int Line_Start_Id,const TMath::Point new_pos2,const TMath::Point new_pos1){
    //! make sure the face isn't already connected to two faces(this is 2d you know...)
    //! quad_i_a is always set and thus does not need to be checked
    if(edges[Line_Start_Id].quad_i_b == -1){
        points.push_back(NavSaveLoadPoint(new_pos1));
        points.push_back(NavSaveLoadPoint(new_pos2));
        GLOBAL_VARS::angle_map->is_viable_point.push_back(false);
        GLOBAL_VARS::angle_map->is_viable_point.push_back(false);

        //! create a new face

        edges.push_back(NavSaveLoadEdge(edges[Line_Start_Id].a_i,points.size()-1,-1));
        edges.push_back(NavSaveLoadEdge(edges[Line_Start_Id].b_i,points.size()-2,-1));
        //! cap edge
        edges.push_back(NavSaveLoadEdge(points.size()-1,points.size()-2,-1));
        //! dividing edge
        edges.push_back(NavSaveLoadEdge(edges[Line_Start_Id].b_i,points.size()-1,-1));


        points[points.size()-2].edges.push_back(edges.size()-2);
        points[points.size()-2].edges.push_back(edges.size()-1);
        points[points.size()-1].edges.push_back(edges.size()-3);
        points[points.size()-1].edges.push_back(edges.size()-1);

        points[edges[Line_Start_Id].a_i].edges.push_back(edges.size()-3);
        points[edges[Line_Start_Id].b_i].edges.push_back(edges.size()-2);

        return true;
    }else{
        //! we've already extruded from the line
        return false;
    }
}
void NavigationMesh::Extrude(const int Point_Start_Id,const TMath::Point new_pos){
    points.push_back(TMath::Point(new_pos));
    edges.push_back(NavSaveLoadEdge(Point_Start_Id,points.size()-1,-1));

    points[Point_Start_Id].edges.push_back(edges.size()-1);
    points.back().edges.push_back(edges.size()-1);
        GLOBAL_VARS::angle_map->is_viable_point.push_back(false);
}

bool NavigationMesh::Selection(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom){
    static int last_selected = -1;
    std::vector<int> selected;

    if(!e.shift)
        points_selected.clear();

    for(unsigned int i = 0;i < points.size();i++){
        const Vec2f pos = Vec2f(points[i].x(),points[i].y()) - Vec2f(cam_x,cam_y);
        const float len_a = e.mouse_pos.y - pos.y/zoom;
        const float len_b = e.mouse_pos.x - pos.x/zoom;
        if(sqrt((len_a*len_a) + (len_b*len_b)) < 10){
            selected.push_back(i);
            edges_selected.clear();
        }
    }


    if(selected.size() == 1){
        points_selected.push_back(selected[0]);
        last_selected = selected[0];
    }else if(selected.size() > 1){
        if(last_selected == -1){
            points_selected.push_back(selected[0]);
            last_selected = selected[0];
        }else{
            //! if there arn't any further in the vector we need to cycle back
            bool are_any_ahead = false;
            for(unsigned int i = 0;i < selected.size();i++){
                if(selected[i] > last_selected && !are_any_ahead){
                    points_selected.push_back(selected[i]);
                    last_selected = selected[i];
                    are_any_ahead = true;
                }
            }
            if(!are_any_ahead){
                points_selected.push_back(selected[0]);
                last_selected = selected[0];
            }
        }
    }

    RemoveDoublesFromVectorInt(points_selected);
}
double FindDistanceToSegment(double x1, double y1, double x2, double y2, double pointX, double pointY)
{
    double diffX = x2 - x1;
    float diffY = y2 - y1;
    if ((diffX == 0) && (diffY == 0))
    {
        diffX = pointX - x1;
        diffY = pointY - y1;
        return sqrt(diffX * diffX + diffY * diffY);
    }

    float t = ((pointX - x1) * diffX + (pointY - y1) * diffY) / (diffX * diffX + diffY * diffY);

    if (t < 0)
    {
        //point is nearest to the first point i.e x1 and y1
        diffX = pointX - x1;
        diffY = pointY - y1;
    }
    else if (t > 1)
    {
        //point is nearest to the end point i.e x2 and y2
        diffX = pointX - x2;
        diffY = pointY - y2;
    }
    else
    {
        //if perpendicular line intersect the line segment.
        diffX = pointX - (x1 + t * diffX);
        diffY = pointY - (y1 + t * diffY);
    }

    //returning shortest distance
    return sqrt(diffX * diffX + diffY * diffY);
}


bool NavigationMesh::SelectionEdge(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom){
    static int last_selected = -1;
    std::vector<int> selected;

    if(!e.shift)
        edges_selected.clear();

    for(unsigned int i = 0;i < edges.size();i++){
        const int dist = FindDistanceToSegment(points[edges[i].a_i].x()/zoom,points[edges[i].a_i].y()/zoom,points[edges[i].b_i].x()/zoom,points[edges[i].b_i].y()/zoom,(e.mouse_pos.x+cam_x/zoom),(e.mouse_pos.y+cam_y/zoom));
        const int space = 10;
        if(abs(dist) < space){
            selected.push_back(i);
            points_selected.clear();
        }


    }


    if(selected.size() == 1){
        edges_selected.push_back(selected[0]);
        last_selected = selected[0];
    }else if(selected.size() > 1){
        if(last_selected == -1){
            edges_selected.push_back(selected[0]);
            last_selected = selected[0];
        }else{
            //! if there arn't any further in the vector we need to cycle back
            bool are_any_ahead = false;
            for(unsigned int i = 0;i < selected.size();i++){
                if(selected[i] > last_selected && !are_any_ahead){
                    edges_selected.push_back(selected[i]);
                    last_selected = selected[i];
                    are_any_ahead = true;
                }
            }
            if(!are_any_ahead){
                edges_selected.push_back(selected[0]);
                last_selected = selected[0];
            }
        }
    }
    RemoveDoublesFromVectorInt(edges_selected);
}

void NavigationMesh::MakeFace(const int edge1,const int edge2,const int edge3){
    int point1 = edges[edge1].a_i;
    int point2 = edges[edge1].b_i;
    int point3 = 0;


    if(edges[edge2].a_i == edges[edge3].a_i || edges[edge2].a_i == edges[edge3].b_i){
        point3 = edges[edge2].a_i;
    }else if(edges[edge2].b_i == edges[edge3].a_i || edges[edge2].b_i == edges[edge3].b_i){
        point3 = edges[edge2].b_i;
    }


    quads.push_back(NavSaveLoadTriangle(point1,point2,point3,&points));

    points[point1].quads.push_back(quads.size()-1);
    points[point2].quads.push_back(quads.size()-1);
    points[point3].quads.push_back(quads.size()-1);


    quads.back().connected_edge_index_list[0] = edge1;
    quads.back().connected_edge_index_list[1] = edge2;
    quads.back().connected_edge_index_list[2] = edge3;


    //! place after so it doesn't count itself
    edges[edge1].SetOpenQuad(quads.size()-1);
    edges[edge2].SetOpenQuad(quads.size()-1);
    edges[edge3].SetOpenQuad(quads.size()-1);
}
void NavigationMesh::CircleSelect(const int cam_x,const int cam_y, const float zoom){
    if(edges_selected.empty()){
        for(unsigned int i = 0;i < points.size();i++){
            const Vec2f pos = Vec2f(points[i].x(),points[i].y()) - Vec2f(cam_x,cam_y);
            const float len_a = CursorLoc.y - pos.y/zoom;
            const float len_b = CursorLoc.x - pos.x/zoom;
            if(sqrt((len_a*len_a) + (len_b*len_b)) < circle_select_radius){
                points_selected.push_back(i);
            }
        }
        RemoveDoublesFromVectorInt(points_selected);
    }
    if(points_selected.empty()){
        for(unsigned int i = 0;i < edges.size();i++){
            const int dist = FindDistanceToSegment(points[edges[i].a_i].x()/zoom,points[edges[i].a_i].y()/zoom,points[edges[i].b_i].x()/zoom,points[edges[i].b_i].y()/zoom,(CursorLoc.x+cam_x/zoom),(CursorLoc.y+cam_y/zoom));
            if(abs(dist) < circle_select_radius){
                edges_selected.push_back(i);
            }
        }

        RemoveDoublesFromVectorInt(edges_selected);
    }
}
void NavigationMesh::EraseEdge(const int i){

    //! get our two edges
    const int quad_a = edges[i].quad_i_a;
    const int quad_b = edges[i].quad_i_b;


    // we will delete a face so we need to update all the points referencing them
    for(int x = 0;x < points.size();x++){
        for(int y = 0;y < points[x].quads.size();y++){
            if(points[x].quads[y] == quad_a && quad_a != -1){
                points[x].quads.erase(points[x].quads.begin()+y);
            }
            if(points[x].quads[y] == quad_b && quad_b != -1){
                points[x].quads.erase(points[x].quads.begin()+y);
            }
        }
        for(int y = 0;y < points[x].quads.size();y++){
            if((points[x].quads[y] > quad_a && quad_a != -1) && (points[x].quads[y] > quad_b && quad_b != -1)){
                points[x].quads[y]-=2;
            }else{
                if(points[x].quads[y] > quad_a && quad_a != -1){
                    points[x].quads[y]--;
                }
                if(points[x].quads[y] > quad_b && quad_b != -1){
                    points[x].quads[y]--;
                }
            }
        }
    }

    // cycle through all of the edges to update all their new references
    for(unsigned int y = 0;y < edges.size();y++){

        // we place this seperately than the group below
        // if another edge shares a face with this one then we set that edge's face to -1 because we will delete that face

        //! not sure if if-statement necessary
        if(quad_a != -1){
            if(edges[y].quad_i_a == quad_a) edges[y].quad_i_a = -1;
            if(edges[y].quad_i_b == quad_a) edges[y].quad_i_b = -1;
        }

        //! not sure if if-statement necessary
        if(quad_b != -1){
            if(edges[y].quad_i_a == quad_b) edges[y].quad_i_a = -1;
            if(edges[y].quad_i_b == quad_b) edges[y].quad_i_b = -1;
        }

        // we placed this seperately(instead of combining the if statements)
        // because some quads get set to negative one and thus should not be affected here.
        // because we will delete the face later all elements in the vector after it will have an index one less.
        // therefore update all the edges references if they reference a face affected.

        if(quad_a != -1 && quad_b != -1){
            if(edges[y].quad_i_a > quad_a) edges[y].quad_i_a-=2;
            if(edges[y].quad_i_b > quad_a) edges[y].quad_i_b-=2;
        }else{
            if(quad_a != -1){
                if(edges[y].quad_i_a > quad_a) edges[y].quad_i_a--;
                if(edges[y].quad_i_b > quad_a) edges[y].quad_i_b--;
            }
            if(quad_b != -1){
                if(edges[y].quad_i_a > quad_b) edges[y].quad_i_a--;
                if(edges[y].quad_i_b > quad_b) edges[y].quad_i_b--;
            }
        }

    }

    // FINALLY! Erase our quads.
    if(quad_a != -1){
        quads.erase(quads.begin()+quad_a);
    }
    if(quad_b != -1){
        if(quad_a > quad_b)
            quads.erase(quads.begin()+quad_b);
        else
            quads.erase(quads.begin()+quad_b-1);
    }

    // we are going to lose an edge so we need to update all points references to edges
    for(int x = 0;x < points.size();x++){
        for(int y = 0;y < points[x].edges.size();y++){
            if(points[x].edges[y] > i){
                points[x].edges[y]--;
            }
            else if(points[x].edges[y] == i){
                points[x].edges.erase(points[x].edges.begin()+y);
                y--;
            }
        }
    }

    //! erase an edge...
    edges.erase(edges.begin()+i);
}

void NavigationMesh::ErasePoint(const int i){
    while(points[i].edges.size() > 0){
        EraseEdge(points[i].edges[0]);
    }
    for(int x = 0;x < quads.size();x++){
        for(int y = 0;y < 3;y++){
            if(quads[x].connected_index_list[y] > i) quads[x].connected_index_list[y]--;
        }
    }
    for(int x = 0;x < edges.size();x++){
        if(edges[x].a_i > i){
            edges[x].a_i--;
        }
        if(edges[x].b_i > i){
            edges[x].b_i--;
        }
    }
    points.erase(points.begin()+i);
}
void NavigationMesh::MakeEdge(const int point_1,const int point_2){
    edges.push_back(NavSaveLoadEdge(point_1,point_2, -1));
    points[point_1].edges.push_back(edges.size()-1);
    points[point_2].edges.push_back(edges.size()-1);
}

void NavigationMesh::Input(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom){
    static bool circle_selecting = false;
    static bool grabbing = false;
    static bool action = false;
    static bool subdividing = false;
    static std::vector<TMath::Point> last_positions;
    static std::vector<TMath::Point> last_mouse_positions;
    static std::vector<TMath::Point> last_positions_a;
    static std::vector<TMath::Point> last_positions_b;
    static TMath::Point last_position_a = TMath::Point();
    static TMath::Point last_position_b = TMath::Point();
    static int snap_to_edge = -1;
    static int edge_to_subdivide = -1;
    if(IsPressed(e) && e.id == mouse_right && !action){
        Selection(e,cam_x,cam_y,zoom);
        if(points_selected.size() == 0){
            SelectionEdge(e,cam_x,cam_y,zoom);
        }
    }
    if(IsPressed(e) && e.id == mouse_right && circle_select){
        circle_select = false;
        circle_selecting = false;
        action = false;
    }

    if(circle_select){
        if(e.id == clan::mouse_wheel_down){
            circle_select_radius += 1;
            if(circle_select_radius > 50) circle_select_radius = 50;
        }
        else if(e.id == clan::mouse_wheel_up){
            circle_select_radius -= 1;
            if(circle_select_radius < 10) circle_select_radius = 10;
        }
    }

    if(IsPressed(e) && e.id == keycode_c){
        if(circle_select){
            circle_select = false;
            circle_selecting = false;
            action = false;
        }else{
            circle_select = true;
            action = true;
        }
    }
    if(IsPressed(e) && e.id == keycode_y){
        for(int i = 0;i < points_selected.size();i++){
            std::cout << points_selected[i] << "\n";
        }
    }
    if(IsPressed(e) && e.id == keycode_i){
        for(int k = 0;k < points_selected.size();k++){
            for(int j = 0;j < points[points_selected[k]].edges.size();j++){
                std::cout << edges[points[points_selected[k]].edges[j]].a_i << " ";
                std::cout << edges[points[points_selected[k]].edges[j]].b_i << "\n";
            }
        }
    }
    if(circle_select && e.id == mouse_left && e.type == InputEvent::Type::pressed){
        circle_selecting = true;
    }
    if(circle_select && e.id == mouse_left && e.type == InputEvent::Type::released){
        circle_selecting = false;
    }
    if(circle_selecting){
        CircleSelect(cam_x,cam_y,zoom);
    }
    if(IsPressed(e) && e.id == mouse_left){
        if(is_extruding){
            is_extruding = false;
            action = false;
            if(edges_selected.size() > 0){
                if(snap_to_edge != -1){
                    //! add extrudemerge here
                    snap_to_edge = -1;
                }else{
                    for(unsigned int i = 0;i < edges_selected.size();i++){
                        Extrude(edges_selected[i], placment_points_a[i],placment_points_b[i]);
                    }
                }
            }else if(points_selected.size() > 0){
                for(unsigned int i = 0;i < points_selected.size();i++){
                    Extrude(points_selected[i],placment_points[i]);
                }
            }
        }if(grabbing){
            grabbing = false;
            action = false;
        }
    }

    if(IsPressed(e) && e.id == keycode_w){
        for(auto &selected : edges_selected){
            NavSaveLoadEdge *edge = &edges[selected];
            //! no quads up in this sleeve
            if(edge->GetSetQuad() == -1){
                if(edge->is_mutable){
                    const int center_x = (points[edge->a_i].x() + points[edge->b_i].x()) / 2;
                    const int center_y = (points[edge->a_i].y() + points[edge->b_i].y()) / 2;
                    points.push_back(TMath::Point(center_x,center_y));
                    edges.push_back(NavSaveLoadEdge(points.size()-1, edge->a_i));
                    edges[selected] = NavSaveLoadEdge(points.size()-1, edge->b_i);
                }else{
                    const int a = edges[selected].a_i;
                    const int b = edges[selected].b_i;

                    const int center_x = (points[edge->a_i].x() + points[edge->b_i].x()) / 2;
                    const int center_y = (points[edge->a_i].y() + points[edge->b_i].y()) / 2;
                    points.push_back(TMath::Point(center_x,center_y));
                    points.back().is_mutable = false;
                    points.back().is_divided_immutable = true;
                    points.back().divided_mutable_point_a = a;
                    points.back().divided_mutable_point_b = b;
                    if(points[a].is_border && points[b].is_border)
                        points.back().is_border = true;

                    edges.push_back(NavSaveLoadEdge(points.size()-1, edge->a_i));
                    edges[selected] = NavSaveLoadEdge(points.size()-1, edge->b_i);
                    edges.back().is_mutable = false;
                    edges[selected].is_mutable = false;
                }
            }
        }
    }
    if(IsPressed(e) && e.id == keycode_delete){
        std::sort(edges_selected.begin(), edges_selected.end(),[](int i, int j){ return(i>j);});
        for(unsigned int i = 0;i < edges_selected.size();i++){
            if(edges[edges_selected[i]].is_mutable)
                EraseEdge(edges_selected[i]);
        }
        std::sort(points_selected.begin(), points_selected.end(),[](int i, int j){ return(i>j);});
        for(unsigned int i = 0;i < points_selected.size();i++){
            if(points[points_selected[i]].is_mutable)
                ErasePoint(points_selected[i]);
        }
        points_selected.clear();
        edges_selected.clear();
    }
    if(IsPressed(e) && e.id == keycode_s){
//        subdividing = true;
//        action = true;
    }
    if(IsPressed(e) && e.id == keycode_k){
        for(int i = 0;i < points.size();i++){
            for(int j = 0;j < points[i].quads.size();j++){
                std::cout << i << " " << points[i].quads[j] << "\n";
            }
            std::cout << "\n";
        }
        std::cout << "-----------------\n";
    }
    if(IsPressed(e) && e.id == keycode_e){
        if(!action && edges_selected.size() > 0){
            last_positions_a.clear();
            last_positions_b.clear();
            placment_points_a.clear();
            placment_points_b.clear();
            for(unsigned int i = 0;i < edges_selected.size();i++){
                last_positions_a.push_back(TMath::Point(points[edges[edges_selected[i]].a_i].x() - e.mouse_pos.x,points[edges[edges_selected[i]].a_i].y() - e.mouse_pos.y));
                last_positions_b.push_back(TMath::Point(points[edges[edges_selected[i]].b_i].x() - e.mouse_pos.x,points[edges[edges_selected[i]].b_i].y() - e.mouse_pos.y));
                placment_points_a.push_back(TMath::Point(e.mouse_pos.x + last_positions_a.back().x,e.mouse_pos.y + last_positions_a.back().y));
                placment_points_b.push_back(TMath::Point(e.mouse_pos.x + last_positions_b.back().x,e.mouse_pos.y + last_positions_b.back().y));
            }
            is_extruding = true;
            action = true;
        }else if(!action && points_selected.size() > 0){
            last_positions.clear();
            placment_points.clear();
            //! this would need calls like points[points_selected[i]], but we can simplify this easily with the c++11 feature
            for(auto &i : points_selected){
                last_positions.push_back(TMath::Point(points[i].x() - e.mouse_pos.x,points[i].y() - e.mouse_pos.y));
                placment_points.push_back(TMath::Point(e.mouse_pos.x + last_positions.back().x,e.mouse_pos.y + last_positions.back().y));
            }
            is_extruding = true;
            action = true;
        }
    }
    if(IsPressed(e) && e.id == keycode_g){
        if(!action && edges_selected.size() > 0){
            grabbing = true;
            action = true;
            last_positions_a.clear();
            last_positions_b.clear();
            for(unsigned int i = 0;i < edges_selected.size();i++){
                last_positions_a.push_back(TMath::Point(points[edges[edges_selected[i]].a_i].x() - e.mouse_pos.x,points[edges[edges_selected[i]].a_i].y() - e.mouse_pos.y));
                last_positions_b.push_back(TMath::Point(points[edges[edges_selected[i]].b_i].x() - e.mouse_pos.x,points[edges[edges_selected[i]].b_i].y() - e.mouse_pos.y));
            }
        }
        if(!action && points_selected.size() > 0){
            grabbing = true;
            action = true;
            last_mouse_positions.clear();
            last_positions.clear();
            for(unsigned int i = 0;i < points_selected.size();i++){
                if(points[points_selected[i]].is_divided_immutable){
                    last_positions.push_back(TMath::Point(points[points_selected[i]].x(),points[points_selected[i]].y()));
                    last_mouse_positions.push_back(TMath::Point(e.mouse_pos.x,e.mouse_pos.y));
                }else{
                    last_positions.push_back(TMath::Point(points[points_selected[i]].x() - e.mouse_pos.x,points[points_selected[i]].y() - e.mouse_pos.y));
                    last_mouse_positions.push_back(TMath::Point(e.mouse_pos.x,e.mouse_pos.y));
                }
            }
        }
    }
    if(IsPressed(e) && e.id == keycode_f){
        if(!action && edges_selected.size() == 3){
            MakeFace(edges_selected[0],edges_selected[1],edges_selected[2]);
        }
        if(!action && points_selected.size() == 2){
            MakeEdge(points_selected[0],points_selected[1]);
        }
    }

    if(e.type == e.pointer_moved){
        if(is_extruding){
            if(edges_selected.size() > 0){
                placment_points_a.clear();
                placment_points_b.clear();
                for(unsigned int i = 0;i < edges_selected.size();i++){
                    placment_points_a.push_back(TMath::Point(e.mouse_pos.x + last_positions_a[i].x,e.mouse_pos.y + last_positions_a[i].y));
                    placment_points_b.push_back(TMath::Point(e.mouse_pos.x + last_positions_b[i].x,e.mouse_pos.y + last_positions_b[i].y));
                }
            }else if(points_selected.size() > 0){
                placment_points.clear();
                for(unsigned int i = 0;i < points_selected.size();i++){
                    placment_points.push_back(TMath::Point(e.mouse_pos.x + last_positions[i].x,e.mouse_pos.y + last_positions[i].y));
                }
            }
        }if(grabbing){
            if(edges_selected.size() > 0){
                snap_to_edge = -1;
                if(!e.ctrl){
                    for(unsigned int i = 0;i < edges_selected.size();i++){
                        if(points[edges[edges_selected[i]].a_i].is_mutable)
                            points[edges[edges_selected[i]].a_i].loc = TMath::Point(e.mouse_pos.x + last_positions_a[i].x,e.mouse_pos.y + last_positions_a[i].y);
                        if(points[edges[edges_selected[i]].b_i].is_mutable)
                            points[edges[edges_selected[i]].b_i].loc = TMath::Point(e.mouse_pos.x + last_positions_b[i].x,e.mouse_pos.y + last_positions_b[i].y);
                    }
                }
                if(e.ctrl){
                }
            }else if(points_selected.size() > 0){
                for(unsigned int i = 0;i < points_selected.size();i++){
                    if(points[points_selected[i]].is_mutable){
                        points[points_selected[i]].loc = TMath::Point(e.mouse_pos.x + last_positions[i].x,e.mouse_pos.y + last_positions[i].y);
                    }
                    else if(points[points_selected[i]].is_divided_immutable){
                        const int a = points[points_selected[i]].divided_mutable_point_a;
                        const int b = points[points_selected[i]].divided_mutable_point_b;
                        const float rise = (points[a].loc.y - points[b].loc.y);
                        const float run  = (points[a].loc.x - points[b].loc.x);

                        float largest = rise;
                        if(abs(run) > abs(rise)) largest = run;
                        const float mouse_dist = (e.mouse_pos.x-last_mouse_positions[i].x)+(e.mouse_pos.y-last_mouse_positions[i].y);

                        points[points_selected[i]].loc = TMath::Point(last_positions[i].x + mouse_dist*(run/largest),last_positions[i].y + mouse_dist*(rise/largest));
                    }
                }
            }
        }if(subdividing){
            for(auto &edge : edges){
                if((Vec2f(points[edge.a_i].x(),points[edge.a_i].y()) - Vec2f(points[edge.b_i].x(),points[edge.b_i].y())).distance(Vec2f(e.mouse_pos)) > 10){
                    ;
                    break;
                }
            }
        }
    }
    CursorLoc = Vec2f(e.mouse_pos);
}

void NavigationMesh::Save(clan::File &file){


    file.write_uint32(quads.size());
    file.write_uint32(edges.size());
    file.write_uint32(points.size());



    for(int i = 0;i < quads.size(); i++){
        file.write_int32(quads[i].connected_edge_index_list[0]);
        file.write_int32(quads[i].connected_edge_index_list[1]);
        file.write_int32(quads[i].connected_edge_index_list[2]);
    }
    for(int i = 0;i < edges.size(); i++){
        file.write_int32(edges[i].a_i);
        file.write_int32(edges[i].b_i);
    }
    int last = 0;
    if(points.size() > 0)
        last = points[0].shape_attached_to;
    for(int i = 0;i < points.size(); i++){
        FileHelper::write_TPoint(points[i].loc,file);

        if(!points[i].is_border){
            FileHelper::write_bool(!points[i].is_mutable,file);
        }else{
            FileHelper::write_bool(false,file);
        }

        if(points[i].shape_attached_to == -1){
            FileHelper::write_bool(false,file);
        }else{
            if(points[i].shape_attached_to > last){
                FileHelper::write_bool(true,file);
                last = points[i].shape_attached_to;
            }else{
                FileHelper::write_bool(false,file);
            }
        }

    }

    //! find all the open points
    std::vector<int> temp;
    for(int i = 0;i < points.size();i++){
        if(points[i].edges.size() == points[i].quads.size()){
            temp.push_back(i);
        }
    }

    //! save open points
    file.write_uint32(temp.size());
    for(unsigned int i = 0;i < temp.size();i++){
        file.write_uint32(temp[i]);
    }
}

void NavigationMesh::LoadFrom(TPath::NavSaveLoadData &data){
    edges_selected.clear();
    points_selected.clear();

    points.clear();
    edges.clear();
    quads.clear();

    for(int i = 0;i < data.list_of_points.size();i++){
        points.push_back(data.list_of_points[i]);
    }
    edges = data.list_of_edges;
    for(int i = 0;i < edges.size();i++){
        points[edges[i].a_i].edges.push_back(i);
        points[edges[i].b_i].edges.push_back(i);
    }
    quads = data.list_of_triangles;
    for(int i = 0;i < quads.size();i++){
        quads[i].nav_points = &points;




        int point1 = edges[quads[i].connected_edge_index_list[0]].a_i;
        int point2 = edges[quads[i].connected_edge_index_list[0]].b_i;
        int point3 = 0;


        if      (edges[quads[i].connected_edge_index_list[1]].a_i == edges[quads[i].connected_edge_index_list[2]].a_i || edges[quads[i].connected_edge_index_list[1]].a_i == edges[quads[i].connected_edge_index_list[2]].b_i){
            point3 = edges[quads[i].connected_edge_index_list[1]].a_i;
        }else if(edges[quads[i].connected_edge_index_list[1]].b_i == edges[quads[i].connected_edge_index_list[2]].a_i || edges[quads[i].connected_edge_index_list[1]].b_i == edges[quads[i].connected_edge_index_list[2]].b_i){
            point3 = edges[quads[i].connected_edge_index_list[1]].b_i;
        }

        quads[i].connected_index_list[0] = point1;
        quads[i].connected_index_list[1] = point2;
        quads[i].connected_index_list[2] = point3;

        points[point1].quads.push_back(i);
        points[point2].quads.push_back(i);
        points[point3].quads.push_back(i);

        edges[quads[i].connected_edge_index_list[0]].SetOpenQuad(i);
        edges[quads[i].connected_edge_index_list[1]].SetOpenQuad(i);
        edges[quads[i].connected_edge_index_list[2]].SetOpenQuad(i);
    }
}



void NavigationMesh::RemoveDoubles(){
    // as apposed to regular deletion we will eliminate double edges first to
    // prevent problems of two edges simply swapping places in the points section
    // no need to merge double quads because that should never happen.
    // maybe add the functionality later when it is released to the
    // generally retarded population... I mean the general population.
    // also maybe make it possible to merge three points in the same spot.
    // do ^by only merging doubles then repeat until there are only singles.

    std::vector <int> edges_to_erase;
    //! iterate through edges we want to remove redundancies
    for(int i = 0; i < edges.size(); i++){
        //! iterate again so we have something to compare against
        for(int j = 0; j < edges.size(); j++){
            //! make sure we don't compare a point against itself(e.g. if j==i)
            //! and make sure that we don't compare two edges,just in different order
            //! the > or < symbol accomplishes both of these tasks
            if(j > i){
                TMath::Point i_a_pos = points[edges[i].a_i].loc;
                TMath::Point i_b_pos = points[edges[i].b_i].loc;
                TMath::Point j_a_pos = points[edges[j].a_i].loc;
                TMath::Point j_b_pos = points[edges[j].b_i].loc;
                //! the lines are the same if the following is true:
                if((i_a_pos == j_a_pos && i_b_pos == j_b_pos) || (i_b_pos == j_a_pos && i_a_pos == j_b_pos)){
                    edges[j].SetOpenQuad(edges[i].GetSetQuad());
                    edges[i].a_i = -1;
                    edges[i].b_i = -1;
                    edges_to_erase.push_back(i);
                }
            }
        }
    }

    std::vector <int> points_to_erase;
//    //! iterate through points we want to remove redundancies
//    for(int i = 0; i < points.size(); i++){
//        //! iterate again so we have something to compare against
//        for(int j = 0; j < points.size(); j++){
//            //! make sure we don't compare a point against itself
//            //! and make sure that we don't compare two points,just in different order
//            //! the > or < symbol accomplishes both of these tasks
//
//            if(j > i){
//                TMath::Point i_pos = points[i].loc;
//                TMath::Point j_pos = points[j].loc;
//                //! are these two points reduntant?
//                if(i_pos.x == j_pos.x && i_pos.y == j_pos.y){
//                    //! here we actually eliminate the redundancy
//                    //! we will merge everything from refering to i to j
//                    for(int k = 0;k < points[i].edges.size();k++){
//                        if(points[edges[points[i].edges[k]].a_i].loc == j_pos){
//                            edges[points[i].edges[k]].a_i = j;
//                        }else{
//                            edges[points[i].edges[k]].b_i = j;
//                        }
//                        points[j].edges.push_back(k);
//                    }
//                    points[i].edges.clear();
//
//
//                    for(int k = 0;k < points[i].quads.size();k++){
//                        //! if the points are in the same location then this is
//                        if(points[quads[points[i].quads[k]].connected_index_list[0]].loc == j_pos){
//                            quads[points[i].quads[k]].connected_index_list[0] = j;
//                        }else if(points[quads[points[i].quads[k]].connected_index_list[1]].loc == j_pos){
//                            quads[points[i].quads[k]].connected_index_list[1] = j;
//                        }else{
//                            quads[points[i].quads[k]].connected_index_list[2] = j;
//                        }
//                        points[j].quads.push_back(k);
//                    }
//                    points[i].quads.clear();
//                    points_to_erase.push_back(i);
//                }
//            }
//        }
//    }


//        if(points[x].quads[y] == quad_b && quad_b != -1){
//            points[x].quads.erase(points[x].quads.begin()+y);
//        }

    std::sort(edges_to_erase.begin(),edges_to_erase.end(),[](int &a, int &b){return a > b;});
    for(int i = 0;i < edges_to_erase.size();i++){
        EraseEdge(edges_to_erase[i]);
    }

    std::sort(points_to_erase.begin(),points_to_erase.end(),[](int &a, int &b){return a > b;});
    for(int i = 0;i < points_to_erase.size();i++){
        ErasePoint(points_to_erase[i]);
    }
}

Rectf NavigationMesh::MirrorX(Rectf map_rect){
    const int nav_point_size = points.size();
    const int nav_edges_size = edges.size();
    const int nav_quads_size = quads.size();
    const int map_right      = map_rect.get_top_right().x;

    for(int i = 0;i < nav_point_size;i++){
        NavSaveLoadPoint pnt = points[i];
        for(int x = 0;x < pnt.edges.size();x++){
            pnt.edges[x] += nav_edges_size;
        }
        for(int x = 0;x < pnt.quads.size();x++){
            pnt.quads[x] += nav_quads_size;
        }


        pnt.loc = TMath::Point(map_right-pnt.loc.x+map_right,pnt.loc.y);


        pnt.shape_attached_to = GLOBAL_VARS::element_list.size() - pnt.shape_attached_to;
        pnt.chain_attached_to += nav_point_size;

        points.push_back(pnt);
    }



    for(int i = 0;i < nav_edges_size;i++){
        NavSaveLoadEdge edge = edges[i];
        edge.a_i += nav_point_size;
        edge.b_i += nav_point_size;
        if(edge.quad_i_a != -1)
            edge.quad_i_a += nav_quads_size;
        if(edge.quad_i_b != -1)
            edge.quad_i_b += nav_quads_size;
        edges.push_back(edge);
    }

    for(int i = 0;i < nav_quads_size;i++){
        NavSaveLoadTriangle quad = quads[i];
        std::cout << i << " 1_1. " << quad.connected_index_list[0] << "\n";
        std::cout << i << " 2_1. " << quad.connected_index_list[1] << "\n";
        std::cout << i << " 3_1. " << quad.connected_index_list[2] << "\n";
        quad.connected_index_list[0] += nav_point_size;
        quad.connected_index_list[1] += nav_point_size;
        quad.connected_index_list[2] += nav_point_size;
        std::cout << i << " 1_2. " << quad.connected_index_list[0] << "\n";
        std::cout << i << " 2_2. " << quad.connected_index_list[1] << "\n";
        std::cout << i << " 3_2. " << quad.connected_index_list[2] << "\n";
        quad.connected_edge_index_list[0] += nav_edges_size;
        quad.connected_edge_index_list[1] += nav_edges_size;
        quad.connected_edge_index_list[2] += nav_edges_size;
        quads.push_back(quad);
    }

    RemoveDoubles();
}

void NavigationMesh::GeneratePiece(){
    auto &element = GLOBAL_VARS::element_list.back();
    const int first_point_of_piece = points.size();//! we will add a point which is what this refers to so we do not sub 1
    const int first_edge_of_piece = points.size();//! we will add a edge which is what this refers to so we do not sub 1
    if(element->type == WORLD_EDITOR_STATIC_OBSTACLE){
        const auto &element_conv = static_cast<StaticElement *>(element);
        for(unsigned int i = 0;i < element_conv->element_template_data->chain_points.size();i++){
            auto &pnt = element_conv->element_template_data->chain_points[i];
            points.push_back(TMath::Point(pnt.x + element->GetPos().x,pnt.y + element->GetPos().y));
            points.back().is_mutable = false;
            points.back().chain_attached_to = i;
            points.back().shape_attached_to = GLOBAL_VARS::element_list.size()-1;


            //! the edges attached to this are systematically made and thus can be assigned like the following:
            if(i != 0){
                points.back().edges.push_back(edges.size()+1);
                points.back().edges.push_back(edges.size());
            }
            if(i > 0){
                edges.push_back(NavSaveLoadEdge( i-1+first_point_of_piece,i+first_point_of_piece ));
                edges.back().is_mutable = false;
            }
        }
        points[first_point_of_piece].edges.push_back(first_edge_of_piece);
        points[first_point_of_piece].edges.push_back(edges.size());

        edges.push_back(NavSaveLoadEdge( points.size()-1,first_point_of_piece));
        edges.back().is_mutable = false;


    }
}
void NavigationMesh::MovedPiece(int i){
    for(int x = 0;x < points.size();x++){
        if(points[x].shape_attached_to == i)
            points[x].loc = TMath::Point(static_cast<StaticElement *>(GLOBAL_VARS::element_list[i])->GetPointX(points[x].chain_attached_to),
                                         static_cast<StaticElement *>(GLOBAL_VARS::element_list[i])->GetPointY(points[x].chain_attached_to));
    }
}
Rectf NavigationMesh::GenerateBoundery(){
    int farthest_x = 0;
    int farthest_y = 0;
    for(int ii = 0;ii < points.size();ii++){
        auto &pnt = points[ii];
        if(farthest_x < pnt.x()) farthest_x = pnt.x();
        if(farthest_y < pnt.y()) farthest_y = pnt.y();
    }
    int lowest_x = farthest_x;
    int lowest_y = farthest_y;
    for(int ii = 0;ii < points.size();ii++){
        auto &pnt = points[ii];
        if(lowest_x > pnt.x()) lowest_x = pnt.x();
        if(lowest_y > pnt.y()) lowest_y = pnt.y();
    }
    const int margin = 150;


    // add a-a-a-a-a-a-a-a-a-a-a-a-allllllll four of the points
    points.push_back(TMath::Point(lowest_x-margin,lowest_y-margin));
    points.back().is_border = true;
    points.back().is_mutable = false;
    points.push_back(TMath::Point(lowest_x-margin,farthest_y+margin));
    points.back().is_border = true;
    points.back().is_mutable = false;
    points.push_back(TMath::Point(farthest_x+margin,farthest_y+margin));
    points.back().is_border = true;
    points.back().is_mutable = false;
    points.push_back(TMath::Point(farthest_x+margin,lowest_y-margin));
    points.back().is_border = true;
    points.back().is_mutable = false;


    // add all four of the edges and reference the points back to them
    edges.push_back(NavSaveLoadEdge(points.size()-4,points.size()-3));
    points[points.size()-4].edges.push_back(edges.size()-1);
    points[points.size()-3].edges.push_back(edges.size()-1);
    edges.back().is_mutable = false;
    edges.push_back(NavSaveLoadEdge(points.size()-3,points.size()-2));
    points[points.size()-3].edges.push_back(edges.size()-1);
    points[points.size()-2].edges.push_back(edges.size()-1);
    edges.back().is_mutable = false;
    edges.push_back(NavSaveLoadEdge(points.size()-2,points.size()-1));
    points[points.size()-2].edges.push_back(edges.size()-1);
    points[points.size()-1].edges.push_back(edges.size()-1);
    edges.back().is_mutable = false;
    edges.push_back(NavSaveLoadEdge(points.size()-1,points.size()-4));
    points[points.size()-1].edges.push_back(edges.size()-1);
    points[points.size()-4].edges.push_back(edges.size()-1);
    edges.back().is_mutable = false;

    //! make sure these are recognized as no-angles by the angle editor
    GLOBAL_VARS::angle_map->is_viable_point.push_back(false);
    GLOBAL_VARS::angle_map->is_viable_point.push_back(false);
    GLOBAL_VARS::angle_map->is_viable_point.push_back(false);
    GLOBAL_VARS::angle_map->is_viable_point.push_back(false);

    return Rectf(lowest_x-margin,lowest_y-margin,farthest_x+margin,farthest_y+margin);
}
