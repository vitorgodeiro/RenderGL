/****
* @file Vec4.h
* @brief Definition of Vector class
*/

/* Copyright © 2019 Vítor Godeiro. */

#ifndef _VEC4_H_
#define _VEC4_H_

#include <cmath>
#include <iostream>
#include <cassert>

/** 
* @class Vec4
* @brief The class that describes an vector in the Ray Tracer project
*/
class Vec4
{
    public:
        /** @brief The construtor specifies a Vec4 class.*/
        Vec4( float e0_=0.f, float e1_=0.f, float e2_=0.f, float e3_=1.f ): e{ e0_, e1_, e2_, e3_ }{ }

        inline float r() const { return e[0]; }
        inline float g() const { return e[1]; }
        inline float b() const { return e[2]; }
        inline float a() const { return e[3]; }

        inline float& r() { return e[0]; }
        inline float& g() { return e[1]; }
        inline float& b() { return e[2]; }
        inline float& a() { return e[3]; }
        
        inline float operator[] (size_t i) const { return e[i]; }
        inline float& operator[](size_t i) { return e[i]; }

        inline const Vec4& operator+( void ) const { return *this; }
        inline Vec4 operator-( void ) const { return Vec4(-e[0], -e[1], -e[2]); }

        inline Vec4& operator+=( const Vec4& v);
        inline Vec4& operator-=( const Vec4& v);
        inline Vec4& operator*=( const Vec4& v);
        inline Vec4& operator/=( const Vec4& v);
        inline Vec4& operator*=( const float f);
        inline Vec4& operator/=( const float f);

        inline Vec4 min(const Vec4& v1, const Vec4& v2);
        inline Vec4 max(const Vec4& v1, const Vec4& v2);

    private:
        float e[4];
};

#include "vec4.inl"

#endif
