/**
* @file camera.h
* @brief Definition of Camera class
* Created:       21/04/2019
* Last Modified: 21/04/2019
* Copyright © 2019 Vitor Godeiro. 
**/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vec3.h"

/**
* @class Camera
* @ brief 
*/
class Camera {
    private:
        Vec3 position;
        Vec3 u1, u2, u3;
        float hfov, vfov;
    public:
        Camera();
        float getHFov();
        void setHFov(float val);       
        float getVFov();
        void setVFov(float val);

};

#endif
