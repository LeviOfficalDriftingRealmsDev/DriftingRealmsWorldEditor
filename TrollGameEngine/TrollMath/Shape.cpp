#include "Shape.hpp"

using namespace TMath;

unsigned int Shape::current_id = 0;

Shape::Shape(){
    current_id++;
    id = current_id;
}

Shape::Shape(const float &start_x, const float &start_y, SHAPE_TYPES shape_type){
    current_id++;
    id = current_id;
    Center = Point(start_x,start_y);
    Origin = Point(0,0);
    type = shape_type;
}

Shape::Shape(const Point *shape_center, SHAPE_TYPES shape_type){
    current_id++;
    id = current_id;
    Center = Point(shape_center);
    Origin = Point(0,0);
    type = shape_type;
}

Shape::~Shape(){
    ;
}

unsigned int Shape::get_id() const{
    return id;
}

void Shape::set_center_position(const float &new_x, const float &new_y){
    Center.x = new_x;
    Center.y = new_y;
}

const Point *Shape::get_center_position() const{
    return &Center;
}

void Shape::set_origin_position(const float &new_x, const float &new_y){
    Origin.x = new_x;
    Origin.y = new_y;
}

const Point *Shape::get_origin_position() const{
    return &Origin;
}

Shape::SHAPE_TYPES Shape::get_type() const{
    return type;
}
