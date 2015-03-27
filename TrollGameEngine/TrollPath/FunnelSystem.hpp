
#pragma once
#include <iostream>

#include "../TrollMath/TrollMath.hpp"

#include <ClanLib/display.h>
#include <algorithm>
#include "../TrollCore/Algorithm.hpp"

#include "../TrollPath/SaveLoadMeshData.hpp"
#include "../TrollPath/RunTimeData.hpp"
#include "../TrollPath/NavMesh.hpp"

namespace TPath{
    class Funnel{
        public:
            //! constructor
            Funnel( TPath::AIAgent *agent, PathData *path_data, const TMath::Point *point_a,  const TMath::Point *point_b );
            //! the possible states that the test_line_in_funnel method can return
            enum FUNNEL_STATE{INSIDE,SIDE_A,SIDE_B};
            //! takes a line and returns if it is inside the funnel, or which side it overlaps on
            RayStepResult test_line_in_funnel( const TMath::Line *a, const TMath::Line *b, const TMath::Line *new_line );
            //! start the construction of a new funnel
            void build_funnel( std::vector<NavNode*> &path_steps, unsigned int node_index, const TMath::Point *point_a );
            //! reset all of the bools that determine what point on the wall is a or b
            inline void reset_funnel( std::vector<NavNode*> &path_steps );
            //! reverse seach from the funnel ends to find the new apex point, a new funnel will be built at this index
            unsigned int get_next_unexplored_node_index( std::vector<NavNode*> &path_steps, TMath::Point &apex_point );
            //! The rays to do not take turns immediatly after each other, this method determines which ray needs to move next
            //! and set the pointers in the data structure to the next point
            FunnelRay *get_and_prime_next_ray(NavNode *node);
            //! Shrink the walls of the of the path so that the agents with a radius can pass through without bumping into walls
            void shrink_path_walls(const float &agent_radius, std::vector<NavNode*> &path_steps);
        private:
            FunnelRay *ray_a=0, *ray_b=0, *currentay=0, *oppositeay=0;
    };
};
