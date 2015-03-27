#pragma once

#include "Point.hpp"

namespace TMath{

    class Shape{

        public:
            //! define the different shape types
            enum SHAPE_TYPES{CIRCLE,ELLIPSE,TRIANGLE,RECTANGLE,POLYGON,UNDEFINED};
            static unsigned int current_id;

            Shape();
            Shape(const float &start_x, const float &start_y, SHAPE_TYPES shape_type);
            Shape(const Point *shape_center, SHAPE_TYPES shape_type);
            virtual ~Shape();

            //! modifiers
            //! once a shape has been made, setting the center position will move all of the points in the shape
            virtual void set_center_position(const float &new_x, const float &new_y);
            virtual void set_origin_position(const float &new_x, const float &new_y);

            //! accesors
            const Point *get_center_position() const;
            const Point *get_origin_position() const;
            unsigned int get_id() const;
            SHAPE_TYPES get_type() const;

            //! pure virtual accesors
            virtual float get_width()=0;
            virtual float get_height()=0;

            Point *get_absolute_point(Point *relative_position);

        protected:
            unsigned int id;
            //! the center position of a shape is where all of the points will be added to it, not the top left.
            //! the origin starts at (0,0) and is the point that all other points in the shape are rotated around.
            Point Center, Origin;
            SHAPE_TYPES type=SHAPE_TYPES::UNDEFINED;
    };
};
