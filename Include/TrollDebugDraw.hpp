
#include "ClanLib/core.h"
#include "ClanLib/display.h"
#include "TrollMath.hpp"

using namespace TMath;
using namespace TPath;

namespace TROLL{

    struct ShapeDrawData{
        bool draw_point=false;
        bool draw_lines=false;
        bool draw_radii=false;
    };

    struct NavPointDrawData{
        bool draw_point=false;
        bool draw_lines=false;
        bool draw_angles=false;
        bool draw_magnitudes=false;
        bool draw_connected_points=false;
    };

    clan::Font *dfont;

    struct Debug{

        static void draw_nav_map( TMath::ShapeList *shapes, TPath::NavigationMap *Map, NavPointDrawData *data, clan::Canvas *can, float trans_x=0, float trans_y=0){
            for(unsigned int i=0; i<Map->nav_point_list.size(); ++i ){

                TPath::NavigationPoint *cp = Map->nav_point_list[i];

                const float cx_start = cp->pos.x + trans_x;
                const float cy_start = cp->pos.y + trans_y;

                const float cx_end = cp->pos.x - (cos(cp->angle ) * 60.0f) + trans_x;
                const float cy_end = cp->pos.y - (sin(cp->angle ) * 60.0f) + trans_y;

                clan::Vec2f vs(cx_start, cy_start);
                clan::Vec2f ve(cx_end, cy_end);

                if(data->draw_point){
                    can->fill_circle( vs, 5.0f, clan::Colorf::red );

                    for(unsigned int ii=0; ii<cp->max_projection_index; ++ii ){
                        clan::Vec2f cpv(cp->pos.x-cos(cp->angle)*cp->UnitProjections[ii],cp->pos.y-sin(cp->angle)*cp->UnitProjections[ii]);
                        can->fill_circle( cpv + Vec2f(trans_x,trans_y), Map->radii_list[ii], clan::Colorf::chocolate );
                    }
                }

//                if(data->draw_angles){
//                    can->draw_line( vs , ve , clan::Colorf::green );
//                }

                if(data->draw_angles){

                    std::string msg = "";
                    msg += "ID: ";
                    msg += clan::StringHelp::int_to_text(cp->id);
                    msg += "\n";
                    /*msg += "Connected Points:\n";

                    for(unsigned int ii=0; ii<cp->connected_points_list.size(); ++ii ){
                        msg += clan::StringHelp::int_to_text(cp->connected_points_list[ii]->dest_node->id);
                        msg += ",\n";
                    }*/

                    dfont->draw_text(*can, cx_end, cy_end, msg, clan::Colorf::white);

                    can->draw_line( vs, ve, clan::Colorf::green );
                }
            }

            if(data->draw_lines){

                for(unsigned int i=0; i<Map->nav_point_list.size(); ++i ){

                    TPath::NavigationPoint *cp = Map->nav_point_list[i];

                    for(unsigned int ii=0; ii<cp->connected_points_list.size(); ++ii){

                        TPath::NavigationPoint *current_connected_point = cp->connected_points_list[ii]->dest_node;
                        TPath::NavigationPath  *current_connected_path = cp->connected_points_list[ii];

                        //! Loop through the connected points list of radius sizes
                        for(unsigned int iii=0; iii <Map->amount_of_unit_radii; ++iii ){

                            TMath::Line *test_line_upper = new TMath::Line(0,0,0,0);// = new TMath::Line(0,0,0,0);
                            TMath::Line *test_line_lower = new TMath::Line(0,0,0,0);// = new TMath::Line(0,0,0,0);

                            TPath::NavigationMap::get_ray_lines(
                                test_line_upper,
                                test_line_lower,
                                cp,
                                current_connected_point,
                                iii,
                                Map->radii_list[iii]
                            );

                            clan::Vec2f upper_point_start(test_line_upper->A.x,test_line_upper->A.y);
                            clan::Vec2f upper_point_end(test_line_upper->B.x,test_line_upper->B.y);

                            clan::Vec2f lower_point_start(test_line_lower->A.x,test_line_lower->A.y);
                            clan::Vec2f lower_point_end(test_line_lower->B.x,test_line_lower->B.y);

                            clan::Colorf current_hit_color = clan::Colorf::green;

                            test_line_upper->compute_slope();
                            test_line_lower->compute_slope();

                            if( current_connected_path->max_traveler_size_index<iii ){
                                current_hit_color = clan::Colorf::red;
                            }else{

                                can->draw_line(upper_point_start + Vec2f(trans_x,trans_y),upper_point_end + Vec2f(trans_x,trans_y),current_hit_color);
                                can->draw_line(lower_point_start + Vec2f(trans_x,trans_y),lower_point_end + Vec2f(trans_x,trans_y),current_hit_color);
                            }
                        }
                    }
                }
            }
        }

        static void draw_shape( TMath::Shape *shape, ShapeDrawData *data, clan::Canvas *can, float trans_x=0, float trans_y=0 ){

            float dx = shape->Center.x + trans_x;
            float dy = shape->Center.y + trans_y;

            clan::Vec2f clanv(dx,dy);

            switch( shape->type ){

                case CIRCLE : {

                    TMath::Circle *shape_cast = (TMath::Circle*)shape;

                    can->fill_circle( clanv, shape_cast->radius, clan::Colorf::red );

                }

                case ELLIPSE : {
                    ;
                }

                case TRIANGLE : {
                    ;
                }

                case RECTANGLE : {
                    ;
                }

                case POLYGON : {

                    TMath::Polygon *shape_cast = (TMath::Polygon*)shape;

                    const unsigned int lit = shape_cast->get_amount_of_points()-1;

                    for(unsigned int i=0; i<lit; ++i){

                        const float x1 = shape_cast->get_line_abs(i)->A.x + trans_x;
                        const float y1 = shape_cast->get_line_abs(i)->A.y + trans_y;
                        const float x2 = shape_cast->get_line_abs(i)->B.x + trans_x;
                        const float y2 = shape_cast->get_line_abs(i)->B.y + trans_y;

                        can->draw_line(x1, y1, x2, y2);
                    }


                }

                case UNDEFINED : {
                    ;
                }

                default : {
                    ;
                }

            };

        }
    };
}
