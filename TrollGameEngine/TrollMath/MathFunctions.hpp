#pragma once

//#include "Point.hpp"

#include <math.h>
#include <stdlib.h>
#include "MathConstants.hpp"

namespace TMath{

    struct TrigOps{
        /// An implementation of the law of cosines
        static float Get_A_From_SSS(float a, float b, float c);
        static float Get_B_From_SSS(float a, float b, float c);
        static float Get_C_From_SSS(float a, float b, float c);
        static float RadianToDegree(float r);
        static float normalize_angle(float a);
        static float normalize_angle_pi(float a);
    };

    //! remake the random functions

    static float GetRand(unsigned int maximum, unsigned int seed=0){
        //srand(seed);
        return ( ((rand() % (maximum*10000) ) + 1) / 10000.0f );
    }

    static int GetRandInt(unsigned int maximum, unsigned int seed=0){
        if(seed){
            srand(seed);
        }
        if(maximum){
            return((rand() % (maximum)) + 1);
        }
        return 0;
    }

    //constexpr bool random_bool_list[64] = {0,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,1};


};
