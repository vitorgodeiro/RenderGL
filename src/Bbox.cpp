#include "../include/Bbox.h"

std::string Bbox::toString(){
	return "X_min = " + std::to_string(this->xMin) + "\nX_max = " + std::to_string(this->xMax) + "\nY_min = " 
					  + std::to_string(this->yMin) + "\nY_max = " + std::to_string(this->yMax) + "\nZ_min = " 
					  + std::to_string(this->zMin) + "\nZ_max = " + std::to_string(this->zMax) + "\n";
}

Bbox::Bbox(){
  	xMin = yMin = zMin = std::numeric_limits<float>::infinity();
	xMax = yMax = zMax =  -std::numeric_limits<float>::infinity();
}

void Bbox::update (float x, float y, float z){
	if (x < xMin) {xMin = x;}
	if (x > xMax) {xMax = x;}

	if (y < yMin) {yMin = y;}
	if (y > yMax) {yMax = y;}
	
	if (z < zMin) {zMin = z;}
	if (z > zMax) {zMax = z;}
}

float* Bbox::getCenter(){
    float *center = new float[3];
	std::cout << "Xmin and Xmax =" << xMin << " " << xMax << std::endl;
    center[0] = (xMin + xMax)/2.0f;
    center[1] = (yMin + yMax)/2.0f;
    center[2] = (zMin + zMax)/2.0f;
    return center;
}

