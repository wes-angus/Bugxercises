// the following code is modifed from bullet devil: the game made by Studio 8
// it can and will be reused by all previous members of studio 8 (last year)
// memember were Arash Ahkiani, AldwinCho, Brendan Drenkov, and Philip Ong 

#pragma once
#include "include\GL\glew.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include "Texture.h"
#include "Vector3f.h"

typedef struct Vec
{
	float x;
	float y;
	float z;
}Vec;

typedef struct facef
{// bascially a 3 int template which will take in 3 locations for the 3 data types of the faces
	int first;
	int second;
	int third;
}facef;

typedef struct OFace
{// a face contians the 3 locations in the array of the data types 
	facef vert;
	facef text;
	facef norm;
}OFace;

class object
{
public:
	object ();
	object (float x, float y, float z);
	~object();
	void loadobject(std::string filename);
	void draw(Texture &name);
	virtual void transx(float addx){
		x+=addx;
	}
	virtual void transz(float addz){
		z+=addz;
	}

	void changeLoc(Vector3f *otherV)
	{
		x=otherV->x;
		y=otherV->y;
		z=otherV->z;
	}

	void changeOrient(Vector3f *otherV)
	{// change speed 
		//std::cout<<otherV->x<<"=x "<<otherV->y<<"=y "<<otherV->z<<"=z "<<"\n";
		VecOrient->x= otherV->x;
		VecOrient->y= otherV->y;
		VecOrient->z= otherV->z;
	}
		
	void changeOrient(float tx, float ty,float tz) 
	{// change speed 
		VecOrient->x= tx;
		VecOrient->y= ty;
		VecOrient->z= tz;
	}
	void changeVelocity(Vector3f *otherV)
	{// change speed 
		Velocity->x= otherV->x;
		Velocity->y= otherV->y;
		Velocity->z= otherV->z;
	}
		
	void changeVelocity(float tx, float ty,float tz) 
	{// change speed 
		Velocity->x= tx;
		Velocity->y= ty;
		Velocity->z= tz;
	}

	void changeAcel(float tx, float ty,float tz) 
	{// change acceleration 
		acel->x= tx;
		acel->y= ty;
		acel->z= tz;
	}
	void CalCent(){// used to calculate the center, failed 
		Centx=(float)(Maxx+Minx)/2;
		Centy=(float)(Maxy+Miny)/2;
		Centz=(float)(Maxz+Minz)/2;
	}
	float returnxSizex()
	{
		return Maxx-Minx;
	}
	float returnxSizez()
	{
		return Maxz-Minz;
	}

	void findMaxMin();
	void SortMaxMin();
	void loadObjTexture(std::string fileName);
	//void update(float Delta);
	//void loadObjTexture(GLuint textureID);
	object* getObj();

	virtual bool collision(object &other);
	virtual void giveMinMax(object &objectSource);
	virtual void giveMinMaxOther(object &objectTarget,object &objectSource);

	float Speedfactor;
	float theataO,theata,theataz;
	float x,y,z;// the current position of the object 
	float Centx,Centy,Centz;// the center of the object 
	float initx,inity,initz;
	float Maxx,Minx,Maxy,Miny,Maxz,Minz;
	bool left, right;
	
	//AI
	float f_lerptime;
	bool b_seek;
	bool b_flee;
	bool lerp;
	Vector3f *v_Seek;
	Vector3f *v_Flee;
	Vector3f *v_initPos;
	Vector3f *v_Pos;
	//bool left;

	//more movement 
	float Lerp(float X0, float X1, float aDeltaT);
	void startSeek(Vector3f *v_pos);
	void startflee(Vector3f *v_flee);
	void startBoth(Vector3f *v_pos);
	void copyObject(object *copyObject);
	void AIStuff();
	void update(float deltaT);
	void lerpUpdate(Vector3f *v_loc2,Vector3f *v_loc1, float deltaT);
	void update(Vector3f *v_player,float deltaT,int speed,std::vector<object*> *otherboidsVector,Vector3f *Cage);
	int boidNum;

	Vector3f *VecOrient,*Velocity,*acel;

	//for morphin 
	void Vmorphing(object &ObjectPrevKeyFrame, object &ObjectNextKeyFrame, float delta);
	void Vmorphing(std::vector<object*> *objectVector, float delta);
	bool VmorphBool,VmorphCont; 
	int morphCounter;

	// below is for stuff 
	//sf::Image img_data;
	//GLuint texture_handle;

	int health;

	 unsigned long int num_vertex;
	 unsigned long int num_normal;
	 unsigned long int num_uv;
	 float *bverticies;
	 float *bnormals;
	 float *buvs;

	 std::vector<Vec> *PtrN;
	std::vector<Vec> *PtrV;
	std::vector<Vec> *PtrT;
	std::vector<OFace> *PtrF;

	std::vector<Vec> normals;
	std::vector<Vec> vertices;
	std::vector<Vec> textures;
	std::vector<OFace> faces;

protected:
	float MorphTime;
	bool loaded;

	
	/*std::vector<Vec> normals;
	std::vector<Vec> vertices;
	std::vector<Vec> textures;
	std::vector<OFace> faces;

	std::vector<Vec> *PtrN;
	std::vector<Vec> *PtrV;
	std::vector<Vec> *PtrT;
	std::vector<OFace> *PtrF;*/
};

struct MMesh : public object{
 unsigned long int num_vertex;
 unsigned long int num_normal;
 unsigned long int num_uv;
 float *vertices;
 float *normals;
 float *uvs;
};