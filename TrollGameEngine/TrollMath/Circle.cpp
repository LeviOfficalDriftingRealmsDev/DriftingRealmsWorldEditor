
#include "Circle.hpp"

using namespace TMath;

Circle::Circle() : Shape(){
    type=SHAPE_TYPES::CIRCLE;
}

Circle::Circle(const float &x, const float &y, const float &start_radius) : Shape(x,y,type=SHAPE_TYPES::CIRCLE){
    radius = start_radius;
}

Circle::Circle(const Point *shape_center, const float &start_radius) : Shape(shape_center,type=SHAPE_TYPES::CIRCLE){
    radius = start_radius;
}

Circle::~Circle(){
    ;
}

bool Circle::raycast(const Line *ray, RaycastCircleOutputData *output ) const{

    const Circle *test_circle = this;

    output->projection = ray->make_line_projection(test_circle->get_center_position(),false);

    if(output->projection->get_length() < test_circle->get_radius() ){

        //! get the angles that face toward the intersection points
        const float angle_of_line = ray->get_angle() - PI_2;
        const float theta = angle_of_line - PI_2;

        //! use the pythegorean rule to get the chord length of the intersection
        const float r_squard = test_circle->get_radius() * test_circle->get_radius();
        const float l_squard = output->projection->get_length_sq();
        const float half_chord_length = std::sqrt( fabs( r_squard - l_squard ) );

        const float cost = (cos(theta) * half_chord_length);
        const float sint = (sin(theta) * half_chord_length);

        const float a_x = output->projection->get_b()->x - cost;
        const float a_y = output->projection->get_b()->y - sint;

        const float b_x = output->projection->get_b()->x + cost;
        const float b_y = output->projection->get_b()->y + sint;

        output->collision_point_a = new TMath::Point(a_x,a_y);
        output->collision_point_b = new TMath::Point(b_x,b_y);

        Rectangle line_aabb;
        ray->make_bounding_box(&line_aabb);

        //! we will add 1 px of padding in the even that there is a vertical or horizontal line
        if( ray->is_horizontal() || ray->is_vertical() ){
            line_aabb.expand(1.0f);
        }

        //! now test to see if only one collision point intersected or 2
        if( line_aabb.is_point_inside(output->collision_point_a) ){
            output->a_hit = true;
        }

        if( line_aabb.is_point_inside(output->collision_point_b) ){
            output->b_hit = true;
        }

        if(output->a_hit || output->b_hit){
            output->hit_in_segment=true;
            return true;
        }
    }

    return false;
}

void Circle::set_radius(const float &r){
    radius=r;
}

float Circle::get_width(){
    return radius*2.0f;
}

float Circle::get_height(){
    return radius*2.0f;
}

float Circle::get_radius() const{
    return radius;
}
