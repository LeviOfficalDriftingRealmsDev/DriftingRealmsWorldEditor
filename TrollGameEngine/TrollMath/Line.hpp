#pragma once

#include "Point.hpp"
#include "Rectangle.hpp"

#include <iostream>

//! note that line is not a shape, and has no seperate relative and absolute point positions

namespace TMath{

    class Line{

        public:

            Line();
            Line(const float &x1, const float &y1, const float &x2, const float &y2);
            Line(const Point *point_a, const Point *point_b);
            Line(const Line *pcopy);

            void set_a(const Point *new_a);
            void set_a(const float &x, const float &y);
            void set_b(const float &x, const float &y);
            void set_b(const Point *new_b);
            void translate(const Point *move_both);
            void translate(const float &move_both_x, const float &move_both_y);
            void translate_a(const float &x, const float &y);
            void translate_b(const float &x, const float &y);

            //! only returns a pointer if the point is not on the line and if it is the line-segments range returns 0 otherwise
            Line* make_line_projection(const Point *p, bool confine_to_segment=true) const;
            Point* make_line_projection_point(const Point *p, bool confine_to_segment=true) const;

            //! Sets the points in this box so that they conform to the shape that is passed, making a bounding box
            void make_bounding_box(Rectangle *aabb, const float &padding=0) const;

            bool raycast(const Line *ray, Point *collision_point) const;


            const bool is_point_left_of_line(const TMath::Point *check) const{
                 return ((B.x - A.x)*(check->y - A.y) - (B.y - A.y)*(check->x - A.x)) >= 0;
            }


            //! insures that A is to the top left of B
            void normalize();

            bool is_in_bounding_box(const Point *p) const;

            //! test to see if a point is on the line
            bool is_point_on_line(const Point *p) const;


            //! note that these return a value even if they are not within the line segment
            float get_x_at(const float &y) const;
            float get_y_at(const float &x) const;
            float get_x_intercept() const;
            float get_y_intercept() const;

            float get_top() const;
            float get_right() const;
            float get_bottom() const;
            float get_left() const;

            float get_angle(const bool a_to_b=true) const;

            float get_slope() const;

            bool is_horizontal() const;
            bool is_vertical() const;

            float get_length() const;
            float get_length_sq() const;

            const Point* get_a() const;
            const Point* get_b() const;

            Point* get_center() const;

        private:
            Point A, B;
    };
};
