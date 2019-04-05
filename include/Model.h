#ifndef __MODEL_H__
#define __MODEL_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <glm/vec3.hpp> // glm::vec3
#include "../src/Bbox.cpp"
#include <stdio.h>
#include <sstream>

/** 
* @class Model
* @brief The class that describe and have 
*		 information of Model
*/
class Model {

	private:
		int numberTriangles;
		int numberMaterials;

		float *vertexPositions;
		float *vertexNormal;
		float *vertexColorIndex;
		float *faceNormal;
		
		float *ambientColor;
		float *diffuseColor;
		float *specularColor; 
		float *materialShine;
		
	public:
		Model(std::string file);
		~Model();
		Bbox box;

		float *getVertexPositions();
		float *getVertexNormal();
		float *getVertexColorIndex();
		int getNumVertex();
};
#endif
