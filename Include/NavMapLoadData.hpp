#pragma once
//#include "../TrollMath/TrollMath.hpp"
//struct NavSaveLoadEdge{
//    NavSaveLoadEdge(){
//    }
//    NavSaveLoadEdge( unsigned int i1, unsigned int i2){
//        a_i=i1;
//        b_i=i2;
//    }
//    NavSaveLoadEdge( unsigned int i1, unsigned int i2, int qi1){
//        a_i=i1;
//        b_i=i2;
//        quad_i_a=qi1;
//    }
//    //! sets the quad not set
//    //! returns false if they are both set
//    bool SetOpenQuad(int id){
//        if(quad_i_a == -1) quad_i_a = id;
//        else if(quad_i_b == -1) quad_i_b = id;
//        else return false;
//        return true;
//    }
//    int GetUnSetQuad(){
//        if(quad_i_a == -1) return quad_i_a;
//        return quad_i_b;
//    }
//    int GetSetQuad(){
//        if(quad_i_a != -1) return quad_i_a;
//        return quad_i_b;
//    }
//    int a_i, b_i, quad_i_a=-1, quad_i_b=-1;
//
//    //! chain points that define collision are immutable
//    bool is_mutable = true;
//};
//
//
//struct NavSaveLoadPoint{
//    NavSaveLoadPoint(){
//        loc = TMath::Point(0,0);
//    }
//    NavSaveLoadPoint(int _x, int _y){
//        loc = TMath::Point(_x,_y);
//    }
//    NavSaveLoadPoint(TMath::Point p){
//        loc = p;
//    }
//    TMath::Point loc;
//    std::vector<int> edges;
//    std::vector<int> quads;
//    int x(){return loc.x;}
//    int y(){return loc.y;}
//
//
//    //! chain points that define collision are immutable
//    bool is_mutable = true;
//
//    //! simply used for saving in the map editor(not to use as chain shape), unneccesary for actuall game
//    //! will probably eliminate later because I think I can use the shape_attached_to to detect which ones to save as "true"
//    bool is_border = false;
//
//    //! if the line was immutable and divided then it has special deletion rules
//    bool is_divided_immutable = false;
//    int shape_attached_to = -1;
//    int chain_attached_to = -1;
//
//    int divided_mutable_point_a = -1;
//    int divided_mutable_point_b = -1;
//};
//
//struct NavSaveLoadTriangle{
//    std::vector<NavSaveLoadPoint> *nav_points;
//    NavSaveLoadTriangle(){
//        ;
//    }
//    NavSaveLoadTriangle(int n1, int n2, int n3,std::vector<NavSaveLoadPoint> *_nav_points){
//        connected_index_list[0]=n1;
//        connected_index_list[1]=n2;
//        connected_index_list[2]=n3;
//        nav_points = _nav_points;
//    }
//    int connected_index_list[3]={-1,-1,-1};
//    int connected_edge_index_list[3]={-1,-1,-1};
//
//    void Draw(Canvas &c, int cam_x, int cam_y,float zoom){
////        if(connected_index_list[0] != -1){
////            c.fill_circle((GetCenter().x - cam_x)/zoom,
////                          (GetCenter().y - cam_y)/zoom,
////                          25/(zoom/2 + .5) //! this is the radius and the zoom modified it wierd so it had to be halved and we add .5 to make it return to the original size while scaling properly
////                          ,Colorf::green);
////        }
//    }
//    void DrawExtra(Canvas &c, int cam_x, int cam_y,float zoom, int i, clan::Font *dfont){
//        if(connected_index_list[0] != -1){
//            //! debug draw the "ids" aka, where they are in the vector
//            dfont->draw_text(c,(GetCenter().x - cam_x)/zoom,
//                               (GetCenter().y - cam_y)/zoom,
//                               clan::StringHelp::int_to_text(i),
//                               Colorf::white);
//        }
//    }
//    TMath::Point GetCenter(){
//        return  TMath::Point(((nav_points->at(connected_index_list[0]).x()+nav_points->at(connected_index_list[1]).x()+nav_points->at(connected_index_list[2]).x())/3),
//                             ((nav_points->at(connected_index_list[0]).y()+nav_points->at(connected_index_list[1]).y()+nav_points->at(connected_index_list[2]).y())/3));
//    }
//};
//
//
//struct NavSaveLoadData{
//    std::vector<TMath::Point> list_of_points;
//    std::vector<NavSaveLoadEdge> list_of_edges;
//    std::vector<NavSaveLoadTriangle> list_of_triangles;
//    std::vector<unsigned int> list_of_open_points;
//    std::vector<std::vector <int> > list_of_chain_points;
//    std::vector<float> angles;
//    std::vector<float> angle_sizes;
//};
