
#include "Rectangle.hpp"

using namespace TMath;

Rectangle::Rectangle(){
    type=SHAPE_TYPES::RECTANGLE;
}

Rectangle::Rectangle( const Point *shape_center, const float &x1, const float &y1, const float &x2, const float &y2 ) : Shape(shape_center,SHAPE_TYPES::RECTANGLE){

    TopLeft.x = x1;
    TopLeft.y = y1;

    BottomRight.x = x2;
    BottomRight.y = y2;

    set_absolute_positions();
}

Rectangle::Rectangle( const Point *shape_center, const float &width, const float &height ) : Shape(shape_center,SHAPE_TYPES::RECTANGLE){

    float half_width = (width / 2.0f);
    float half_height = (width / 2.0f);

    TopLeft.x = -half_width;
    TopLeft.y = -half_height;

    BottomRight.x = half_width;
    BottomRight.y = half_height;

    set_absolute_positions();
}

Rectangle::Rectangle( const float &x, const float &y, const float &x1, const float &y1, const float &x2, const float &y2 ) : Shape(x,y,SHAPE_TYPES::RECTANGLE){
    TopLeft.x = x1;
    TopLeft.y = y1;

    BottomRight.x = x2;
    BottomRight.y = y2;

    set_absolute_positions();
}

Rectangle::Rectangle( const float &x, const float &y, const float &width, const float &height ) : Shape(x,y,SHAPE_TYPES::RECTANGLE){

    float half_width = (width / 2.0f);
    float half_height = (width / 2.0f);

    TopLeft.x = -half_width;
    TopLeft.y = -half_height;

    BottomRight.x = half_width;
    BottomRight.y = half_height;

    set_absolute_positions();
}

Rectangle::~Rectangle(){
    ;
}

/*void Rectangle::make_bounding_box(const Shape *conform_shape, const float &padding){

    switch(conform_shape->get_type()){

        case SHAPE_TYPES::CIRCLE:{

            const Circle *cs = static_cast<const Circle*>(conform_shape);

            TopLeft.x = -cs->get_radius();
            TopLeft.y = -cs->get_radius();

            BottomRight.x = cs->get_radius();
            BottomRight.y = cs->get_radius();

            Center = cs->get_center_position();

            set_absolute_positions();

            break;
        }

        case SHAPE_TYPES::ELLIPSE:{
            ;
            break;
        }

        case SHAPE_TYPES::TRIANGLE:{
            ;
            break;
        }
        case SHAPE_TYPES::RECTANGLE:{
            ;
            break;
        }

        case SHAPE_TYPES::POLYGON:{

            const Polygon *cs = static_cast<const Polygon*>(conform_shape);

            TopLeft.x = cs->get_left_point(false)->x;
            TopLeft.y = cs->get_top_point(false)->y;

            BottomRight.x = cs->get_right_point(false)->x;
            BottomRight.y = cs->get_bottom_point(false)->y;

            Center = cs->get_center_position();

            set_absolute_positions();

            break;
        }

        default: break;
    }

}*/

bool Rectangle::is_point_inside(const Point *test_point) const{

    if( (test_point->x > (TopLeft.x + Center.x) ) && (test_point->y > (TopLeft.y + Center.y) ) ){
        if( (test_point->x < (BottomRight.x + Center.x) ) && (test_point->y < (BottomRight.y + Center.y) ) ){
            return true;
        }
    }
    return false;
}

bool Rectangle::does_overlap(const Rectangle* other_rect) const{

    const float a_bottom = get_bottom_right_point(true)->y;
    const float b_bottom = other_rect->get_bottom_right_point(true)->y;

    const float a_top = get_top_left_point(true)->y;
    const float b_top = other_rect->get_top_left_point(true)->y;

   if( (a_bottom > b_top) && (b_bottom > a_top) ){

        const float a_left = get_top_left_point(true)->x;
        const float b_left = other_rect->get_top_left_point(true)->x;

        const float a_right = get_bottom_right_point(true)->x;
        const float b_right = other_rect->get_bottom_right_point(true)->x;

        if( (a_left < b_right) && (a_right > b_left) ){

            return true;
        }
    }

    return false;
}

void Rectangle::set_center_position(const float &new_x, const float &new_y){
    Shape::set_center_position(new_x,new_y);
    set_absolute_positions();
}

void Rectangle::set_center_position(const Point *p){
    Shape::set_center_position(p->x,p->y);
    set_absolute_positions();
}

void Rectangle::set_bottom_right(const float &new_x, const float &new_y){
    BottomRight.x = new_x;
    BottomRight.y = new_y;
    AbsBottomRight.x = Center.x + BottomRight.x;
    AbsBottomRight.y = Center.y + BottomRight.y;
}

void Rectangle::set_bottom_right(const Point *new_pos){
    BottomRight.x = new_pos->x;
    BottomRight.y = new_pos->y;
    AbsBottomRight.x = Center.x + BottomRight.x;
    AbsBottomRight.y = Center.y + BottomRight.y;
}

void Rectangle::set_top_left(const float &new_x, const float &new_y){
    TopLeft.x = new_x;
    TopLeft.y = new_y;
    AbsTopLeft.x = Center.x + TopLeft.x;
    AbsTopLeft.y = Center.y + TopLeft.y;
}

void Rectangle::set_top_left(const Point *new_pos){
    TopLeft.x = new_pos->x;
    TopLeft.y = new_pos->y;
    AbsTopLeft.x = Center.x + TopLeft.x;
    AbsTopLeft.y = Center.y + TopLeft.y;
}

void Rectangle::expand(const float &amount){
    TopLeft.x-=amount;
    TopLeft.y-=amount;
    BottomRight.x+=amount;
    BottomRight.y+=amount;
    set_absolute_positions();
}

void Rectangle::expand(const float &width, const float &height){
    TopLeft.x-=width;
    TopLeft.y-=height;
    BottomRight.x+=width;
    BottomRight.y+=height;
    set_absolute_positions();
}

float Rectangle::get_width(){
    return fabs(BottomRight.x - TopLeft.x);
}

float Rectangle::get_height(){
    return fabs(BottomRight.y - TopLeft.y);
}

void Rectangle::set_absolute_positions(){
    AbsTopLeft.x = Center.x + TopLeft.x;
    AbsTopLeft.y = Center.y + TopLeft.y;
    AbsBottomRight.x = Center.x + BottomRight.x;
    AbsBottomRight.y = Center.y + BottomRight.y;
}

const Point *Rectangle::get_top_left_point(bool absolute_position) const{
    if(absolute_position){
        return &AbsTopLeft;
    }

    return &TopLeft;
}

const Point *Rectangle::get_bottom_right_point(bool absolute_position) const{
    if(absolute_position){
        return &AbsBottomRight;
    }

    return &BottomRight;
}

void Rectangle::normalize(){
    if(TopLeft.x > BottomRight.x){
        const float tmp = TopLeft.x;
        TopLeft.x = BottomRight.x;
        BottomRight.x = tmp;
    }
    if(TopLeft.y > BottomRight.y){
        const float tmp = TopLeft.y;
        TopLeft.y = BottomRight.y;
        BottomRight.y = tmp;
    }
}
