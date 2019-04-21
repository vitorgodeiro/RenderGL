/****
* @file vec3.h
* @brief Definition of Vector class
*/

/* Copyright © 2018 Vítor Godeiro. */

#ifndef _Vec3_H_
#define _Vec3_H_

#include <cmath>
#include <iostream>
#include <cassert>

/** 
* @class Vec3
* @brief The class that describes an vector in the Ray Tracer project
*/
class Vec3
{
    public:
        /** @brief The construtor specifies a vec3 class.*/
        Vec3( float e0_=0.f, float e1_=0.f, float e2_=0.f ): e{ e0_, e1_, e2_ }{ }

        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }
        inline float z() const { return e[2]; }

        inline float& x() { return e[0]; }
        inline float& y() { return e[1]; }
        inline float& z() { return e[2]; }
        
        inline float operator[] (size_t i) const { return e[i]; }
        inline float& operator[](size_t i) { return e[i]; }

        inline const Vec3& operator+( void ) const { return *this; }
        inline Vec3 operator-( void ) const { return Vec3(-e[0], -e[1], -e[2]); }

        inline Vec3& operator+=( const Vec3& v);
        inline Vec3& operator-=( const Vec3& v);
        inline Vec3& operator*=( const Vec3& v);
        inline Vec3& operator/=( const Vec3& v);
        inline Vec3& operator*=( const float f);
        inline Vec3& operator/=( const float f);

        inline float length() const {return (sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]));}
        inline float squared_length() const {return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
        static inline float dot(const Vec3 & v1, const Vec3 & v2);
        static inline Vec3 cross(const Vec3 & v1, const Vec3 & v2);
        static inline Vec3 unit_vector(const Vec3 & v);
        inline void make_unit_vector();
        inline Vec3 min(const Vec3& v1, const Vec3& v2);
        inline Vec3 max(const Vec3& v1, const Vec3& v2);

    private:
        float e[3];
};

typedef Vec3 Point3;

#include "vec3.inl"

#endif
