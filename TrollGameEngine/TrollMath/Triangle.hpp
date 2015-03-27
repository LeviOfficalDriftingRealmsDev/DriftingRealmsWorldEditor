#pragma once

#include "Shape.hpp"
#include "Point.hpp"

namespace TMath{

    class Triangle : public Shape{

        public:
            enum POINTS{
                POINT_A,
                POINT_B,
                POINT_C
            };

            Triangle();
            ~Triangle();
            Triangle(Triangle *copy_me);
            Triangle(Point *a, Point *b, Point *c, Point *start_point);

            float get_width();
            float get_height();

            //! ONLY RETURNS THE RELATIVE POINT!
            POINTS get_top();
            POINTS get_right();
            POINTS get_bottom();
            POINTS get_left();

            void translate(float move_x, float move_y);
            void set_angle(const float &new_angle);
            void set_angle(const float &new_angle, Point *origin);

            //! set the point, by default when being called only the relative position will be set
            //! when translate is set to true absolute becomes irrelevant.
            void set_a(const float &x, const float &y, bool absolute=false, bool translate=false);
            void set_b(const float &x, const float &y, bool absolute=false, bool translate=false);
            void set_c(const float &x, const float &y, bool absolute=false, bool translate=false);

            float get_abs_x( POINTS abc );
            float get_abs_y( POINTS abc );

            Point get_abs_point( POINTS abc );

            Point *get_a();
            Point *get_b();
            Point *get_c();

            Point *get_point( POINTS abc );

        private:
            Point a;
            Point b;
            Point c;
            float angle;
    };
};
