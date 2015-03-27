#include "NavMesh.hpp"

using namespace TPath;

NavMesh::NavMesh(const NavSaveLoadData *mesh_file_data){

    point_count = mesh_file_data->list_of_points.size();
    edge_count = mesh_file_data->list_of_edges.size();
    triangle_count = mesh_file_data->list_of_triangles.size();

    list_of_points = new NavPoint[point_count];
    list_of_edges = new NavEdge[edge_count];
    list_of_triangles = new NavTriangle[triangle_count];

    //! coppy all of the point positions
    for(unsigned int i=0; i<point_count; ++i ){
        list_of_points[i].id = i;
        list_of_points[i].pos.x = mesh_file_data->list_of_points[i].x;
        list_of_points[i].pos.y = mesh_file_data->list_of_points[i].y;
        list_of_points[i].shrink_pos.x = mesh_file_data->list_of_points[i].x;
        list_of_points[i].shrink_pos.y = mesh_file_data->list_of_points[i].y;
    }

    //! load the list of open points
    for(unsigned int i=0; i<mesh_file_data->list_of_open_points.size(); ++i){
        list_of_points[mesh_file_data->list_of_open_points[i]].is_open = true;
    }

    //! set the edges to have pointers to the points a and b
    for(unsigned int i=0; i<edge_count; ++i ){

        list_of_edges[i].id = i;

        //! use the saved indexes to get pointers to the points
        list_of_edges[i].a = &list_of_points[mesh_file_data->list_of_edges[i].p_a_index];
        list_of_edges[i].b = &list_of_points[mesh_file_data->list_of_edges[i].p_b_index];

        //! Use the edges as connections between the points
        list_of_edges[i].a->neighbor_list.push_back(list_of_edges[i].b);
        list_of_edges[i].b->neighbor_list.push_back(list_of_edges[i].a);

        //! set the center position of the edge
        list_of_edges[i].center_pos.x = (list_of_edges[i].a->pos.x + list_of_edges[i].b->pos.x) / 2.0f;
        list_of_edges[i].center_pos.y = (list_of_edges[i].a->pos.y + list_of_edges[i].b->pos.y) / 2.0f;
    }

    //! set the triangles to have pointers to the edges a, b, and c
    for(unsigned int i=0; i<triangle_count; ++i ){

        list_of_triangles[i].id = i;

        //! assign the edges from file indices
        for(unsigned int ii=0; ii<3; ++ii ){
            list_of_triangles[i].connected_edges[ii] = &list_of_edges[mesh_file_data->list_of_triangles[i].connected_edge_index_list[ii]];
        }

        for(unsigned int ii=0; ii<3; ++ii ){
            if(!list_of_edges[mesh_file_data->list_of_triangles[i].connected_edge_index_list[ii]].tri_a){
                list_of_edges[mesh_file_data->list_of_triangles[i].connected_edge_index_list[ii]].tri_a = &list_of_triangles[i];
            }
            else{
                list_of_edges[mesh_file_data->list_of_triangles[i].connected_edge_index_list[ii]].tri_b = &list_of_triangles[i];
            }
        }

        for(unsigned int ii=0; ii<3; ++ii ){
            list_of_triangles[i].center_pos.x += list_of_triangles[i].connected_edges[ii]->center_pos.x;
            list_of_triangles[i].center_pos.y += list_of_triangles[i].connected_edges[ii]->center_pos.y;
        }

        list_of_triangles[i].center_pos.x = list_of_triangles[i].center_pos.x/3.0f;
        list_of_triangles[i].center_pos.y = list_of_triangles[i].center_pos.y/3.0f;
    }

    for(unsigned int i=0; i<edge_count; ++i){
        if( list_of_edges[i].tri_a && list_of_edges[i].tri_b ){
            list_of_edges[i].is_open=true;
            list_of_edges[i].tri_a->connected_tris[list_of_edges[i].tri_a->connected_triangle_count] = list_of_edges[i].tri_b;
            list_of_edges[i].tri_b->connected_tris[list_of_edges[i].tri_b->connected_triangle_count] = list_of_edges[i].tri_a;
            ++list_of_edges[i].tri_a->connected_triangle_count;
            ++list_of_edges[i].tri_b->connected_triangle_count;
        }

        list_of_edges[i].width = static_cast<unsigned int>( list_of_edges[i].a->pos.get_distance_to_point(&list_of_edges[i].b->pos) );
    }

    //! the edges from the editor are not in any particular order, this section must sort them in order to get the list of points to each triangle
    for(unsigned int i=0; i<triangle_count; ++i ){

        //! get all potential points
        NavPoint *all_points[6]={0,0,0,0,0,0};

        for(unsigned int ii=0; ii<3; ++ii ){
            all_points[ii] = list_of_triangles[i].connected_edges[ii]->a;
            all_points[3+ii] = list_of_triangles[i].connected_edges[ii]->b;
        }

        //! remove duplicates in the array
        for(unsigned int ii=0; ii<6; ++ii ){
            unsigned int this_index=ii;
            for(unsigned int iii=0; iii<6; ++iii ){
                if( all_points[ii] == all_points[iii] && (this_index != iii) ){
                    all_points[iii]=0;
                }
            }
        }

        //! assign the unique points to the triangles
        unsigned int current_i=0;
        for(unsigned int ii=0; ii<6; ++ii ){
            if(all_points[ii]){
                list_of_triangles[i].connected_points[current_i] = all_points[ii];
                ++current_i;
            }
            if(current_i==3){
                break;
            }
        }
    }

    //! add the nodes to the nav mesh

    //! count the amount of nodes that will be added to the mesh
    node_count=triangle_count;

    for(unsigned int i=0; i<edge_count; ++i ){
        if( list_of_edges[i].is_open ){
            ++node_count;
        }
    }

    list_of_nodes = new NavNode[node_count];

    //! index counter is the position that the next node will be added to in the array
    unsigned int index_counter=0;

    //! add a nav node to every open edge
    for(unsigned int i=0; i<edge_count; ++i ){
        if( list_of_edges[i].is_open ){
            //! the index counter is to so that unusable edge centers (ones that are not open) are not added
            list_of_nodes[index_counter].edge = &list_of_edges[i];
            list_of_nodes[index_counter].is_active = true;
            list_of_edges[i].node = &list_of_nodes[index_counter];
            ++index_counter;
        }
    }

    //! add a nav node to every quad
    for(unsigned int i=0; i<triangle_count; ++i ){
        list_of_nodes[index_counter].tri = &list_of_triangles[i];
        //! the node in the quads will only be set/used when there is a start and destination point defined
        //! so set them to inactive until needed
        list_of_nodes[index_counter].is_active = false;
        list_of_triangles[i].node = &list_of_nodes[index_counter];
        ++index_counter;
    }

    //! connect all the nodes
    for(unsigned int i=0; i<node_count; ++i ){

        list_of_nodes[i].id = i;

        if(list_of_nodes[i].tri){
            for(unsigned int ii=0; ii<3; ++ii ){
                //! connect the nodes to the center node position
                if(list_of_nodes[i].tri->connected_edges[ii]->is_open){
                    //! add the node if it is open and make sure to add it from both directions
                    list_of_nodes[i].tri->connected_edges[ii]->node->neighbor_nodes.push_back(&list_of_nodes[i]);
                    list_of_nodes[i].neighbor_nodes.push_back(list_of_nodes[i].tri->connected_edges[ii]->node);

                    //! connect the edge nodes to each other
                    for(unsigned int iii=0; iii<3; ++iii ){
                        if(list_of_nodes[i].tri->connected_edges[ii]->node != list_of_nodes[i].tri->connected_edges[iii]->node){
                            if(list_of_nodes[i].tri->connected_edges[iii]->is_open){
                                //! add the node if it is open and make sure to add it from both directions
                                list_of_nodes[i].tri->connected_edges[ii]->node->neighbor_nodes.push_back(list_of_nodes[i].tri->connected_edges[iii]->node);
                                list_of_nodes[i].tri->connected_edges[iii]->node->neighbor_nodes.push_back(list_of_nodes[i].tri->connected_edges[ii]->node);
                            }
                        }
                    }
                }
            }
        }

        if(list_of_nodes[i].edge){
            list_of_nodes[i].pos.set_point(&list_of_nodes[i].edge->center_pos);
        }

        if(list_of_nodes[i].tri){
            list_of_nodes[i].pos.set_point(&list_of_nodes[i].tri->center_pos);
        }
    }

    //! The nodes are all connected now measure the distances among all the nodes
    for(unsigned int i=0; i<node_count; ++i ){

        list_of_nodes[i].neighbor_distances = new unsigned int[list_of_nodes[i].neighbor_nodes.size()];

        for(unsigned int ii=0; ii<list_of_nodes[i].neighbor_nodes.size(); ++ii){
            const unsigned int new_distance = static_cast<unsigned int>(list_of_nodes[i].pos.get_distance_to_point(&list_of_nodes[i].neighbor_nodes[ii]->pos));
            list_of_nodes[i].neighbor_distances[ii] = new_distance;
        }
    }

    //! calculate the angles of all of the nav points
    for(unsigned int i=0; i<point_count; ++i){
        if(mesh_file_data->angles[i] != -1.0f){
            list_of_points[i].cos_t = cos(mesh_file_data->angles[i]);
            list_of_points[i].sin_t = sin(mesh_file_data->angles[i]);
            list_of_points[i].theta = mesh_file_data->angles[i];
            list_of_points[i].arc_size = mesh_file_data->angle_sizes[i];
            list_of_points[i].has_angle=true;
        }
    }
}

void NavMesh::activate_quad_node(NavTriangle *tri, const TMath::Point *p){
    tri->node->is_active=true;
    tri->node->pos.set_point(p);
    recalculate_node_weight(tri);
}

void NavMesh::reset_mesh(){
    for(unsigned int i=0; i<node_count; ++i){

        if(list_of_nodes[i].tri){
            list_of_nodes[i].is_active = false;
            list_of_nodes[i].pos.set_point(&list_of_nodes[i].tri->center_pos);
        }
    }
}

void NavMesh::recalculate_node_weight(const NavTriangle *tri){

    for(unsigned int i=0; i<tri->node->neighbor_nodes.size(); ++i){
        const unsigned int new_distance = static_cast<unsigned int>(tri->node->pos.get_distance_to_point(&tri->node->neighbor_nodes[i]->pos));

        tri->node->neighbor_distances[i] = new_distance;

        //! unfortunatly the the connections are not in any particular order so they need to be
        //! searched so that the destinations have the same distances back to the quad being updated
        for(unsigned int ii=0; ii<tri->node->neighbor_nodes[i]->neighbor_nodes.size(); ++ii){
            if(tri->node->neighbor_nodes[i]->neighbor_nodes[ii] == tri->node){
                tri->node->neighbor_nodes[i]->neighbor_distances[ii] = new_distance;
            }
        }
    }
}

NavTriangle *NavMesh::get_containing_tri(const TMath::Point *p){

    for(unsigned int i=0; i<triangle_count; ++i){

        //! convert to barycentric coordinate system
        const float x1 = list_of_triangles[i].connected_points[0]->shrink_pos.x;
        const float x2 = list_of_triangles[i].connected_points[1]->shrink_pos.x;
        const float x3 = list_of_triangles[i].connected_points[2]->shrink_pos.x;

        const float y1 = list_of_triangles[i].connected_points[0]->shrink_pos.y;
        const float y2 = list_of_triangles[i].connected_points[1]->shrink_pos.y;
        const float y3 = list_of_triangles[i].connected_points[2]->shrink_pos.y;

        const float denominator = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
        const float a = ((y2 - y3)*(p->x - x3) + (x3 - x2)*(p->y - y3)) / denominator;
        const float b = ((y3 - y1)*(p->x - x3) + (x1 - x3)*(p->y - y3)) / denominator;
        const float c = 1 - a - b;

        if( (0 <= a) && (a <= 1) && (0 <= b) && (b <= 1) && (0 <= c) && (c <= 1) ){
            return &list_of_triangles[i];
        }
    }

    return 0;
}

//! NavNode *node_a, NavNode *node_b, const TMath::Point *point_a,  const TMath::Point *point_b

PathData *NavMesh::find_path( TPath::AIAgent *agent, const TMath::Point *point_a, const TMath::Point *point_b ){

    //! this data container will contain all of the information needed to get the agent to his destination
    PathData *return_data = new PathData;

    //! find the triangles that the start and end points are in
    NavTriangle *start_tri = get_containing_tri(point_a);
    NavTriangle *end_tri = get_containing_tri(point_b);

    //! make sure both triangles are on the map
    if( !start_tri || !end_tri ){
        return return_data;
    }

    //! if the start and end triangles are the same then simply move to the destination
    if( start_tri == end_tri ){
        return_data->path_found=true;
        return_data->point_list.push_back(point_b);
        return return_data;
    }

    NavNode *node_a = start_tri->node;
    NavNode *node_b = end_tri->node;

    //! save the origonal positions of the nodes that will be moved
    const float point_a_x=node_a->pos.x, point_a_y=node_a->pos.y, point_b_x=node_b->pos.x, point_b_y=node_b->pos.y;

    if(point_a){
        node_a->pos.x=point_a->x;
        node_a->pos.y=point_a->y;
    }

    if(point_b){
        node_b->pos.x=point_b->x;
        node_b->pos.y=point_b->y;
    }

    recalculate_node_weight( node_a->tri );
    recalculate_node_weight( node_b->tri );

    node_a->is_active = true;
    node_b->is_active = true;

    NavNode *start_node = node_a;

    std::vector<NavNode*> open_list;
    start_node->path_score = 0;

    open_list.push_back(start_node);

    for(unsigned int i=0; i<open_list.size(); ++i){

        NavNode *current_node = open_list[i];

        if(current_node == node_b){
            open_list.clear();
            break;
        }

        //! Itterate through the neighbor nodes of the currenrly being explored node
        for(unsigned int ii=0; ii<current_node->neighbor_nodes.size(); ++ii){

            const unsigned int node_dist = current_node->neighbor_distances[ii];
            NavNode *current_neighbor = current_node->neighbor_nodes[ii];

            //! check to make sure that the point is possible to explore
            if(current_neighbor->is_active){

                bool unit_fit = true;

                if(current_neighbor->edge){
                    unit_fit = (current_neighbor->edge->width > agent->radius );
                }

                //! make sure that the current neighbor has not already been visited and makes sure that it can be visited
                if( !current_neighbor->is_explored && unit_fit ){
                    if(!current_neighbor->alread_in_open_list){
                        open_list.push_back(current_neighbor);
                        current_neighbor->alread_in_open_list=true;
                    }

                    //! The new tentative score that this node may be assigned if it is smaller then its current tentative path score
                    const unsigned int path_compare = current_node->path_score + node_dist;

                    if(path_compare < current_neighbor->path_score){
                        current_neighbor->path_score = path_compare;
                        current_neighbor->prev = current_node;
                    }
                }
            }
        }

        current_node->is_explored = true;
    }

    return_data->path_found = false;

    //! the end point was reached so there is a possible path
    if(node_b->prev){

        NavNode *itterator_node = node_b;

        while(itterator_node != node_a){
            if(itterator_node != node_b){
                //! add the nodes back in reverse so that when they are listed they will be in order from start to end
                return_data->node_list.insert( return_data->node_list.begin(), itterator_node );
            }
            itterator_node = itterator_node->prev;
        }
        return_data->path_found = true;
    }

    //! now reset the nodes so that the next path can be found
    for(unsigned int i=0; i<this->node_count; ++i){
        this->list_of_nodes[i].path_score=TMath::INF_INT;
        this->list_of_nodes[i].is_explored=false;
        this->list_of_nodes[i].alread_in_open_list=false;
        this->list_of_nodes[i].prev=0;
    }

    //! return the node map to its defualt state
    node_a->is_active = false;
    node_b->is_active = false;

    if(point_a){
        node_a->pos.x=point_a_x;
        node_a->pos.y=point_a_y;
    }

    if(point_b){
        node_b->pos.x=point_b_x;
        node_b->pos.y=point_b_y;
    }

    recalculate_node_weight( node_a->tri );
    recalculate_node_weight( node_b->tri );

    //! if a path has been found then run the funnel algorythm so that the ships round the corners as tighly as possible
    if( return_data->path_found ){
        Funnel funnel(agent,return_data,point_a,point_b);
    }

    return_data->point_list.push_back(point_b);

    return return_data;
}
