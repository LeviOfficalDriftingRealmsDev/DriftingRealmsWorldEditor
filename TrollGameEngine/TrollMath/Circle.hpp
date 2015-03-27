#pragma once

#include "Shape.hpp"
#include "Line.hpp"
#include "Rectangle.hpp"

namespace TMath{

    struct RaycastCircleOutputData{

        bool hit_in_segment=false;
        bool a_hit=false;
        bool b_hit=false;
        float chord_length=0.0f;

        Point *collision_point_a=0;
        Point *collision_point_b=0;
        Line *projection=0;
    };

    class Circle : public Shape{

        public:
            Circle();
            Circle( const float &x, const float &y, const float &start_radius);
            Circle( const Point *shape_center, const float &start_radius);
            ~Circle();
            bool raycast(const Line *ray, RaycastCircleOutputData *output ) const;
            //! modifiers
            void set_radius(const float &r);

            //! accesors
            float get_width();
            float get_height();
            float get_radius() const;
        private:
            float radius;
    };
};
