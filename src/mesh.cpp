#include "../include/mesh.h"

float *Mesh::getVertexPositions() 		 { return this->vertexPositions; }
float *Mesh::getVertexNormal() 			 { return this->vertexNormal; }
float *Mesh::getVertexColorIndex() 		 { return this->vertexColorIndex; }
float *Mesh::getAmbientColor()  		 { return this->ambientColor; }
float *Mesh::getDiffuseColor()  		 { return this->diffuseColor; }
float *Mesh::getSpecularColor() 	 	 { return this->specularColor; }
float *Mesh::getMaterialShine() 		 { return this->materialShine; }
float *Mesh::getFaceNormal() 			 { return this->faceNormal; }
float *Mesh::getVertexTexturePositions() { return this->vertexTexturePositions; }

int Mesh::getNumVertex() { return numberTriangles*3; }
int Mesh::getNumTriangles() { return numberTriangles; }

int Mesh::getnumberMaterials() { return numberMaterials; }

Mesh::Mesh(std::string file){
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
	this->vertexTexturePositions = new float[2*this->numberTriangles];
	
	fscanf(fp,"Material count = %d\n", &numberMaterials);

	this->ambientColor = new float[3*this->numberMaterials];
	this->diffuseColor = new float[3*this->numberMaterials];
	this->specularColor = new float[3*this->numberMaterials];
	this->materialShine = new float[this->numberMaterials];
	
	for(int materialCount = 0; materialCount < numberMaterials; materialCount++){
		fscanf(fp, "ambient color %f %f %f\n", &(ambientColor[3*materialCount ]), &(ambientColor[3*materialCount + 1]), &(ambientColor[3*materialCount + 2]));
		fscanf(fp, "diffuse color %f %f %f\n", &diffuseColor[3*materialCount], &diffuseColor[3*materialCount + 1], &diffuseColor[3*materialCount + 2]);
		fscanf(fp, "specular color %f %f %f\n", &specularColor[3*materialCount ], &specularColor[3*materialCount + 1], &specularColor[3*materialCount + 2]);
		fscanf(fp, "material shine %f\n", &materialShine[materialCount]);
	}
	
	fscanf(fp,"Texture = %s\n", &texture);

	fscanf(fp, "%c", &ch);
	while(ch!= '\n') {fscanf(fp, "%c", &ch);}

	for (int nTriangleActual = 0; nTriangleActual < this->numberTriangles; nTriangleActual++){
		char a;
        int posUpdate = nTriangleActual*9;

        if (this->texture == 'N'){
        	fscanf(fp, "v%c %f %f %f %f %f %f %f\n", &a, &(vertexPositions[posUpdate]), &(vertexPositions[posUpdate + 1]), &(vertexPositions[posUpdate + 2]),
														 &(vertexNormal[posUpdate]), &(vertexNormal[posUpdate + 1]), &(vertexNormal[posUpdate + 2]),
					  									 &(vertexColorIndex[nTriangleActual*3]));	
			this->box.update(vertexPositions[posUpdate], vertexPositions[posUpdate+1], vertexPositions[posUpdate+2]);

        	posUpdate += 3;
			fscanf(fp, "v%c %f %f %f %f %f %f %f\n", &a, &(vertexPositions[posUpdate]), &(vertexPositions[posUpdate + 1]), &(vertexPositions[posUpdate + 2]),
														 &(vertexNormal[posUpdate]), &(vertexNormal[posUpdate + 1]), &(vertexNormal[posUpdate + 2]),
														 &(vertexColorIndex[nTriangleActual*3]));
			this->box.update(vertexPositions[posUpdate], vertexPositions[posUpdate+1], vertexPositions[posUpdate+2]);

        	posUpdate += 3;
			fscanf(fp, "v%c %f %f %f %f %f %f %f\n", &a, &(vertexPositions[posUpdate]), &(vertexPositions[posUpdate + 1]), &(vertexPositions[posUpdate + 2]),
														 &(vertexNormal[posUpdate]), &(vertexNormal[posUpdate + 1]), &(vertexNormal[posUpdate + 2]),
														 &(vertexColorIndex[nTriangleActual*3]));
			this->box.update(vertexPositions[posUpdate], vertexPositions[posUpdate+1], vertexPositions[posUpdate+2]);
		}else if (this->texture == 'Y'){
			fscanf(fp, "v%c %f %f %f %f %f %f %f %f %f\n", &a, &(vertexPositions[posUpdate]), &(vertexPositions[posUpdate + 1]), &(vertexPositions[posUpdate + 2]),
														 &(vertexNormal[posUpdate]), &(vertexNormal[posUpdate + 1]), &(vertexNormal[posUpdate + 2]),
					  									 &(vertexColorIndex[nTriangleActual*3]), &(vertexTexturePositions[posUpdate]), &(vertexTexturePositions[posUpdate+1]));	
			this->box.update(vertexPositions[posUpdate], vertexPositions[posUpdate+1], vertexPositions[posUpdate+2]);	

        	posUpdate += 3;
			fscanf(fp, "v%c %f %f %f %f %f %f %f %f %f\n", &a, &(vertexPositions[posUpdate]), &(vertexPositions[posUpdate + 1]), &(vertexPositions[posUpdate + 2]),
														 &(vertexNormal[posUpdate]), &(vertexNormal[posUpdate + 1]), &(vertexNormal[posUpdate + 2]),
														 &(vertexColorIndex[nTriangleActual*3]), &(vertexTexturePositions[posUpdate]), &(vertexTexturePositions[posUpdate+1]));
			this->box.update(vertexPositions[posUpdate], vertexPositions[posUpdate+1], vertexPositions[posUpdate+2]);
        	
        	posUpdate += 3;
			fscanf(fp, "v%c %f %f %f %f %f %f %f %f %f\n", &a, &(vertexPositions[posUpdate]), &(vertexPositions[posUpdate + 1]), &(vertexPositions[posUpdate + 2]),
														 &(vertexNormal[posUpdate]), &(vertexNormal[posUpdate + 1]), &(vertexNormal[posUpdate + 2]),
														 &(vertexColorIndex[nTriangleActual*3]), &(vertexTexturePositions[posUpdate]), &(vertexTexturePositions[posUpdate+1]));
			this->box.update(vertexPositions[posUpdate], vertexPositions[posUpdate+1], vertexPositions[posUpdate+2]);
		}

		fscanf(fp, "face normal %f %f %f\n", &(faceNormal[nTriangleActual*3]), &(faceNormal[nTriangleActual*3 + 1]), &(faceNormal[nTriangleActual + 2]));
	}
}

Mesh::~Mesh(){
	delete []this->vertexPositions;
	delete []this->vertexNormal;
	delete []this->vertexColorIndex;
	delete []this->vertexTexturePositions;
}
