/**
* @file Mat4GL.h
* @brief Definition of Matrix 4x4 class
* Created:       21/04/2019
* Last Modified: 21/04/2019
* Copyright © 2019 Vitor Godeiro. 
**/

#ifndef _Mat4GL_H_
#define _Mat4GL_H_

#include <cmath>
#include <iostream>
#include <cassert>
#include "vec3.h"

const float DEG2RAD = 3.141593f / 180.0f;

/** 
* @class Mat4GL
* @brief The class that describes an Matrix 4x4
*/
class Mat4GL
{
    public:
        

        Mat4GL() {
            e[0] = e[5] = e[10] = e[15] = e[1] = e[2] = e[3] = e[4] = e[6] = e[7] = e[8] = e[9] = e[11] = e[12] = e[13] = e[14] = 0;
            
        }

         Mat4GL(float val) {
            e[0] = e[5] = e[10] = e[15] = val;
            e[1] = e[2] = e[3] = e[4] = e[6] = e[7] = e[8] = e[9] = e[11] = e[12] = e[13] = e[14] = 0;
        }


        Mat4GL(float e0_, float e1_, float e2_, float e3_,
              float e4_, float e5_, float e6_, float e7_,
              float e8_, float e9_, float e10_, float e11_,
              float e12_, float e13_, float e14_, float e15_ ): e{ e0_, e1_, e2_, e3_, e4_, e5_, e6_, e7_, e8_, e9_, e10_, e11_, e12_, e13_, e14_, e15_}{ }
        
       

        inline float operator[] (size_t i) const { return e[i]; }
        inline float& operator[](size_t i) { return e[i]; }

        inline const Mat4GL& operator+( void ) const { return *this; }
        inline Mat4GL operator-( void ) const { return Mat4GL(-e[0], -e[1], -e[2], -e[3], -e[4], -e[5], -e[6], -e[7], -e[8], -e[9], -e[10], -e[11], -e[12], -e[13], -e[14], -e[15]); }

        inline Mat4GL& operator+=( const Mat4GL& v);
        inline Mat4GL& operator-=( const Mat4GL& v);
        inline Mat4GL& operator*=( const Mat4GL& v);
        inline Mat4GL& operator*=( const float f);
        inline Mat4GL& operator/=( const float f);

        void to_string(){
            std::cout << e[0] << " " << e[1] << " " << e[2] << " " << e[3] <<  std::endl;
            std::cout << e[4] << " " << e[5] << " " << e[6] << " " << e[7] <<  std::endl;
            std::cout << e[8] << " " << e[9] << " " << e[10] << " " << e[11] <<  std::endl;
            std::cout << e[12] << " " << e[13] << " " << e[14] << " " << e[15] <<  std::endl;
        }

        static Mat4GL lookAt(Vec3 eye, Vec3 center, Vec3 up){
            Vec3 n = Vec3::unit_vector(eye - center);
            Vec3 u = Vec3::cross(Vec3::unit_vector(up),n);
            Vec3 v = Vec3::cross(n, u);
            return Mat4GL(u[0], u[1], u[2], Vec3::dot(-eye, u),
                          v[0], v[1], v[2], Vec3::dot(-eye, v),
                          n[0], n[1], n[2], Vec3::dot(-eye, n), 
                          0,    0,    0,    1);
        }

        static Mat4GL perspective (float fovY, float aspect, float zNear, float zFar){
            float tangent = tanf(fovY/2 * DEG2RAD); 
            float height = zNear * tangent;         
            float width = height * aspect;        
            return Mat4GL::setFrustum(-width, width, -height, height, zNear, zFar);
        }

        static Mat4GL setFrustum(float l, float r, float b, float t, float n, float f){
            Mat4GL mat;
            mat[0]  = 2 * n / (r - l);
            mat[2]  = (r + l) / (r - l);
            mat[5]  = 2 * n / (t - b);
            mat[6]  = (t + b) / (t - b);
            mat[10] = -(f + n) / (f - n);
            mat[11] = -(2 * f * n) / (f - n);
            mat[14] = -1;
            mat[15] = 0;
            return mat;
        }

        static Mat4GL viewPort(float l, float r, float b, float t){
			Mat4GL mat;
			mat[0] = (r-l)/2;
			mat[3] = (r+l)/2;
			mat[5] = (t-b)/2;
			mat[7] = (t+b)/2;
			mat[10] = 1;
			mat[15] = 1;
			return mat;
        }

    private:
        float e[16];
};

#include "mat4.inl"

#endif
