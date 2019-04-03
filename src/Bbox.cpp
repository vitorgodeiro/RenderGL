#include "../include/Bbox.h"

std::string Bbox::toString(){
	return "X_min = " + std::to_string(this->xMin) + "\nX_max = " + std::to_string(this->xMax) + "\nY_min = " 
					  + std::to_string(this->yMin) + "\nY_max = " + std::to_string(this->yMax) + "\nZ_min = " 
					  + std::to_string(this->zMin) + "\nZ_max = " + std::to_string(this->zMax) + "\n";
}

Bbox::Bbox(){
  	xMin = yMin = zMin = std::numeric_limits<float>::max();
	xMax = yMax = zMax =  std::numeric_limits<float>::min();
}

void Bbox::update (float x, float y, float z){
	if (x < xMin) {xMin = x;}
	if (x > xMax) {xMax = x;}

	if (y < yMin) {yMin = y;}
	if (y > yMax) {yMax = y;}

	if (z < zMin) {zMin = z;}
	if (z > zMax) {zMax = z;}
}
