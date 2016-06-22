#include "mesh.h"
mesh::mesh(std::string filename)
{// load in the object into buffer ONLY THIS TIME 
	loadobject(filename);
	numFace=PtrF->size()*3;
	VBOData= new float[numFace*8];
	for(unsigned int i=0;i<((int)PtrF->size());i++)	//
	{// while the object is less than the faces 
		OFace tempFace;
		tempFace=PtrF->at(i);
		//vertex 1
		Vec tempVert1=PtrV->at(tempFace.vert.first-1);
		Vec tempNorm1=PtrN->at(tempFace.norm.first-1);
		Vec tempText1=PtrT->at(tempFace.text.first-1);
		//normals 
		VBOData[i*24+0]=tempNorm1.x;
		VBOData[i*24+1]=tempNorm1.y;
		VBOData[i*24+2]=tempNorm1.z;
		//textures 
		VBOData[i*24+3]=(tempText1.x);
		VBOData[i*24+4]=(tempText1.y);
		//vertices 
		VBOData[i*24+5]=tempVert1.x;
		VBOData[i*24+6]=tempVert1.y;
		VBOData[i*24+7]=tempVert1.z;

		//For Vertex 2
		Vec tempNorm2=PtrN->at(tempFace.norm.second-1);
		Vec tempVert2=PtrV->at(tempFace.vert.second-1);
		Vec tempText2=PtrT->at(tempFace.text.second-1);
		VBOData[i*24+8]=tempNorm2.x;
		VBOData[i*24+9]=tempNorm2.y;
		VBOData[i*24+10]=tempNorm2.z;
		VBOData[i*24+11]=(tempText2.x);
		VBOData[i*24+12]=(tempText2.y);
		VBOData[i*24+13]=tempVert2.x;
		VBOData[i*24+14]=tempVert2.y;
		VBOData[i*24+15]=tempVert2.z;

//		//For Vertex 3
		Vec tempNorm3=PtrN->at(tempFace.norm.third-1);
		Vec tempVert3=PtrV->at(tempFace.vert.third-1);
		Vec tempText3=PtrT->at(tempFace.text.third-1);
		VBOData[i*24+16]=tempNorm3.x;
		VBOData[i*24+17]=tempNorm3.y;
		VBOData[i*24+18]=tempNorm3.z;
		VBOData[i*24+19]=(tempText3.x);
		VBOData[i*24+20]=(tempText3.y);
		VBOData[i*24+21]=tempVert3.x;
		VBOData[i*24+22]=tempVert3.y;
		VBOData[i*24+23]=tempVert3.z;

			//std::cout <<i<<"=i "<<"\n";
		if (i+1==((int)PtrF->size()))
			std::cout<<"";
	}

    glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER,  numFace*8*sizeof(float),VBOData, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0);
}

mesh::~mesh()
{
	delete[] VBOData;
	glDeleteBuffers(1,&buffer);
}

void mesh::MeshDraw(Texture &textName,bool invisible,float tempx,float tempy,float tempz,float yrotate,float zrotate)
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
		glTranslatef(tempx,tempy,tempz);
		glRotatef(yrotate,0,1,0);// rotate the object 
		glRotatef(zrotate,0,0,1);// rotate the object 
		glBindBuffer( GL_ARRAY_BUFFER, buffer );
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glNormalPointer(GL_FLOAT,8*sizeof(float),0);//(void*)sizeof(objData)
			glTexCoordPointer(2,GL_FLOAT, 8*sizeof(float),(void*)(3*sizeof(float)));
			glVertexPointer( 3, GL_FLOAT, 8*sizeof(float), (void*)(5*sizeof(float)) );//( triangles or squares, type) 
			if (invisible==false){
				//glActiveTexture(GL_TEXTURE0 );
				//glUniform1i(ptrShader->
				
				//glUniform1i(textName.texture_handle, 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textName.texture_handle);
			}
			glDrawArrays( GL_TRIANGLES, 0, numFace*2);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer( GL_ARRAY_BUFFER, 0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}