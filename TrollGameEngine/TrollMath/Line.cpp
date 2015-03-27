#include "Line.hpp"

using namespace TMath;

Line::Line(){
    A.x = 0;
    A.y = 0;
    B.x = 0;
    B.y = 0;
}

Line::Line(const float &x1, const float &y1, const float &x2, const float &y2){
    A.x = x1;
    A.y = y1;
    B.x = x2;
    B.y = y2;
}

Line::Line(const Point *point_a, const Point *point_b){
    A.x = point_a->x;
    A.y = point_a->y;
    B.x = point_b->x;
    B.y = point_b->y;
}

Line::Line(const Line *pcopy){
    A.x = pcopy->A.x;
    A.y = pcopy->A.y;
    B.x = pcopy->B.x;
    B.y = pcopy->B.y;
}

void Line::set_a(const Point *new_a){
    A.x = new_a->x;
    A.y = new_a->y;
}

void Line::set_a(const float &x, const float &y){
    A.x = x;
    A.y = y;
}

void Line::set_b(const Point *new_b){
    B.x = new_b->x;
    B.y = new_b->y;
}

void Line::set_b(const float &x, const float &y){
    B.x = x;
    B.y = y;
}

void Line::translate_a(const float &x, const float &y){
    A.x += x;
    A.y += y;
}

void Line::translate_b(const float &x, const float &y){
    B.x += x;
    B.y += y;
}

void Line::translate(const Point *move_both){
    A.x += move_both->x;
    A.y += move_both->y;
    B.x += move_both->x;
    B.y += move_both->y;
}

void Line::translate(const float &move_both_x, const float &move_both_y){
    A.x += move_both_x;
    A.y += move_both_y;
    B.x += move_both_x;
    B.y += move_both_y;
}

Line* Line::make_line_projection(const Point *p, bool confine_to_segment) const{

    const float angle_of_line = get_angle();
    const float dist_to_point = A.get_distance_to_point(p);
    const float angle_to_point = A.get_angle_to_point(p);
    const float angle_to_projection = ( angle_of_line - PI_2 );
    const float dist_to_projection  = sin( angle_to_point - angle_of_line ) * dist_to_point;

    Line *projection_line = new Line();

    projection_line->A.x = p->x;
    projection_line->A.y = p->y;

    projection_line->B.x = p->x - (cos(angle_to_projection) * dist_to_projection);
    projection_line->B.y = p->y - (sin(angle_to_projection) * dist_to_projection);

    if(!is_point_on_line(&projection_line->B) && confine_to_segment ){
        return 0;
    }

    return projection_line;
}

Point* Line::make_line_projection_point(const Point *p, bool confine_to_segment) const{

    const float angle_of_line = get_angle();
    const float dist_to_point = A.get_distance_to_point(p);
    const float angle_to_point = A.get_angle_to_point(p);
    const float angle_to_projection = ( angle_of_line - PI_2 );
    const float dist_to_projection  = sin( angle_to_point - angle_of_line ) * dist_to_point;

    Point *projection_point = new Point();

    projection_point->x = p->x - (cos(angle_to_projection) * dist_to_projection);
    projection_point->y = p->y - (sin(angle_to_projection) * dist_to_projection);

    if(!is_point_on_line(projection_point) && confine_to_segment){
        return 0;
    }

    return projection_point;
}

//! Sets the points in this box so that they conform to the shape that is passed, making a bounding box
void Line::make_bounding_box(Rectangle *aabb, const float &padding) const{

    const Point *line_center = get_center();
    aabb->set_center_position(line_center);
    delete line_center;

    aabb->set_top_left( get_a()->x - aabb->get_center_position()->x, get_a()->y - aabb->get_center_position()->y);
    aabb->set_bottom_right( get_b()->x - aabb->get_center_position()->x, get_b()->y - aabb->get_center_position()->y);

    aabb->expand(padding);
    aabb->normalize();
}


bool Line::raycast(const Line *ray, Point *collision_point) const{

    const Line *test_line_a = this;
    const Line *test_line_b = ray;

    const float m1 = test_line_a->get_slope();
    const float m2 = test_line_b->get_slope();
    const bool a_and_b_are_parallel = (fabs(m1 - m2) == 0.001f);

    const float yi1 = test_line_a->get_y_intercept();
    const float yi2 = test_line_b->get_y_intercept();

    //! the lines are perpendicular
    if( test_line_a->is_horizontal() && test_line_b->is_vertical() ){

        collision_point->x = test_line_b->get_a()->x;
        collision_point->y = test_line_a->get_a()->y;
    }

    //! the lines are perpendicular
    else if(test_line_a->is_vertical() && test_line_b->is_horizontal()){
        collision_point->x = test_line_a->get_a()->x;
        collision_point->y = test_line_b->get_a()->y;
    }

    //! make sure that the lines are not parallel, even if they are parallel and in the same space they will have infinite points
    //! to return so in all practicality there is no point in returning true.
    else if( a_and_b_are_parallel ){
        return false;
    }

    //! line a is vertical so we need to get the y value from b
    else if( test_line_a->is_vertical() ){
        collision_point->x = test_line_a->get_a()->x;
        collision_point->y = test_line_b->get_y_at(collision_point->x);
    }

    //! line b is vertical so we need to get the y value from a
    else if( test_line_b->is_vertical() ){
        collision_point->x = test_line_b->get_a()->x;
        collision_point->y = test_line_a->get_y_at(collision_point->x);
    }

    //! line a is hortizonal so we need to get the x value from b
    else if( test_line_a->is_horizontal() ){
        collision_point->y = test_line_a->get_a()->y;
        collision_point->x = test_line_b->get_x_at(collision_point->y);
    }

    //! line b is hortizonal so we need to get the x value from a
    else if( test_line_b->is_horizontal() ){
        collision_point->y = test_line_b->get_a()->y;
        collision_point->x = test_line_a->get_x_at(collision_point->y);
    }

    //! both of the lines have a defined slope and are not the same therefore we do not need to worry about division by zero or infinity
    else{
        collision_point->x = ((yi2-yi1) / (m1-m2));
        collision_point->y = test_line_a->get_y_at(collision_point->x);
    }

    //! now that we have the points of intersection find if they are actually on the line segments
    if( test_line_a->is_in_bounding_box(collision_point) && test_line_b->is_in_bounding_box(collision_point) ){
        return true;
    }

    return false;
}


void Line::normalize(){
    if( (A.x > B.x) && (A.y > B.y) ){
        const float tmpx = A.x;
        const float tmpy = A.y;
        A.x=B.x;
        A.y=B.y;
        B.x=tmpx;
        B.y=tmpy;
    }
}

bool Line::is_in_bounding_box(const Point *p) const{

    const bool within_lr = (p->x >= get_left()) && (p->x <= get_right());
    const bool within_tb = (p->y >= get_top()) && (p->y <= get_bottom());

    if( within_lr && within_tb ){
        return true;
    }

    return false;
}

bool Line::is_point_on_line(const Point *p) const{

    if(((p->x == A.x) && (p->y == A.y)) || ((p->x == B.x) && (p->y == B.y))){
        return true;
    }

    const float compare_slope = ( (p->y - A.y) / ( p->x - A.x) );

    bool slopes_are_close = false;

    if( fabs(compare_slope - get_slope()) < 0.001f ){
        slopes_are_close = true;
    }

    if( isinf(compare_slope) && isinf(get_slope())){
        slopes_are_close = true;
    }

    if(slopes_are_close){
        if(is_in_bounding_box(p)){
            return true;
        }
    }

    return false;
}






float Line::get_x_at(const float &y) const{
    return ( (y - A.y ) / (get_slope()) ) + A.x;
}

float Line::get_y_at(const float &x) const{
    return ( get_slope() * ( x - A.x ) ) + A.y;
}

float Line::get_x_intercept() const{
    return get_x_at(0);
}

float Line::get_y_intercept() const{
    return get_y_at(0);
}

float Line::get_top() const{
    if(A.y < B.y){
        return A.y;
    }else{
        return B.y;
    }
}

float Line::get_right() const{
    if(A.x > B.x){
        return A.x;
    }else{
        return B.x;
    }
}

float Line::get_bottom() const{
    if(A.y > B.y){
        return A.y;
    }else{
        return B.y;
    }
}

float Line::get_left() const{
    if(A.x < B.x){
        return A.x;
    }else{
        return B.x;
    }
}

float Line::get_angle(const bool a_to_b) const{
    if(a_to_b){
        return A.get_angle_to_point(&B);
    }else{
        return B.get_angle_to_point(&A);
    }
}

float Line::get_slope() const{
    return (B.y - A.y) / ( B.x - A.x);
}

bool Line::is_horizontal() const{

    if(get_slope() == 0){
        return true;
    }
    else{
        return false;
    }
}

bool Line::is_vertical() const{

    if(isinf(get_slope())){
        return true;
    }
    else{
        return false;
    }
}

float Line::get_length() const{
    return A.get_distance_to_point(&B);
}

float Line::get_length_sq() const{
    return A.get_distance_to_point_sq(&B);
}

const Point* Line::get_a() const{
    return &A;
}

const Point* Line::get_b() const{
    return &B;
}

Point* Line::get_center() const{
    return new Point( (B.x + A.x) / 2.0f , (B.y + A.y) / 2.0f );
}
