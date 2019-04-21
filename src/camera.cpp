/**
* @file camera.cpp
* @brief Implementation of Camera class
* Created:       21/04/2019
* Last Modified: 21/04/2019
* Copyright © 2019 Vitor Godeiro. 
**/

#include "../include/camera.h"

Camera::Camera(){
    position = Vec3(0.0);
    u1 = Vec3(1.0f, 0.0f, 0.0f);
    u2 = Vec3(0.0f, 1.0f, 0.0f);
    u3 = Vec3(0.0f, 0.0f, 1.0f);
    hfov = 60;
    vfov = 60;

}

float Camera::getHFov(){
    return hfov;
}

void Camera::setHFov(float val){
    hfov = val;
}

float Camera::getVFov(){
    return vfov;
}

void Camera::setVFov(float val){
    vfov = val;
}
