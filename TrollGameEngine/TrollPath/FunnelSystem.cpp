#include "FunnelSystem.hpp"

using namespace TPath;

Funnel::Funnel( TPath::AIAgent *agent, PathData *path_data, const TMath::Point *point_a,  const TMath::Point *point_b ){

    shrink_path_walls(agent->radius, path_data->node_list);

    build_funnel(path_data->node_list,0,point_a);

    bool path_found=false;
    unsigned int node_i=1;
    TMath::Line test_line;
    FunnelRay *ray_to_step=0;

    while(!path_found){

        RayStepResult result;

        for(; node_i<path_data->node_list.size(); ++node_i ){

            //! get the ray that needs to be advanced and find what wall then end point in the wall will be assigned to
            ray_to_step = get_and_prime_next_ray( path_data->node_list[node_i] );
            //! make a test line to the next proposed point in the line
            test_line.set_a( ray_to_step->line.get_a() );
            test_line.set_b( &ray_to_step->nav_point->shrink_pos );
            //! find if the new line is in the funnel, makes a new apex, or is oustide

            result=test_line_in_funnel( &ray_to_step->line, &ray_to_step->opposite_ray->line, &test_line );

            if(result.advance_ray){
                ray_to_step->line.set_b(&ray_to_step->nav_point->shrink_pos);
            }

            if(result.new_apex_found){
                path_data->point_list.push_back(result.new_apex);
                break;
            }
        }

        test_line.set_b( point_b );

        //! if an apex is still not found then try the other ray
        if(!result.new_apex_found){
            ray_to_step=ray_a;
            ray_to_step->opposite_ray=ray_b;
            test_line.set_a( ray_to_step->line.get_a() );
            result=test_line_in_funnel( &ray_to_step->line, &ray_to_step->opposite_ray->line, &test_line);

            if(result.new_apex_found){
                path_data->point_list.push_back(result.new_apex);
            }
        }

        //! The last point must be considrered if no apex was found
        if(!result.new_apex_found){
            ray_to_step=ray_b;
            ray_to_step->opposite_ray=ray_a;
            test_line.set_a( ray_to_step->line.get_a());
            result=test_line_in_funnel( &ray_to_step->line, &ray_to_step->opposite_ray->line, &test_line);

            if(result.new_apex_found){
                path_data->point_list.push_back(result.new_apex);
            }
        }

        if(result.new_apex_found){

            path_data->point_list.push_back(result.new_apex);

            //! make a new funnel at the point of the new apex
            node_i = get_next_unexplored_node_index(path_data->node_list,result.new_apex);

            if(node_i<path_data->node_list.size()){
                build_funnel(path_data->node_list,node_i,&result.new_apex);
                test_line.set_a( &result.new_apex );
                ++node_i;
            }
            else{
                path_found=true;
            }
        }

        //! If there is still no apex then the algorithm is complete
        if(!result.new_apex_found ){
            path_found=true;
        }
    }

    //! reset all the lines
    reset_funnel(path_data->node_list);

    delete ray_a;
    delete ray_b;
}

void Funnel::shrink_path_walls(const float &agent_radius, std::vector<NavNode*> &path_steps){

    for(unsigned int i=0; i<path_steps.size(); ++i){
        for(unsigned int ii=0; ii<2; ++ii){

            NavPoint *nav_p = path_steps[i]->edge->a;

            if(ii==1){
                nav_p = path_steps[i]->edge->b;
            }

            if( nav_p->arc_size ){
                const float arc_2 = nav_p->arc_size / 2.0f;
                const float mag = fabs((agent_radius / sin( arc_2 )) * sin( (TMath::PI_2)-(arc_2) ))+(agent_radius/2.0f);
                nav_p->shrink_pos.x = nav_p->pos.x+(nav_p->cos_t*mag);
                nav_p->shrink_pos.y = nav_p->pos.y+(nav_p->sin_t*mag);
            }

            else{
                nav_p->shrink_pos.x = nav_p->pos.x;
                nav_p->shrink_pos.y = nav_p->pos.y;
            }
        }
    }
}

void Funnel::reset_funnel( std::vector<NavNode*> &path_steps ){
    //! reset all the lines
    for(unsigned int i=0; i<path_steps.size(); ++i){
        path_steps[i]->edge->a->is_in_tunnel=false;
        path_steps[i]->edge->b->is_in_tunnel=false;
        path_steps[i]->edge->a->is_a=false;
        path_steps[i]->edge->b->is_a=false;
        path_steps[i]->edge->a->is_b=false;
        path_steps[i]->edge->b->is_b=false;
    }
}

void Funnel::build_funnel( std::vector<NavNode*> &path_steps, unsigned int node_index, const TMath::Point *point_a){
    //! the assignmen of the first two points in the tunnel is arbitrary
    if(!ray_a){ray_a = new FunnelRay;}
    ray_a->line.set_a(point_a);
    ray_a->line.set_b(&path_steps[node_index]->edge->a->shrink_pos);

    if(!ray_b){ray_b = new FunnelRay;}
    ray_b->line.set_a(point_a);
    ray_b->line.set_b(&path_steps[node_index]->edge->b->shrink_pos);

    path_steps[node_index]->edge->a->is_a=true;
    path_steps[node_index]->edge->a->is_in_tunnel=true;
    path_steps[node_index]->edge->b->is_b=true;
    path_steps[node_index]->edge->b->is_in_tunnel=true;
}

unsigned int Funnel::get_next_unexplored_node_index(std::vector<NavNode*> &path_steps, TMath::Point &apex_point){

    reset_funnel(path_steps);

    //! start at the end and then move back to the first point and find where the new apex was and return the node
    //! that will need to be explored for the next tunnel search
    for(unsigned int i=(path_steps.size()-1); i>0; --i){

        //! if neither a nor b are in the tunnel then this is the node that needs to be started with
        if( apex_point.equal( &path_steps[i]->edge->a->shrink_pos ) ){
            return i+1;
        }

        //! if neither a nor b are in the tunnel then this is the node that needs to be started with
        if( apex_point.equal( &path_steps[i]->edge->b->shrink_pos ) ){
            return i+1;
        }
    }

    return 1;
}

FunnelRay *Funnel::get_and_prime_next_ray(NavNode *node){

    //! The first thing to do is set up all of the needed data for the rays before they can be proccessed
    FunnelRay *ray_to_step=0;
    NavPoint *edge_a=node->edge->a;
    NavPoint *edge_b=node->edge->b;

    edge_a->node=node;
    edge_b->node=node;

    //! a has not been explored, so explore it
    if(!edge_a->is_in_tunnel){

        //! Check what the opposite side is and set this point to the opposite of that
        if(edge_b->is_a){
            ray_to_step=ray_b;
            ray_to_step->opposite_ray=ray_a;
            ray_to_step->nav_point=edge_a;
            edge_a->is_b=true;
            edge_a->is_in_tunnel=true;
        }

        else{
            ray_to_step=ray_a;
            ray_to_step->opposite_ray=ray_b;
            ray_to_step->nav_point=edge_a;
            edge_a->is_a=true;
            edge_a->is_in_tunnel=true;
        }
    }

    //! b has not been explored, so explore it
    if(!edge_b->is_in_tunnel){

        if(edge_a->is_b){
            ray_to_step=ray_a;
            ray_to_step->opposite_ray=ray_b;
            ray_to_step->nav_point=edge_b;
            edge_b->is_a=true;
            edge_b->is_in_tunnel=true;
        }

        else{
            ray_to_step=ray_b;
            ray_to_step->opposite_ray=ray_a;
            ray_to_step->nav_point=edge_b;
            edge_b->is_b=true;
            edge_b->is_in_tunnel=true;
        }
    }

    return ray_to_step;
}

RayStepResult Funnel::test_line_in_funnel( const TMath::Line *a, const TMath::Line *b, const TMath::Line *new_line ){

    RayStepResult ray_result;

    float angle_a = a->get_angle(false);
    float angle_b = b->get_angle(false);
    float angle_c = new_line->get_angle(false);

    //! 1. rotate the angles so that a is always equal to 0
    angle_b = TMath::TrigOps::normalize_angle(angle_b - angle_a);
    angle_c = TMath::TrigOps::normalize_angle(angle_c - angle_a);

    //! 2. rotate b so that is in the range 0,PI
    if(angle_b > TMath::PI){
        angle_b = (TMath::PI*2.0f) - angle_b;
        angle_c = (TMath::PI*2.0f) - angle_c;
    }

    angle_a = 0.0f;

    float avrg_angle =  fabs((angle_a-angle_b) / 2.0f) - angle_c;

    //! normalize
    while(avrg_angle < -TMath::PI){
        avrg_angle += (2.0f*TMath::PI);
    }

    //! 3. now it is an easy compare
    if(angle_c >= angle_b){
        if( avrg_angle > 0.0f ) {
            //! A shall mean that the new line is on the near side of the funnel, meaning it widens the funnel and will be ignored
            return ray_result;
        }
        else{
            //! And B means that new line goes outside the funnel and overlaps the angle with opposite ray, this means that a new apex has been found
            ray_result.new_apex_found=true;
            ray_result.new_apex.set_point(b->get_b());
            return ray_result;
        }
    }

    //! The new line is inside then keep it and continue
    ray_result.advance_ray=true;
    return ray_result;
}
