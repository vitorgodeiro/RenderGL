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

		/**
		* @brief  Function that split String from delimiter 
		* @param  Char which is the delimiter
		* @return Vector of String with substrings delimited by 
		*		  the delimiter
		*/
		std::vector<std::string> splitString(const char *str, char delimiter);

		/**
		* @brief  Function that read the information of vertex
		*         from String and parse to Array 
		* @param  File with information of vertex
		* @return Vector of float with size 7 
		*          [pos(x,y,z) normal(x,y,z) color_index]
		*/
		float * readVertex (FILE *input);

		/**
		* @brief  Function that read the information of face normal
		*         from String and parse to Array 
		* @param  String with information of face normal
		* @return Vector of float with normal of face
		*/
		float *readFaceNormal (FILE *input);

		/**
		* @brief  Function that get the array with vertex information
		* 	      and split in several arrays (vertexPosition, 
		* 		  vertexNormal and faceNormal)
		* @param data Float* that define de data of vertex
		* @param vertexN Int that define whether it is v1, v2 or v3 
		* @param triangleN Int that define the triangle number
		* @return void
		**/
		void parseVertex(float data[], int vertexN, int triangleN);
		
		/**
		* @brief  Function that get the array with face normal
		*		  information and split one array
		* @param data Float* that define de data of face normal
		* @param triangleN Int that define the triangle number
		* @return void
		**/
		void parseNormal(float data[], int triangleN);

		int numberTriangles;
		int numberMaterials;
		std::string nameModel;

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
		std::string toString();
		Bbox box;
		float *getVertexPositions();
		float *getVertexNormal();
		float *getVertexColorIndex();
		int getNumVertex();
};
#endif
