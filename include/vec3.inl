/****
* @file vec3.inl
* @brief Implementation of Vec3 class
*/

/* Copyright © 2018 Vítor Godeiro. */

#include "vec3.h"

inline Vec3 operator+( const Vec3 & v1, const Vec3 & v2 ) { return Vec3( v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]); }

inline Vec3 operator-( const Vec3 & v1, const Vec3 & v2 ) { return Vec3( v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]); }

inline Vec3 operator*( const Vec3 & v1, const Vec3 & v2 ) { return Vec3( v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]); }

inline Vec3 operator/( const Vec3 & v1, const Vec3 & v2 ) { return Vec3( v1[0]/v2[0], v1[1]/v2[1], v1[2]/v2[2]); }

inline Vec3 operator*( const Vec3 & v, float t ) { return Vec3( v[0] * t, v[1] * t, v[2] * t ); }

inline Vec3 operator*( float t, const Vec3 & v ) { return Vec3( v[0] * t, v[1] * t, v[2] * t ); }

inline Vec3 operator/( const Vec3 & v, float t ) { return Vec3( v[0]/t, v[1]/t, v[2]/t ); }

inline Vec3& Vec3::operator+=( const Vec3 & v ) {
    this[0] += v[0];
    this[1] += v[1];
    this[2] += v[2];

    return *this;
}

inline Vec3& Vec3::operator-=( const Vec3 & v ) {
    this[0] -= v[0];
    this[1] -= v[1];
    this[2] -= v[2];

    return *this;
}

inline Vec3& Vec3::operator*=( const Vec3 & v ) {
    this[0] *= v[0];
    this[1] *= v[1];
    this[2] *= v[2];

    return *this;
}

inline Vec3& Vec3::operator/=( const Vec3 & v ) {
    this[0] /= v[0];
    this[1] /= v[1];
    this[2] /= v[2];

    return *this;
}

inline Vec3& Vec3::operator*=( const float t ) {
    this[0] *= t;
    this[1] *= t;
    this[2] *= t;

    return *this;
}

inline Vec3& Vec3::operator/=( const float t ) {
    this[0] /= t;
    this[1] /= t;
    this[2] /= t;

    return *this;
}

inline float Vec3::dot( const Vec3 & v1, const Vec3 & v2 ) {
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

inline Vec3 Vec3::cross( const Vec3 & v1, const Vec3 & v2 ) {
    return Vec3 (  (v1[1]*v2[2] - v1[2]*v2[1]),
                (-(v1[0]*v2[2] - v1[2]*v2[0])),
                  (v1[0]*v2[1] - v1[1]*v2[0]));
}

inline Vec3 Vec3::unit_vector( const Vec3 & v ) {
    return v/v.length();
}

inline void Vec3::make_unit_vector( ) {
    auto v_dot_v = e[0]*e[0] + e[1]*e[1] + e[2]*e[2] ;
    assert( fabs( v_dot_v - 0.f ) > 0.000001 );
    float k = 1.f/ sqrt( v_dot_v );
    this->e[0] *= k;
    this->e[1] *= k;
    this->e[2] *= k;
}

inline Vec3 min(const Vec3& v1, const Vec3& v2) {
    Vec3 ans;
    ans[0] = std::min(v1.x(), v2.x());
    ans[1] = std::min(v1.y(), v2.y());
    ans[2] = std::min(v1.z(), v2.z());
    return ans;
}

inline Vec3 max(const Vec3& v1, const Vec3& v2) {
    Vec3 ans;
    ans[0] = std::max(v1.x(), v2.x());
    ans[1] = std::max(v1.y(), v2.y());
    ans[2] = std::max(v1.z(), v2.z());
    return ans;
}