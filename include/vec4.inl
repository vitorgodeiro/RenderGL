/****
* @file vec4.inl
* @brief Implementation of Vec4 class
*/

/* Copyright © 2018 Vítor Godeiro. */

#include "vec4.h"

inline Vec4 operator+( const Vec4 & v1, const Vec4 & v2 ) { return Vec4( v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2], v1[3] + v2[3]); }

inline Vec4 operator-( const Vec4 & v1, const Vec4 & v2 ) { return Vec4( v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], v1[3] - v2[3]); }

inline Vec4 operator*( const Vec4 & v1, const Vec4 & v2 ) { return Vec4( v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2], v1[3] * v2[3]); }

inline Vec4 operator/( const Vec4 & v1, const Vec4 & v2 ) { return Vec4( v1[0]/v2[0], v1[1]/v2[1], v1[2]/v2[2], v1[3]/v2[3]); }

inline Vec4 operator*( const Vec4 & v, float t ) { return Vec4( v[0] * t, v[1] * t, v[2] * t, v[3] * t ); }

inline Vec4 operator*( float t, const Vec4 & v ) { return Vec4( v[0] * t, v[1] * t, v[2] * t, v[3] * t ); }

inline Vec4 operator/( const Vec4 & v, float t ) { return Vec4( v[0]/t, v[1]/t, v[2]/t, v[3]/t ); }

inline Vec4& Vec4::operator+=( const Vec4 & v ) {
    this[0] += v[0];
    this[1] += v[1];
    this[2] += v[2];
    this[3] += v[3];

    return *this;
}

inline Vec4& Vec4::operator-=( const Vec4 & v ) {
    this[0] -= v[0];
    this[1] -= v[1];
    this[2] -= v[2];
    this[3] -= v[3];

    return *this;
}

inline Vec4& Vec4::operator*=( const Vec4 & v ) {
    this[0] *= v[0];
    this[1] *= v[1];
    this[2] *= v[2];
    this[3] *= v[3];

    return *this;
}

inline Vec4& Vec4::operator/=( const Vec4 & v ) {
    this[0] /= v[0];
    this[1] /= v[1];
    this[2] /= v[2];
    this[3] /= v[3];

    return *this;
}

inline Vec4& Vec4::operator*=( const float t ) {
    this[0] *= t;
    this[1] *= t;
    this[2] *= t;
    this[3] *= t;

    return *this;
}

inline Vec4& Vec4::operator/=( const float t ) {
    this[0] /= t;
    this[1] /= t;
    this[2] /= t;
    this[3] /= t;

    return *this;
}

inline Vec4 min(const Vec4& v1, const Vec4& v2) {
    Vec4 ans;
    ans[0] = std::min(v1.r(), v2.r());
    ans[1] = std::min(v1.g(), v2.g());
    ans[2] = std::min(v1.b(), v2.b());
    ans[3] = std::min(v1.a(), v2.a());
    return ans;
}

inline Vec4 max(const Vec4& v1, const Vec4& v2) {
    Vec4 ans;
    ans[0] = std::max(v1.r(), v2.r());
    ans[1] = std::max(v1.g(), v2.g());
    ans[2] = std::max(v1.b(), v2.b());
    ans[3] = std::max(v1.a(), v2.a());
    return ans;
}