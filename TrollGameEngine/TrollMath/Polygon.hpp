#pragma once

#include "Point.hpp"
#include "Shape.hpp"
#include "Line.hpp"
#include "Circle.hpp"

#include <iostream>
#include <vector>

namespace TMath{

    struct RaycastPolygonOutputData{
        bool fast=false;
        bool did_hit=false;
        std::vector<Point> hit_locations;
        std::vector<unsigned int> hit_indexes;
    };

    class Polygon : public Shape{

        public:

            //! The Constructors
            //! These two constructors will make a copy of the point list. The first and last points MUST be the same!
            Polygon(const float &center_x, const float &center_y, const Point *new_list, const unsigned int point_count, const bool ltr_winding=true);
            Polygon(const Point *center, const Point *new_list, const unsigned int point_count, const bool ltr_winding=true);

            //! the last paramater controls wether or not a copy is made for the list of points in the geometry
            //! This is practical in case you want to make an array of shapes with differnt positions but with the same internal geometry.
            //! Using this you can simply move one position in the shared pointer list and move the coorisponding point in all of the polygons

            /*Polygon(const float &center_x, const float &center_y, Polygon *shape_copy, bool shadow=true);
            Polygon(const Point *center, Polygon *shape_copy, bool shadow=true);*/

            virtual ~Polygon();

            void set_center_position(const float &new_x, const float &new_y);
            //! Modifiers
            //! When you move polygon these the relative positions will remain unchanged, however the rotation will change.

            void translate(const float &move_x, const float &move_y);
            void translate(const Point* movement);

            void set_position(const float &new_x, const float &new_y);
            void set_position(const Point* new_pos);

            void translate_point(const unsigned int i, const float &x, const float &y);
            void translate_point(const unsigned int i, const Point *movement);

            void set_point(const unsigned int i, const float &x, const float &y, bool absolute_position=false);
            void set_point(const unsigned int i, const Point *position, bool absolute_position=false);

            // add add_last_point(), insert_point(), remove_point scale(),

            void add_rotation(float add_angle);
            void set_rotation(float set_angle);

            //! Accesors.
            float get_width();
            float get_height();

            //void set_vertex_groups

            //! retuns a non const pointer, make sure to delete it later!
            Line *make_line(const unsigned int i, bool absolute_positions=false) const;

            //! creates an aabb from the polygon
            void make_bounding_box(Rectangle *aabb, const float &padding=0) const;

            //! The fast paramater if set to true will instruct the method to return true on the first found intersection
            bool raycast(const Line *ray, RaycastPolygonOutputData *output, bool fast) const;
            bool is_point_inside(const Point *p) const;

            //bool does_collide(const Polygon *test_polygon, RaycastPolygonOutputData *output) const;
            bool does_collide(const Circle *test_circle, RaycastPolygonOutputData *output) const;

            bool collide_polygon(const Line *ray, RaycastPolygonOutputData *output, bool fast) const;

            //! returns a pointer to a point in the polygon
            const Point *get_point(const unsigned int i, bool absolute_positions=false) const;
            const Point *get_point_no_bounds(const int i, bool absolute_positions=false) const;

            unsigned int get_amount_of_points() const;
            unsigned int get_amount_of_lines() const;

            //! used to create the dimesions of the polygon
            const Point *get_top_point(bool get_absolute=true) const;
            const Point *get_right_point(bool get_absolute=true) const;
            const Point *get_bottom_point(bool get_absolute=true) const;
            const Point *get_left_point(bool get_absolute=true) const;

            const bool is_winding_left_to_right() const;

        private:
            //! These are just helper functions that set the absolute positions of the points from the relative points and angles.
            void inline compute_internal_positions();
            void inline compute_internal_position(const unsigned int i);
            void inline compute_internal_angles();
            void inline compute_internal_angle(const unsigned int i);

            bool inline is_index_valid(const unsigned int i);
            //! If you want a closed shape then make sure that the first point is equal to the last point
            //! When you acces the amount of points on a closed polygon you will apear to get one more point
            //! Then is actualy on the polygon, this is becuase the first and last point are part of the
            //! polygon shape. The boolean paramater is simply record wether or not the shape is closed.
            void inline set_points(const Point *new_list, const unsigned int point_count, bool is_shape_closed);
        private:
            bool winding_left_to_right=false;
            mutable float recompute_width=true, recompute_height=true;
            Polygon *base_shape=0;
            unsigned int amount_of_points=0;
            //! For optimization the magnitudes and angles of each point will be stored in an array.
            //! When moving the origin this will trigger a recomputation of these variables
            float *angle_list=0, *magnitude_list=0;
            float angle=0, angle_about_origin=0;
            mutable float width=0, height=0;
            Point *point_list_rel=0, *point_list_abs=0;

        //! Possible methods to add
        // get left, right, bottom, left
        // scale

    };
};
