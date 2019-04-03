#include "../include/Model.h"

std::string Model::toString(){
	
}

float *Model::getVertexPositions(){ return this->vertexPositions; }
float *Model::getVertexNormal(){ return this->vertexNormal; }
float *Model::getVertexColorIndex(){ return this->vertexColorIndex; }

int Model::getNumVertex(){
	return numberTriangles*3;
}
void Model::parseVertex(float data[], int triangleN){
	this->faceNormal[3*triangleN] = data[0];
	this->faceNormal[3*triangleN + 1] = data[1];
	this->faceNormal[3*triangleN + 2] = data[2];
}
void Model::parseVertex(float data[], int vertexN, int triangleN){
	int posUpdate = 3*vertexN + triangleN*9;
	this->vertexPositions[posUpdate] = data[0];
	this->vertexPositions[posUpdate + 1] = data[1];
	this->vertexPositions[posUpdate + 2] = data[2];
	this->box.update(data[0], data[1], data[2]);

	this->vertexNormal[posUpdate] = data[3];
	this->vertexNormal[posUpdate + 1] = data[4];
	this->vertexNormal[posUpdate + 2] = data[5];

	this->vertexColorIndex[vertexN + 3*triangleN] = data[6];
}


Model::Model(std::string file){
  	std::string line;
	std::ifstream myfile (file);
	std::cout << box.toString();
	if (myfile.is_open()){
        std::getline (myfile,line);
        this->nameModel = line.substr(line.find('=')+1);
        std::getline (myfile,line);
        this->numberTriangles = std::stoi(line.substr(line.find('=')+1));  
		this->vertexPositions = new float[9*this->numberTriangles];
		this->vertexNormal = new float[9*this->numberTriangles];
		this->vertexColorIndex = new float[3*this->numberTriangles];
		this->faceNormal = new float[this->numberTriangles];

        std::getline (myfile,line);
        this->numberMaterials = std::stoi(line.substr(line.find('=')+1));
		
        for(int materialCount = numberMaterials; 0 < materialCount; materialCount--){
            std::getline (myfile,line);
            std::cout << line  << std::endl;
            std::getline (myfile,line);
            std::cout << line << std::endl;
            std::getline (myfile,line);
            std::cout << line << std::endl;
            std::getline (myfile,line);
            std::cout << line << std::endl;
        }
    
        std::cout << "**********************************************" << std::endl;
        
        std::getline (myfile, line); // readTrash
		
		for (int nTriangleActual = 0; nTriangleActual < this->numberTriangles; nTriangleActual++){
            std::getline (myfile,line);
            parseVertex(readVertex(line), 0, nTriangleActual);
            std::getline (myfile,line);
            parseVertex(readVertex(line), 1, nTriangleActual);
            std::getline (myfile,line);
            parseVertex(readVertex(line), 2, nTriangleActual);
            std::getline (myfile,line);
            readFaceNormal(line);
		}
		myfile.close();
		std::cout << box.toString();
	}
	else {std::cout << "Unable to open file"; }
}

Model::~Model(){
	delete []this->vertexPositions;
	delete []this->vertexNormal;
	delete []this->vertexColorIndex;
}

std::vector<std::string> Model::splitString(const char *str,char delimiter = ' '){
	std::vector<std::string> result;
    do {
        const char *begin = str;
        while(*str != delimiter && *str) { str++;}
        result.push_back(std::string(begin, str));
    } while (0 != *str++);
    return result;
}

float * Model::readVertex (std::string input){
    float *output = new float[7];
    std::vector<std::string> inputSplited = splitString(input.c_str(), ' ');
    output[0] = std::strtof(inputSplited[1].c_str(), 0);
    output[1] = std::strtof(inputSplited[2].c_str(), 0); 
    output[2] = std::strtof(inputSplited[3].c_str(), 0);
    output[3] = std::strtof(inputSplited[4].c_str(), 0);
    output[4] = std::strtof(inputSplited[5].c_str(), 0);
    output[5] = std::strtof(inputSplited[6].c_str(), 0);
    output[6] = std::strtof(inputSplited[7].c_str(), 0);
    return output;
}

float * Model::readFaceNormal (std::string input){
    float output[3];
    std::vector<std::string> inputSplited = splitString(input.c_str(), ' ');
    output[0] = std::strtof(inputSplited[1].c_str(), 0);
    output[1] = std::strtof(inputSplited[2].c_str(), 0);
    output[2] = std::strtof(inputSplited[3].c_str(), 0);
    return output;
}
