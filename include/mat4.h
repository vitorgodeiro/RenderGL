/**
* @file mat4.h
* @brief Definition of Matrix 4x4 class
* Created:       21/04/2019
* Last Modified: 21/04/2019
* Copyright © 2019 Vitor Godeiro. 
**/

#ifndef _MAT4_H_
#define _MAT4_H_

#include <cmath>
#include <iostream>
#include <cassert>

/** 
* @class Mat4
* @brief The class that describes an Matrix 4x4
*/
class Mat4
{
    public:
        /** @brief The construtor specifies a vec3 class.*/
        Mat4( float e0_=0.f, float e1_=0.f, float e2_=0.f, float e3_=0.f,
              float e4_=0.f, float e5_=0.f, float e6_=0.f, float e7_=0.f,
              float e8_=0.f, float e9_=0.f, float e10_=0.f, float e11_=0.f,
              float e12_=0.f, float e13_=0.f, float e14_=0.f, float e15_=0.f ): e{ e0_, e1_, e2_, e3_, e4_, e5_, e6_, e7_, e8_, e9_, e10_, e11_, e12_, e13_, e14_, e15_}{ }
        
        inline float operator[] (size_t i) const { return e[i]; }
        inline float& operator[](size_t i) { return e[i]; }

        inline const Mat4& operator+( void ) const { return *this; }
        inline Mat4 operator-( void ) const { return Mat4(-e[0], -e[1], -e[2], -e[3], -e[4], -e[5], -e[6], -e[7], -e[8], -e[9], -e[10], -e[11], -e[12], -e[13], -e[14], -e[15]); }

        inline Mat4& operator+=( const Mat4& v);
        inline Mat4& operator-=( const Mat4& v);
        inline Mat4& operator*=( const Mat4& v);
        inline Mat4& operator*=( const float f);
        inline Mat4& operator/=( const float f);

        

    private:
        float e[16];
};

#include "mat4.inl"

#endif
