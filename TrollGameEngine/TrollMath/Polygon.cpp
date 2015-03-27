
#include "Polygon.hpp"

using namespace TMath;

Polygon::Polygon(const float &center_x, const float &center_y, const Point *new_list, const unsigned int point_count, const bool ltr_winding) : Shape(center_x,center_y,SHAPE_TYPES::POLYGON){

    amount_of_points=point_count;

    point_list_rel = new Point[amount_of_points];
    point_list_abs = new Point[amount_of_points];

    angle_list = new float[amount_of_points];
    magnitude_list = new float[amount_of_points];

    for(unsigned int i=0; i<amount_of_points; ++i){
        point_list_rel[i].x = new_list[i].x;
        point_list_rel[i].y = new_list[i].y;
    }

    compute_internal_positions();
    compute_internal_angles();
    winding_left_to_right = ltr_winding;
}

Polygon::Polygon(const Point *center, const Point *new_list, const unsigned int point_count, const bool ltr_winding) : Shape(center,SHAPE_TYPES::POLYGON){

    amount_of_points=point_count;

    point_list_rel = new Point[point_count];
    point_list_abs = new Point[point_count];

    angle_list = new float[amount_of_points];
    magnitude_list = new float[amount_of_points];

    for(unsigned int i=0; i<point_count; ++i){
        point_list_rel[i].x = new_list[i].x;
        point_list_rel[i].y = new_list[i].y;
    }

    compute_internal_positions();
    compute_internal_angles();
    winding_left_to_right = ltr_winding;
}

Polygon::~Polygon(){
    ;
}

void Polygon::set_center_position(const float &new_x, const float &new_y){
    Shape::set_center_position(new_x,new_y);
    compute_internal_positions();
}

//! add shadow feature later

/*Polygon::Polygon(const float &center_x, const float &center_y, Polygon *shape_copy, bool shadow) : Shape::Shape(center_x,center_y,SHAPE_TYPES::POLYGON){

    is_shadow = shadow;

    if(is_shadow){
        base_shape=shape_copy;
        amount_of_points = shape_copy->get_amount_of_points();

        for(unsigned int i=0; i<amount_of_points; ++i){
            point_list_abs[i].x = Center.x + base_shape->point_list_rel[i].x;
            point_list_abs[i].y = Center.y + base_shape->point_list_rel[i].y;
        }

    }
    else{
        set_points(shape_copy->point_list_rel, shape_copy->amount_of_points, shape_copy->is_closed );
    }
}

Polygon::Polygon(const Point *center, Polygon *shape_copy, bool shadow) : Shape::Shape(center,SHAPE_TYPES::POLYGON){

    is_shadow = shadow;

    if(is_shadow){
        base_shape=shape_copy;
        amount_of_points = shape_copy->get_amount_of_points();
    }
    else{
        set_points(shape_copy->point_list_rel, shape_copy->amount_of_points, shape_copy->is_closed );
    }
}
*/




const Point* Polygon::get_point(const unsigned int i, bool absolute_positions) const{

    const bool is_i_in_range = ( i < amount_of_points );

    if(absolute_positions && is_i_in_range ){
        return &point_list_abs[i];
    }

    else if(!absolute_positions && is_i_in_range){

        return &point_list_rel[i];

    }

    else{
        return 0;
    }

}

const Point* Polygon::get_point_no_bounds(const int i, bool absolute_positions) const{

    int real_index = i;

    while(real_index > (int)amount_of_points){
        real_index -= amount_of_points;
    }

    while(real_index < 0){
        real_index += (int)amount_of_points;
    }

    if(absolute_positions ){
        return &point_list_abs[real_index];
    }

    else{
        return &point_list_rel[real_index];
    }
}

Line* Polygon::make_line(unsigned int i, bool absolute_positions) const{

    const bool is_i_in_range = ( (i+1) < amount_of_points );

    if(is_i_in_range){

        if(absolute_positions ){
            return new Line( &point_list_abs[i], &point_list_abs[i+1] );
        }

        else{
            return new Line( &point_list_rel[i], &point_list_rel[i+1] );
        }
    }
    else{
        return 0;
    }
}

void Polygon::make_bounding_box(Rectangle *aabb, const float &padding) const{
    aabb->set_center_position(get_center_position());
    aabb->set_top_left(get_left_point(false)->x,get_top_point(false)->y);
    aabb->set_bottom_right(get_right_point(false)->x,get_bottom_point(false)->x);
}

bool Polygon::raycast(const Line *ray, RaycastPolygonOutputData *output, bool fast) const{

    const Polygon *test_polygon = this;

    bool did_hit=false;

    const unsigned int amount_of_lines = (test_polygon->get_amount_of_points()-1);

    for(unsigned int i=0; i<amount_of_lines; ++i){

        TMath::Line *current_polygon_line = test_polygon->make_line(i,true);

        Point *current_point = new Point();

        if(ray->raycast(current_polygon_line, current_point)){
            output->hit_locations.push_back(current_point);
            output->hit_indexes.push_back(i);
            did_hit=true;
        }

        else{
            delete current_point;
        }

        delete current_polygon_line;

        if(did_hit){
            output->did_hit=true;
        }

        if(fast && did_hit){
            return true;
        }
    }

    return output->did_hit;
}

bool Polygon::is_point_inside(const Point *p) const{

    const Polygon *test_polygon = this;

    const unsigned int amount_of_lines = (test_polygon->get_amount_of_points()-1);

    int amout_of_hits = 0;

    //! test for and handle 'double' collisions at the vertex positions
    for(unsigned int i=0; i<test_polygon->get_amount_of_points(); ++i){
        if(p->y == point_list_abs[i].y && p->x > point_list_abs[i].x){

            int prev_index = i-1;
            int next_index = i+1;

            if(i==0){
                prev_index = amount_of_lines;
            }

            if( point_list_abs[prev_index].y > p->y ){
                ++amout_of_hits;
            }

            if( point_list_abs[next_index].y > p->y ){
                ++amout_of_hits;
            }
        }
    }

    Line ray(-p->x,p->y,p->x,p->y);

    for(unsigned int i=0; i<amount_of_lines; ++i){

        TMath::Line *cpl = test_polygon->make_line(i,true);

        Point *current_point = new Point();

        if(ray.raycast(cpl, current_point)){
            ++amout_of_hits;
        }

        delete current_point;
        delete cpl;
    }

    const bool inside = (amout_of_hits % 2 != 0);

    if(inside){
        return true;
    }

    return false;
}

/*bool Polygon::does_collide(const Polygon *test_polygon, RaycastPolygonOutputData *output) const{
    ;//finish me!
}*/

bool Polygon::does_collide(const Circle *test_circle, RaycastPolygonOutputData *output) const{

    //! check to see if the center is inside the polygon
    if(is_point_inside(test_circle->get_center_position())){
        ;//return true; // add this to the fast version
    }

    //! the circle is not inside so perform the raycasts
    for( unsigned int i=0; i<get_amount_of_lines(); ++i ){

        TMath::Line *cpl = make_line(i,true);

        RaycastCircleOutputData *out = new RaycastCircleOutputData;

        if(test_circle->raycast(cpl,out)){

            if(out->a_hit){
                output->hit_locations.push_back(out->collision_point_a);
                output->hit_indexes.push_back(i);
            }
            else{
                delete out->collision_point_a;
            }

            if(out->b_hit){
                output->hit_locations.push_back(out->collision_point_b);
                output->hit_indexes.push_back(i);
            }
            else{
                delete out->collision_point_b;
            }

        }
        delete out->projection;
        delete cpl;
    }

    if(output->hit_locations.size()){
        output->did_hit=true;
        return true;
    }

    return false;
}

unsigned int Polygon::get_amount_of_points() const{
    return amount_of_points;
}

unsigned int Polygon::get_amount_of_lines() const{
    return amount_of_points-1;
}

void Polygon::translate(const float &move_x, const float &move_y){
    Center.x += move_x;
    Center.y += move_y;
    compute_internal_positions();
}

void Polygon::translate(const Point* movement){
    Center.x += movement->x;
    Center.y += movement->y;
    compute_internal_positions();
}

void Polygon::set_position(const float &new_x, const float &new_y){
    Center.x = new_x;
    Center.y = new_y;
    compute_internal_positions();
}

void Polygon::set_position(const Point* new_pos){
    Center.x = new_pos->x;
    Center.y = new_pos->y;
    compute_internal_positions();
}

void Polygon::translate_point(const unsigned int i, const float &x, const float &y){

    recompute_width = true;
    recompute_height = true;

    if(is_index_valid(i)){
        point_list_rel[i].x += x;
        point_list_rel[i].y += y;
        compute_internal_angle(i);
        compute_internal_position(i);
    }
}

void Polygon::translate_point(const unsigned int i, const Point *movement){

    recompute_width = true;
    recompute_height = true;

    if(is_index_valid(i)){
        point_list_rel[i].x += movement->x;
        point_list_rel[i].y += movement->y;
        compute_internal_position(i);
        compute_internal_angle(i);
    }
}

void Polygon::set_point(const unsigned int i, const float &x, const float &y, bool absolute_position){

    recompute_width = true;
    recompute_height = true;

    if(is_index_valid(i)){
        point_list_rel[i].x = x;
        point_list_rel[i].y = y;
        compute_internal_position(i);
        compute_internal_angle(i);
    }
}

void Polygon::set_point(const unsigned int i, const Point *position, bool absolute_position){

    recompute_width = true;
    recompute_height = true;

    if(is_index_valid(i)){
        point_list_rel[i].x = position->x;
        point_list_rel[i].y = position->y;
        compute_internal_position(i);
        compute_internal_angle(i);
    }
}

void Polygon::add_rotation(float add_angle){

    recompute_width = true;
    recompute_height = true;

    angle = TrigOps::normalize_angle( angle + add_angle );

    for(unsigned int i=0; i<amount_of_points; ++i){
        const float theta = angle_list[i] + angle;
        const float mag = magnitude_list[i];
        point_list_rel[i].x = (cos(theta)) * mag;
        point_list_rel[i].y = (sin(theta)) * mag;
    }

    compute_internal_positions();
}

void Polygon::set_rotation(float new_angle){

    recompute_width = true;
    recompute_height = true;

    angle = TrigOps::normalize_angle(new_angle);

    for(unsigned int i=0; i<amount_of_points; ++i){

        const float theta = angle_list[i] + angle;
        const float mag = magnitude_list[i];

        point_list_rel[i].x = (cos(theta)) * mag;
        point_list_rel[i].y = (sin(theta)) * mag;
    }

    compute_internal_positions();
}


bool Polygon::is_index_valid(const unsigned int i){
    if( (i>=0) && (i<amount_of_points) ){
        return true;
    }
    return false;
}


const Point* Polygon::get_top_point(bool get_absolute) const{

    unsigned int ci=0; // = compare_index

    for(unsigned int i=0; i<amount_of_points; ++i){

        if( point_list_rel[i].y <= point_list_rel[ci].y ){
            ci = i;
        }

    }

    if(get_absolute){
        return &point_list_abs[ci];
    }

    else{
        return &point_list_rel[ci];
    }
}

const Point* Polygon::get_right_point(bool get_absolute) const{

    unsigned int ci=0; // = compare_index

    for(unsigned int i=0; i<amount_of_points; ++i){

        if( point_list_rel[i].x >= point_list_rel[ci].x ){
            ci = i;
        }
    }

    if(get_absolute){
        return &point_list_abs[ci];
    }

    else{
        return &point_list_rel[ci];
    }
}

const Point* Polygon::get_bottom_point(bool get_absolute) const{
    unsigned int ci=0; // = compare_index

    for(unsigned int i=0; i<amount_of_points; ++i){

        if( point_list_rel[i].y >= point_list_rel[ci].y ){
            ci = i;
        }

    }

    if(get_absolute){
        return &point_list_abs[ci];
    }

    else{
        return &point_list_rel[ci];
    }
}

const Point* Polygon::get_left_point(bool get_absolute) const{
    unsigned int ci=0; // = compare_index

    for(unsigned int i=0; i<amount_of_points; ++i){

        if( point_list_rel[i].x <= point_list_rel[ci].x ){
            ci = i;
        }
    }

    if(get_absolute){
        return &point_list_abs[ci];
    }

    else{
        return &point_list_rel[ci];
    }
}

float Polygon::get_width(){

    if(recompute_width){
        width = fabs( get_left_point()->x - get_right_point()->x );
        recompute_width = false;
    }

    return width;
}

float Polygon::get_height(){

    if(recompute_height){
        height = fabs(get_bottom_point()->y - get_top_point()->y);
        recompute_height = false;
    }

    return height;
}

//! private inline methods below

// this should be optimzed so that the lines do not recomute their length and slopes every call
void Polygon::compute_internal_positions(){
    // set the positions of the absolute points
    for(unsigned int i=0; i<amount_of_points; ++i){
        point_list_abs[i].x = Center.x + point_list_rel[i].x;
        point_list_abs[i].y = Center.y + point_list_rel[i].y;
    }
}

// this should be optimzed so that the lines do not recomute their length and slopes every call
void Polygon::compute_internal_position(const unsigned int i){
    // set the positions of the absolute points
    if(is_index_valid(i)){
        point_list_abs[i].x = Center.x + point_list_rel[i].x;
        point_list_abs[i].y = Center.y + point_list_rel[i].y;
    }
}

void Polygon::compute_internal_angles(){
    for(unsigned int i=0; i<amount_of_points; ++i){
        angle_list[i] = TrigOps::normalize_angle(point_list_rel[i].get_angle_to_point(&Origin) - angle);
        magnitude_list[i] = point_list_rel[i].get_distance_to_point(&Origin);
    }
}

void Polygon::compute_internal_angle(const unsigned int i){
    if(is_index_valid(i)){
        angle_list[i] = TrigOps::normalize_angle(point_list_rel[i].get_angle_to_point(&Origin) - angle );
        magnitude_list[i] = point_list_rel[i].get_distance_to_point(&Origin);
    }
}

const bool Polygon::is_winding_left_to_right() const{
    return winding_left_to_right;
}
