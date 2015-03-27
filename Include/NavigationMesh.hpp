#pragma once
#include "GlobalVars.hpp"
#include "../TrollGameEngine/TrollFileSystem/FileHelp.hpp"
#include "NavMapLoadData.hpp"

using namespace TPath;


class NavigationMesh{
public:
    NavigationMesh();
    ~NavigationMesh();
    void Draw(Canvas &c, const int cam_x,const int cam_y,const float zoom);
    bool Extrude(const int Line_Start_Id,const TMath::Point new_pos2,const TMath::Point new_pos1);
    void Extrude(const int Point_Start_Id,const TMath::Point new_pos);
    bool ExtrudeMerge(const int Line_Start_Id,const int snap_to_edge);
    void Input(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom);
    std::vector<NavSaveLoadPoint> points;
    std::vector<NavSaveLoadEdge> edges;
    std::vector<NavSaveLoadTriangle> quads;
    TMath::Point placment_point_a = TMath::Point(-100,-100);
    TMath::Point placment_point_b = TMath::Point(-100,-100);
    std::vector<TMath::Point> placment_points_a;
    std::vector<TMath::Point> placment_points_b;
    std::vector<TMath::Point> placment_points;
    std::vector<int> edges_selected;
    std::vector<int> points_selected;
    bool is_point_selected = false;
    bool Selection(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom);
    bool SelectionEdge(const clan::InputEvent &e,const int cam_x,const int cam_y, const float zoom);
    bool is_extruding = false;
    bool GetShortestTwoLines(const TMath::Point point_a,const TMath::Point point_b,const TMath::Point point_c,const TMath::Point point_d);
    void MakeFace(const int edge1,const int edge2,const int edge3);
    void MakeEdge(const int point_1,const int point_2);
    void EraseEdge(const int i);
    void ErasePoint(const int i);
    void Save(clan::File &file);
    void LoadFrom(TPath::NavSaveLoadData &data);
    void GeneratePiece();
    void MovedPiece(int i);
    void CircleSelect(const int cam_x,const int cam_y, const float zoom);
    Rectf GenerateBoundery();
    Rectf MirrorX(Rectf map_rect);
    void RemoveDoubles();
    Vec2f CursorLoc;
    bool circle_select = false;
    int circle_select_radius = 10;
};
