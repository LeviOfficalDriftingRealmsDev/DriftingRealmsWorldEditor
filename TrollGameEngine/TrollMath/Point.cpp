#include "Point.hpp"

using namespace TMath;

PolarPoint::PolarPoint(){
    a=0;
    m=0;
}

PolarPoint::PolarPoint(const float &angle, const float &magnitude){
    a=angle;
    m=magnitude;
}

PolarPoint::PolarPoint(const PolarPoint *pcopy){
    a=pcopy->a;
    m=pcopy->m;
}

PolarPoint::~PolarPoint(){
    ;
}

void PolarPoint::set_point(const PolarPoint *pcopy){
    a=pcopy->a;
    m=pcopy->m;
}

//!

Point::Point(){
    x=0;
    y=0;
}

Point::Point(const float &nx, const float &ny){
    x=nx;
    y=ny;
}

Point::Point(const Point *pcopy){
    x=pcopy->x;
    y=pcopy->y;
}

Point::~Point(){
    ;
}

bool Point::equal(const TMath::Point *compare) const{
    if( (this->x==compare->x) && (this->y==compare->y) ){
        return true;
    }
    return false;
}

bool Point::operator==(const Point &other) const{
    return x == other.x && y == other.y;
}
void Point::divide(const float &denominator){
    x = x / denominator;
    y = y / denominator;
}

void Point::divide(const float &denominator_x, const float &denominator_y){
    x = x / denominator_x;
    y = y / denominator_y;
}

void Point::multiply(const float &factor){
    x = x * factor;
    y = y * factor;
}

void Point::multiply(const float &factor_x, const float &factor_y){
    x = x * factor_x;
    y = y * factor_y;
}

void Point::translate(const float &x, const float &y){
    this->x+=x;
    this->y+=y;
}

void Point::translate(const Point *move_point){
    this->x+=move_point->x;
    this->y+=move_point->y;
}

void Point::translate_am(const float &angle, const float &magnitude){
    this->x+=cos(angle) * magnitude;
    this->y+=sin(angle) * magnitude;
}

void Point::translate_am(const PolarPoint *point){
    this->x+=cos(point->a) * point->m;
    this->y+=sin(point->a) * point->m;
}

void Point::set_point(const Point *pcopy){
    x=pcopy->x;
    y=pcopy->y;
}

void Point::set_point(const float &nx, const float &ny){
    x=nx;
    y=ny;
}

float Point::get_angle_to_point(const Point *other_point) const{

    float dx = this->x - other_point->x;
    float dy = this->y - other_point->y;


    if( (dx == 0) && (dy == 0) ){
        return 0.0f;
    }

    return TrigOps::normalize_angle( atan2(dy,dx) );
}

float Point::get_distance_to_point(const Point *other_point) const{

    const float x_d = (this->x - other_point->x);
    const float y_d = (this->y - other_point->y);

    return sqrt( (x_d*x_d) + (y_d*y_d) );
}

float Point::get_distance_to_point_sq(const Point *other_point) const{

    const float x_d = fabs(this->x - other_point->x);
    const float y_d = fabs(this->y - other_point->y);

    return ( (x_d*x_d) + (y_d*y_d) );
}

void Point::set_rotation_around_point(const float &new_rotation, const Point *origin){

    const float magnitude = get_distance_to_point(origin);

    x = (origin->x + (cos(new_rotation)) * magnitude);
    y = (origin->y + (sin(new_rotation)) * magnitude);
}

void Point::add_rotation_around_point(const float &add_rotation, const Point *origin){

    const float new_rotation = add_rotation + get_angle_to_point(origin);

    set_rotation_around_point( new_rotation , origin );
}

bool Point::is_a_closer_then_b(const Point *a, const Point *b){

    if(get_distance_to_point_sq(a) < get_distance_to_point_sq(b)){
        return true;
    }

    return false;
}
