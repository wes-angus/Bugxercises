#pragma once 
#include "Objects.h"
// this class is used to help make the game more efficent 
// used so we can only load the object once 
class mesh:public object 
{
public: 
	mesh(std::string filename);
	~mesh();
	void MeshDraw(Texture &textName,bool invisible,float xCoord,float yCoord,float zCoord,float yrotate,float zrotate );

	Shader *ptrShader;
	Texture * diffuseTexture;

	float *VBOData;
	unsigned int numFace;
	unsigned int buffer;
};