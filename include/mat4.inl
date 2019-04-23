/**
* @file Mat4GL.inl
* @brief Implementation of Matrix 4x4 class
* Created:       21/04/2019
* Last Modified: 21/04/2019
* Copyright © 2019 Vitor Godeiro. 
**/

#include "mat4.h"

inline Mat4GL operator+( const Mat4GL & v1, const Mat4GL & v2 ) { return Mat4GL(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2], v1[3] + v2[3], 
                                                                        v1[4] + v2[4], v1[5] + v2[5], v1[6] + v2[6], v1[7] + v2[7],
                                                                        v1[8] + v2[8], v1[9] + v2[9], v1[10] + v2[10], v1[11] + v2[11],
                                                                        v1[12] + v2[12], v1[13] + v2[13], v1[14] + v2[14], v1[15] + v2[15]); }

inline Mat4GL operator-( const Mat4GL & v1, const Mat4GL & v2 ) { return Mat4GL(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], v1[3] - v2[3], 
                                                                        v1[4] - v2[4], v1[5] - v2[5], v1[6] - v2[6], v1[7] - v2[7],
                                                                        v1[8] - v2[8], v1[9] - v2[9], v1[10] - v2[10], v1[11] - v2[11],
                                                                        v1[12] - v2[12], v1[13] - v2[13], v1[14] - v2[14], v1[15] - v2[15]); }

inline Mat4GL operator*( const Mat4GL & v1, const Mat4GL & v2 ) { 
    float a00=v1[0];
    float a01=v1[4];
    float a02=v1[8];
    float a03=v1[12];

    float a10=v1[1];
    float a11=v1[5];
    float a12=v1[9];
    float a13=v1[13];

    float a20=v1[2];
    float a21=v1[6];
    float a22=v1[10];
    float a23=v1[14];

    float a30=v1[3];
    float a31=v1[7];
    float a32=v1[11];
    float a33=v1[15];

    float b00=v2[0];
    float b01=v2[4];
    float b02=v2[8];
    float b03=v2[12];

    float b10=v2[1];
    float b11=v2[5];
    float b12=v2[9];
    float b13=v2[13];

    float b20=v2[2];
    float b21=v2[6];
    float b22=v2[10];
    float b23=v2[14];

    float b30=v2[3];
    float b31=v2[7];
    float b32=v2[11];
    float b33=v2[15];

    return Mat4GL(a00*b00+a10*b01+a20*b02+a30*b03, a00*b10+a10*b11+a20*b12+a30*b13, a00*b20+a10*b21+a20*b22+a30*b23, a00*b30+a10*b31+a20*b32+a30*b33,
                 a01*b00+a11*b01+a21*b02+a31*b03, a01*b10+a11*b11+a21*b12+a31*b13, a01*b20+a11*b21+a21*b22+a31*b23, a01*b30+a11*b31+a21*b32+a31*b33, 
                 a02*b00+a12*b01+a22*b02+a32*b03, a02*b10+a12*b11+a22*b12+a32*b13, a02*b20+a12*b21+a22*b22+a32*b23, a02*b30+a12*b31+a22*b32+a32*b33, 
                 a03*b00+a13*b01+a23*b02+a33*b03, a03*b10+a13*b11+a23*b12+a33*b13, a03*b20+a13*b21+a23*b22+a33*b23, a03*b30+a13*b31+a23*b32+a33*b33); 
}

inline Mat4GL operator*( const Mat4GL & v, float t ) { return Mat4GL( v[0] * t, v[1] * t, v[2] * t, v[3] * t, 
                                                                v[4] * t, v[5] * t, v[6] * t, v[7] * t,
                                                                v[8] * t, v[9] * t, v[10] * t, v[11] * t,
                                                                v[12] * t, v[13] * t, v[14] * t, v[15] * t); }

inline Mat4GL operator*( float t, const Mat4GL & v ) { return Mat4GL( v[0] * t, v[1] * t, v[2] * t, v[3] * t, 
                                                                v[4] * t, v[5] * t, v[6] * t, v[7] * t,
                                                                v[8] * t, v[9] * t, v[10] * t, v[11] * t,
                                                                v[12] * t, v[13] * t, v[14] * t, v[15] * t); }

inline Mat4GL operator/( const Mat4GL & v, float t ) { return Mat4GL( v[0] / t, v[1] / t, v[2] * t, v[3] / t, 
                                                                v[4] / t, v[5] / t, v[6] * t, v[7] / t,
                                                                v[8] / t, v[9] / t, v[10] * t, v[11] / t,
                                                                v[12] / t, v[13] / t, v[14] / t, v[15] / t); }

inline Mat4GL& Mat4GL::operator+=( const Mat4GL& v){
    e[0] += v[0];
    e[1] += v[1];
    e[2] += v[2];
    e[3] += v[3];
    e[4] += v[4];
    e[5] += v[5];
    e[6] += v[6];
    e[7] += v[7];
    e[8] += v[8];
    e[9] += v[9];
    e[10] += v[10];
    e[11] += v[11];
    e[12] += v[12];
    e[13] += v[13];
    e[14] += v[14];
    e[15] += v[15];

    return *this;
}

inline Mat4GL& Mat4GL::operator-=( const Mat4GL & v ) {
    e[0] -= v[0];
    e[1] -= v[1];
    e[2] -= v[2];
    e[3] -= v[3];
    e[4] -= v[4];
    e[5] -= v[5];
    e[6] -= v[6];
    e[7] -= v[7];
    e[8] -= v[8];
    e[9] -= v[9];
    e[10] -= v[10];
    e[11] -= v[11];
    e[12] -= v[12];
    e[13] -= v[13];
    e[14] -= v[14];
    e[15] -= v[15];

    return *this;
}

inline Mat4GL& Mat4GL::operator*=( const Mat4GL & v ) {
    float a00=e[0];
    float a01=e[4];
    float a02=e[8];
    float a03=e[12];

    float a10=e[1];
    float a11=e[5];
    float a12=e[9];
    float a13=e[13];

    float a20=e[2];
    float a21=e[6];
    float a22=e[10];
    float a23=e[14];

    float a30=e[3];
    float a31=e[7];
    float a32=e[11];
    float a33=e[15];

    float b00=v[0];
    float b01=v[4];
    float b02=v[8];
    float b03=v[12];

    float b10=v[1];
    float b11=v[5];
    float b12=v[9];
    float b13=v[13];

    float b20=v[2];
    float b21=v[6];
    float b22=v[10];
    float b23=v[14];

    float b30=v[3];
    float b31=v[7];
    float b32=v[11];
    float b33=v[15];

    e[0] = a00*b00+a10*b01+a20*b02+a30*b03;
    e[1] = a00*b10+a10*b11+a20*b12+a30*b13;
    e[2] = a00*b20+a10*b21+a20*b22+a30*b23;
    e[3] = a00*b30+a10*b31+a20*b32+a30*b33;

    e[4] = a01*b00+a11*b01+a21*b02+a31*b03;
    e[5] = a01*b10+a11*b11+a21*b12+a31*b13;
    e[6] = a01*b20+a11*b21+a21*b22+a31*b23;
    e[7] = a01*b30+a11*b31+a21*b32+a31*b33;

    e[8] = a02*b00+a12*b01+a22*b02+a32*b03;
    e[9] = a02*b10+a12*b11+a22*b12+a32*b13;
    e[10] = a02*b20+a12*b21+a22*b22+a32*b23;
    e[11] = a02*b30+a12*b31+a22*b32+a32*b33;

    e[12] = a03*b00+a13*b01+a23*b02+a33*b03;
    e[13] = a03*b10+a13*b11+a23*b12+a33*b13;
    e[14] = a03*b20+a13*b21+a23*b22+a33*b23;
    e[15] = a03*b30+a13*b31+a23*b32+a33*b33;

    return *this;
}

inline Mat4GL& Mat4GL::operator*=( const float t ) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    e[3] *= t;
    e[4] *= t;
    e[5] *= t;
    e[6] *= t;
    e[7] *= t;
    e[8] *= t;
    e[9] *= t;
    e[10] *= t;
    e[11] *= t;
    e[12] *= t;
    e[13] *= t;
    e[14] *= t;
    e[15] *= t;

    return *this;
}

inline Mat4GL& Mat4GL::operator/=( const float t ) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    e[3] /= t;
    e[4] /= t;
    e[5] /= t;
    e[6] /= t;
    e[7] /= t;
    e[8] /= t;
    e[9] /= t;
    e[10] /= t;
    e[11] /= t;
    e[12] /= t;
    e[13] /= t;
    e[14] /= t;
    e[15] /= t;

    return *this;
}

