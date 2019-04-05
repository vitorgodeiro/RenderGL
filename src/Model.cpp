#include "../include/Model.h"

float *Model::getVertexPositions(){ return this->vertexPositions; }
float *Model::getVertexNormal(){ return this->vertexNormal; }
float *Model::getVertexColorIndex(){ return this->vertexColorIndex; }

void Model::setVertexPositions(float vertex[]){
	for (int i =0; i < getNumVertex()*3; i++){
		vertex[i] = this->vertexPositions[i];
	}	
}

int Model::getNumVertex(){
	return numberTriangles*3;
}
void Model::parseNormal(float data[], int triangleN){
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
	char ch;
	FILE* fp = fopen(file.c_str(),"r");
	if (fp==NULL) { std::cout << "ERROR: unable to open " << file << std::endl; exit(1); }
	fscanf(fp, "%c", &ch);
	while(ch!= '\n') { fscanf(fp, "%c", &ch); }
	fscanf(fp,"# triangles = %d\n", &numberTriangles);
	this->vertexPositions = new float[9*this->numberTriangles];
	this->vertexNormal = new float[9*this->numberTriangles];
	this->vertexColorIndex = new float[3*this->numberTriangles];
	this->faceNormal = new float[3*this->numberTriangles];
	
	fscanf(fp,"Material count = %d\n", &numberMaterials);

	this->ambientColor = new float[3*this->numberMaterials];
	this->diffuseColor = new float[3*this->numberMaterials];
	this->specularColor = new float[3*this->numberMaterials];
	this->materialShine = new float[this->numberMaterials];

	for(int materialCount = numberMaterials; 0 < materialCount; materialCount--){
		fscanf(fp, "ambient color %f %f %f\n", &ambientColor[3*materialCount], &ambientColor[3*materialCount + 1], &ambientColor[3*materialCount + 2]);
		fscanf(fp, "diffuse color %f %f %f\n", &diffuseColor[3*materialCount], &diffuseColor[3*materialCount + 1], &diffuseColor[3*materialCount + 2]);
		fscanf(fp, "specular color %f %f %f\n", &specularColor[3*materialCount], &specularColor[3*materialCount + 1], &specularColor[3*materialCount + 2]);
		fscanf(fp, "material shine %f\n", &materialShine[materialCount]);
	}

	fscanf(fp, "%c", &ch);
	while(ch!= '\n') {fscanf(fp, "%c", &ch);}

	for (int nTriangleActual = 0; nTriangleActual < this->numberTriangles; nTriangleActual++){
        parseVertex(readVertex(fp), 0, nTriangleActual);
        parseVertex(readVertex(fp), 1, nTriangleActual);
        parseVertex(readVertex(fp), 2, nTriangleActual);
        parseNormal(readFaceNormal(fp), nTriangleActual);
	}
}

Model::~Model(){
	delete []this->vertexPositions;
	delete []this->vertexNormal;
	delete []this->vertexColorIndex;
}

std::vector<std::string> Model::splitString(const char *str,char delimiter = ' '){
	std::vector<std::string> tokens;
   	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter)){ tokens.push_back(token); }
    return tokens;
}

float * Model::readVertex (FILE *input){
    float *output = new float[7];
	char a;
    fscanf(input, "v%c %f %f %f %f %f %f %f\n", &a, &(output[0]), &(output[1]), &(output[2]),
											&(output[3]), &(output[4]), &(output[5]),
											&(output[6]));	
    return output;
}

float * Model::readFaceNormal (FILE *input){
    float *output = new float[3];
    fscanf(input, "face normal %f %f %f\n", &(output[0]), &(output[1]), &(output[2]));
    return output;
}
