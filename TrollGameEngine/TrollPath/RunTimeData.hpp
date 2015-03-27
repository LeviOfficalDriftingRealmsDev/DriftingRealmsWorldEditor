#pragma once
#include <iostream>

#include "../TrollMath/TrollMath.hpp"

#include <ClanLib/display.h>
#include <algorithm>
#include "../TrollCore/Algorithm.hpp"

#include "../TrollPath/SaveLoadMeshData.hpp"

namespace TPath{

    struct NavPoint;
    struct NavEdge;
    struct NavTriangle;
    struct NavNode;

    struct NavPoint{
        float projection_angle=0.0f;
        bool is_open=false, is_a=false, is_b=false, is_in_tunnel=false, has_angle=false;
        unsigned int id=0;
        TMath::Point pos, shrink_pos;
        std::vector<NavPoint*> neighbor_list;
        NavNode *node;
        float sin_t=0.0f, cos_t=0.0f, theta=0.0f, arc_size=0.0f;
    };

    struct NavEdge{
        bool is_open=false;
        unsigned int id=0, width=0;
        TMath::Point center_pos;
        NavPoint *a=0, *b=0;
        NavTriangle *tri_a=0, *tri_b=0;
        NavNode *node;
    };

    struct NavTriangle{
        unsigned int id=0, connected_triangle_count=0;
        NavEdge *connected_edges[3]={0,0,0};
        NavPoint *connected_points[3]={0,0,0};
        NavTriangle *connected_tris[3]={0,0,0};
        TMath::Point center_pos;
        NavNode *node;
    };

    struct NavNode{
        bool is_explored=false, alread_in_open_list=false, is_active=true;
        unsigned int id=0, index=0, path_score=TMath::INF_INT;
        std::vector<NavNode*> neighbor_nodes;
        unsigned int *neighbor_distances=0;
        NavNode *prev=0;
        NavEdge *edge=0;
        NavTriangle *tri=0;
        TMath::Point pos;
        float sin_t=0.0f, cos_t=0.0f;
    };

    struct PathData{
        bool path_found=false;
        std::vector<NavNode*> node_list;
        std::vector<TMath::Point> point_list;
    };

    struct RayStepResult{
        bool new_apex_found=false, advance_ray=false;
        TMath::Point new_apex;
    };

    struct FunnelRay{
        NavPoint *nav_point=0;
        FunnelRay *opposite_ray=0;
        TMath::Line line;
    };

    struct AIAgent{
        TMath::Point *position;
        float radius;
    };
};
