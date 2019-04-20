#ifndef __Mesh_H__
#define __Mesh_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <glm/vec3.hpp> // glm::vec3
#include "../src/bbox.cpp"
#include <stdio.h>
#include <sstream>

/** 
* @class Mesh
* @brief The class that describe and have 
*		 information of Mesh
*/
class Mesh {
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
		Mesh(std::string file);
		~Mesh();
		Bbox box;

		float *getVertexPositions();
		float *getVertexNormal();
		float *getVertexColorIndex();
		int getNumVertex();
};
#endif
