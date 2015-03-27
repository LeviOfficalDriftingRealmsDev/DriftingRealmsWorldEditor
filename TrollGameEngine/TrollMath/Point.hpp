#pragma once
#include <cmath>
#include "MathConstants.hpp"
#include "MathFunctions.hpp"

namespace TMath{

    class Point;

    //! Stores a point that is described by an angle and a magnitude
    class PolarPoint{

        public:
            PolarPoint();
            PolarPoint(const float &angle, const float &magnitude);
            PolarPoint(const PolarPoint *pcopy);
            ~PolarPoint();

            //! modifiers
            void set_point(const PolarPoint *pcopy);

        public:
            //! where a and m are short for angle and magnitude
            float a=0,m=0;
    };

    class Point{
        public:
            Point();
            Point(const float &nx, const float &ny);
            Point(const Point *pcopy);
            ~Point();
            bool operator==(const Point &other) const;
            //! comparisions
            bool equal(const TMath::Point *compare) const;
            //! oppererators
            void divide(const float &denominator);
            void divide(const float &denominator_x, const float &denominator_y);
            void multiply(const float &factor);
            void multiply(const float &factor_x, const float &factor_y);
            //! same thing as add/subtract
            void translate(const float &x, const float &y);
            void translate(const Point *move_point);
            //! move a point via a vector
            void translate_am(const float &angle, const float &magnitude);
            void translate_am(const PolarPoint *point);

            //! modifiers
            void set_point(const float &nx, const float &ny);
            void set_point(const Point *pcopy);

            float get_angle_to_point(const Point *other_point) const;
            float get_distance_to_point(const Point *other_point) const;
            //! In you have some scenario where calcuating the suare root can be omitted
            float get_distance_to_point_sq(const Point *other_point) const;

            void set_rotation_around_point(const float &new_rotation, const Point *origin);
            void add_rotation_around_point(const float &add_rotation, const Point *origin);

            //! tests two points and returns which true if a closer to this point then b
            bool is_a_closer_then_b(const Point *a, const Point *b);

        public:
            //! there is nothing that can go wrong by directly manipulating these values
            float x=0, y=0;
    };
};
