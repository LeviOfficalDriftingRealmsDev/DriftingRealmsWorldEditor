#include "MathFunctions.hpp"

using namespace TMath;

/// An implementation of the law of cosines
float TrigOps::Get_A_From_SSS(float a, float b, float c){
    return acos( ( (b*b) + (c*c) - (a*a) ) / (2*b*c) );
}

float TrigOps::Get_B_From_SSS(float a, float b, float c){
    return acos( ( (c*c) + (a*a) - (b*b) ) / (2*c*a) );
}

float TrigOps::Get_C_From_SSS(float a, float b, float c){
    return acos( ( (a*a) + (b*b) - (c*c) ) / (2*a*b) );
}

float TrigOps::RadianToDegree(float r){
    return (r*(180.0f/TMath::PI));
}

float TrigOps::normalize_angle(float a){

    float p2 = (2.0f*PI);

    while ( a >= p2 ){
        a -= p2;
    }

    while ( a < 0 ){
        a += p2;
    }

    return a;
}

float TrigOps::normalize_angle_pi(float a){

    while ( a >= PI ){
        a -= PI;
    }

    while ( a < 0 ){
        a += PI;
    }

    return a;
}

/*
Point* TrigOps::polar_grid_to_cartesian(float t, float m){

    const float new_x = cos(t)*m;
    const float new_y = sin(t)*m;

    return new Point(new_x,new_y);
}

PolarPoint* TrigOps::cartesian_to_polar_grid(float x, float y){

    const float theta = atan( y / x );
    const float magnitude = sqrt( (x*x) + (y*y) );

    return new PolarPoint(theta,magnitude);
}
*/

        //! Note that these methods call new when returning,
        //! so make sure to delete when done with the pointers
//        static Point* polar_grid_to_cartesian(float t, float m);
//        static PolarPoint* cartesian_to_polar_grid(float x, float y);

//! add the above to the point classes
