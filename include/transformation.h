/**
* @file transformation.h
* @brief Definition of Transformation class
* Created:       21/04/2019
* Last Modified: 21/04/2019
* Copyright © 2019 Vitor Godeiro. 
**/

#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_

#include "mat4.h"
#include <math.h>       


/**
* @class transformation
* @ brief 
*/
class Transformation {
    public:
        static Mat4 translation(float dx, float dy, float dz){
            return Mat4(1, 0, 0, dx, 
                        0, 1, 0, dy, 
                        0, 0, 1, dz, 
                        0, 0, 0, 1);
        }

        static Mat4 scaling(float sx, float sy, float sz){
            return Mat4(sx, 0, 0, 0, 
                        0, sy, 0, 0, 
                        0, 0, sz, 0, 
                        0, 0, 0, 1);
        }

        static Mat4 rotationX(float a){
            return Mat4(1, 0, 0, 0,
                        0, cos(a), sin(a), 0,
                        0, sin(a), cos(a), 0, 
                        0, 0, 0, 1);
        }

        static Mat4 rotationY(float a){
            return Mat4(cos(a), 0, sin(a), 0,
                        0, 1, 0, 0,
                        -sin(a), 0, cos(a), 0, 
                        0, 0, 0, 1);
        }

        static Mat4 rotationZ(float a){
            return Mat4(cos(a), -sin(a), 0, 0,
                        sin(a), cos(a), 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1);
        }

};

#endif
