#pragma once

#include "Shape.hpp"
#include "Point.hpp"

//! Rectangle does not support rotation however you can compute the bounding box of a rotated rectangle.
//! If you want to be able to rotate a shape look at the polygon class, and use its set_as_rectangle method

namespace TMath{
    class Rectangle : public Shape{
        public:
            Rectangle();
            //! remeber that the points are added around the center
            Rectangle( const Point *shape_center, const float &x1, const float &y1, const float &x2, const float &y2 );
            Rectangle( const Point *shape_center, const float &width, const float &height );
            Rectangle( const float &x, const float &y, const float &x1, const float &y1, const float &x2, const float &y2 );
            Rectangle( const float &x, const float &y, const float &width, const float &height );
            ~Rectangle();

            //! test to see if a point is inside
            bool is_point_inside(const Point *test_point) const;

            //! method assumes that both rectangles are normalized
            bool does_overlap(const Rectangle* other_rect) const;

            void set_center_position(const float &new_x, const float &new_y);
            void set_center_position(const Point *p);

            void set_bottom_right(const float &new_x, const float &new_y);
            void set_bottom_right(const Point *new_pos);

            void set_top_left(const float &new_x, const float &new_y);
            void set_top_left(const Point *new_pos);

            void expand(const float &amount);
            void expand(const float &width, const float &height);

            float get_width();
            float get_height();

            const Point *get_top_left_point(bool absolute_position=false) const;
            const Point *get_bottom_right_point(bool absolute_position=false) const;

            //! swaps the points if the top left is not the top left
            void normalize();

        private:
            void inline set_absolute_positions();
            Point TopLeft, BottomRight, AbsTopLeft, AbsBottomRight;
             //! add void expand(point)
    };
};
