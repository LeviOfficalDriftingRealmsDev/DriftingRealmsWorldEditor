
#include "Triangle.hpp"

using namespace TMath;

Triangle::Triangle() {
    ;
}

Triangle::~Triangle() {
    ;
}

Triangle::Triangle(Point *a, Point *b, Point *c, Point *start_point) : Shape(start_point,SHAPE_TYPES::TRIANGLE) {
    this->a = Point(a);
    this->b = Point(b);
    this->c = Point(c);

    Center = Point(start_point);
    angle=0.0f;
}

Triangle::Triangle(Triangle *copy_me) : Shape(copy_me->get_center_position(),SHAPE_TYPES::TRIANGLE){

    a = Point(&copy_me->a);
    b = Point(&copy_me->b);
    c = Point(&copy_me->c);

    Center = Point(&copy_me->Center);
    angle=0.0f;
}

float Triangle::get_abs_x( POINTS abc ){
    switch(abc){
        case POINT_A : return a.x + Center.x; break;
        case POINT_B : return b.x + Center.x; break;
        case POINT_C : return c.x + Center.x; break;
        default: return 0; break;
    }
}

float Triangle::get_abs_y( POINTS abc ){
    switch(abc){
        case POINT_A : return a.y + Center.y; break;
        case POINT_B : return b.y + Center.y; break;
        case POINT_C : return c.y + Center.y; break;
        default: return 0; break;
    }
}

Point Triangle::get_abs_point( POINTS abc ){
    switch(abc){
        case POINT_A : return Point((a.x + Center.x), (a.y + Center.y)); break;
        case POINT_B : return Point((b.x + Center.x), (b.y + Center.y)); break;
        case POINT_C : return Point((c.x + Center.x), (c.y + Center.y)); break;
        default: return Point(0,0); break;
    }
}

Point *Triangle::get_a(){
    return &a;
}

Point *Triangle::get_b(){
    return &b;
}

Point *Triangle::get_c(){
    return &c;
}

Point *Triangle::get_point( POINTS abc ){
    switch(abc){
        case POINT_A : return &a; break;
        case POINT_B : return &b; break;
        case POINT_C : return &c; break;
        default: return 0; break;
    }
}

void Triangle::set_a(const float &x, const float &y, bool absolute, bool translate){
    if(translate){
        a.x += x;
        a.y += y;
    }
    else{
        a.x = x;
        a.y = y;

        if(absolute){
            a.x -= Center.x;
            a.y -= Center.y;
        }
    }
}

void Triangle::set_b(const float &x, const float &y, bool absolute, bool translate){
    if(translate){
        b.x += x;
        b.y += y;
    }
    else{
        b.x = x;
        b.y = y;

        if(absolute){
            b.x -= Center.x;
            b.y -= Center.y;
        }
    }
}

void Triangle::set_c(const float &x, const float &y, bool absolute, bool translate){
    if(translate){
        c.x += x;
        c.y += y;
    }
    else{
        c.x = x;
        c.y = y;

        if(absolute){
            c.x -= Center.x;
            c.y -= Center.y;
        }
    }
}

void Triangle::set_angle(const float &new_angle){

    if(new_angle != angle){

        const float rotation_to_to_angle = angle-new_angle;

        angle = new_angle;

        const float local_angle_a = a.get_angle_to_point(&Origin) + rotation_to_to_angle;
        const float local_angle_b = b.get_angle_to_point(&Origin) + rotation_to_to_angle;
        const float local_angle_c = c.get_angle_to_point(&Origin) + rotation_to_to_angle;

        a.set_rotation_around_point(local_angle_a,&Origin);
        b.set_rotation_around_point(local_angle_b,&Origin);
        c.set_rotation_around_point(local_angle_c,&Origin);
    }
}

void Triangle::set_angle(const float &new_angle, Point *origin){

    if(new_angle != angle){
        const float rotation_to_to_angle = angle-new_angle;
        angle = new_angle;

        const float local_angle_a = a.get_angle_to_point(origin) + rotation_to_to_angle;
        const float local_angle_b = b.get_angle_to_point(origin) + rotation_to_to_angle;
        const float local_angle_c = c.get_angle_to_point(origin) + rotation_to_to_angle;

        a.set_rotation_around_point(local_angle_a,origin);
        b.set_rotation_around_point(local_angle_b,origin);
        c.set_rotation_around_point(local_angle_c,origin);
    }
}

float Triangle::get_width(){
    return get_abs_x(get_right()) - get_abs_x(get_left());
}

float Triangle::get_height(){
    return get_abs_y(get_bottom()) - get_abs_y(get_top());
}

Triangle::POINTS Triangle::get_top(){
    //! test to see if a is the highest point
    if( (a.y >= b.y) && (a.y >= c.y) ){
        return POINTS::POINT_A;
    }
    //! now that we know that a is not the highest point just compare b and c
    else if( b.y >= c.y ){
        return POINTS::POINT_B;
    }
    else{
        return POINTS::POINT_C;
    }
}

Triangle::POINTS Triangle::get_right(){
    //! test to see if a is the highest point
    if( (a.x >= b.x) && (a.x >= c.x) ){
        return POINTS::POINT_A;
    }
    //! now that we know that a is not the highest point just compare b and c
    else if( b.x >= c.x ){
        return POINTS::POINT_B;
    }
    else{
        return POINTS::POINT_C;
    }
}

Triangle::POINTS Triangle::get_bottom(){
    //! test to see if a is the lowest point
    if( (a.y <= b.y) && (a.y <= c.y) ){
        return POINTS::POINT_A;
    }
    //! now that we know that a is not the lowest point just compare b and c
    else if( b.y <= c.y ){
        return POINTS::POINT_B;
    }
    else{
        return POINTS::POINT_C;
    }
}

Triangle::POINTS Triangle::get_left(){
    //! test to see if a is the highest point
    if( (a.x <= b.x) && (a.x <= c.x) ){
        return POINTS::POINT_A;
    }
    //! now that we know that a is not the highest point just compare b and c
    else if( b.x <= c.x ){
        return POINTS::POINT_B;
    }
    else{
        return POINTS::POINT_C;
    }
}

void Triangle::translate(float move_x, float move_y){

    Origin.translate(move_x,move_y);
    Center.translate(move_x,move_y);

    a.translate(move_x,move_y);
    b.translate(move_x,move_y);
    c.translate(move_x,move_y);
}
