#pragma once
#include <iostream>

#include "../TrollMath/TrollMath.hpp"

#include <ClanLib/display.h>
#include <algorithm>
#include "../TrollCore/Algorithm.hpp"

#include "../TrollPath/SaveLoadMeshData.hpp"
#include "../TrollPath/RunTimeData.hpp"
#include "../TrollPath/FunnelSystem.hpp"

namespace TPath{

    struct NavMesh{

        NavMesh(const NavSaveLoadData *mesh_file_data);
        void activate_quad_node(NavTriangle *tri, const TMath::Point *p);
        //! prepare the navigation mesh for another path to be found
        void reset_mesh();
        void recalculate_node_weight(const NavTriangle *tri);
        //! gets the quad that a point is in
        NavTriangle *get_containing_tri(const TMath::Point *p);
        //! calculate a path between two points to the end
        PathData *find_path( TPath::AIAgent *agent, const TMath::Point *point_a, const TMath::Point *point_b );

        unsigned int point_count=0,edge_count=0,triangle_count=0,node_count=0;

        NavPoint *list_of_points=0;
        NavEdge *list_of_edges=0;
        NavTriangle *list_of_triangles=0;
        NavNode *list_of_nodes=0;
    };
};
