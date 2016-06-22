// the following code is modifed from bullet devil: the game made by Studio 8
// it can and will be reused by all previous members of studio 8 (last year)
// memember were Arash Ahkiani, AldwinCho, Brendan Drenkov, and Philip Ong 

#include "Objects.h"
object::object()
{// default constructor 
	PtrN=&normals;
	PtrV=&vertices;
	PtrT=&textures;
	PtrF=&faces;
	loaded=false;
	left=true;
	lerp=false;
	right=false;

	//rotations or physics 
	theata=0;
	theataz=0;
	VecOrient= new Vector3f(0,0,-1);
	Velocity= new Vector3f(0,0,-1);
	acel= new Vector3f(0,0,0);

	//loctaions 
	x=initx=0;
	y=inity=0;
	z=initz=0;
	Centx=0;
	Centy=0;
	Centz=0;
	Speedfactor=0;
	theataO=0;


	// sets the max/mins
	Maxx=-100;
	Minx=100;
	Maxy=-100;
	Miny=100;
	Maxz=-100;
	Minz=100;

	//for morphing 
	morphCounter=0;
	MorphTime=0;
	VmorphCont=false;
	VmorphBool= false; 

	//loadObjTexture("images/box.bmp");
}

object::object(float startx, float starty, float startz)
{
	PtrN=&normals;
	PtrV=&vertices;
	PtrT=&textures;
	PtrF=&faces;
	loaded=false;
	left=true;
	lerp=false;
	right=false;

	//rotations/physics
	VecOrient= new Vector3f(0,0,-1);
	Velocity= new Vector3f(0,0,-1);
	acel= new Vector3f(0,0,0);
	theata=0;
	theataz=0;
	theataO=0;

	//loctaions 
	x=initx=startx;
	y=inity=starty;
	z=initz=startz;
	Centx=0;
	Centy=0;
	Centz=0;
	Speedfactor=0;

	// sets the max/mins
	Maxx=-100;
	Minx=100;
	Maxy=-100;
	Miny=100;
	Maxz=-100;
	Minz=100;

	//for morphing 
	morphCounter=0;
	MorphTime=0;
	VmorphCont=false;
	VmorphBool= false; 

	health = 2;
	//loadObjTexture("images/box.bmp");
}

//object::~object()
//{
//	PtrN->clear();
//	PtrV->clear();
//	PtrT->clear();
//	PtrF->clear();
//}

void object::findMaxMin()
{
	float avX=0,avY=0,avZ=0;
	for(int i=0;i<((int)PtrV->size());i++)
	{
		Vec TempV= PtrV->at(i);
		//Vector3f TempV;
		//TempV= PtrV->at(i);
		avX=TempV.x;
		avY=TempV.y;
		avZ=TempV.z;
		// for all xs
		if (avX>Maxx)
			Maxx=avX;
		if (avX<Minx)
			Minx=avX;
		// for all ys
		if (avY>Maxy)
			Maxy=avY;
		if (avY<Miny)
			Miny=avY;
		// for all zs
		if (avZ>Maxz)
			Maxz=avZ;
		if (avZ<Minz)
			Minz=avZ;
	}
}

void object::SortMaxMin()
{
	std::vector<Vec> tempVV;
	tempVV=vertices;// copies it because we don't want to modify it 
	int j;
	float key;
	for(int i=1;i<tempVV.size();i++)
	{// main loop 
		j = i-1;
		key = tempVV.at(i).x;// array 
        
		while( j >= 0 && tempVV.at(j).x > key ){
			tempVV.at(j+1).x = tempVV.at(j).x;
			j--;
		}
		tempVV.at(j+1).x =key;
    }
	Minx=tempVV.at(0).x;
	Maxx=tempVV.at(tempVV.size()-1).x;

	//for ys 
	for(int i=1;i<tempVV.size();i++)
	{// main loop 
		j = i-1;
		key = tempVV.at(i).y;// array 
        
		while( j >= 0 && tempVV.at(j).y > key ){
			tempVV.at(j+1).y = tempVV.at(j).y;
			j--;
		}
		tempVV.at(j+1).y =key;
    }
	Miny=tempVV.at(0).y;
	Maxy=tempVV.at(tempVV.size()-1).y;

	//for zs 
	for(int i=1;i<tempVV.size();i++)
	{// main loop 
		j = i-1;
		key = tempVV.at(i).z;// array 
        
		while( j >= 0 && tempVV.at(j).z > key ){
			tempVV.at(j+1).z = tempVV.at(j).z;
			j--;
		}
		tempVV.at(j+1).z =key;
    }
	Minz=tempVV.at(0).z;
	Maxz=tempVV.at(tempVV.size()-1).z;
}

bool object::collision(object &other)
{// basically if a point is less than x,y,z below checks x coord 
	//std::cout<<"this max="<<this->Maxx+this->x<<" this min="<<this->x-this->Minx<<" other max="<<other.Maxx+other.x<<" other min="<<other.x-other.Minx<<"\n";
	if(((this->Maxx+this->x)>=(other.Maxx+other.x) && (this->x+this->Minx)<=(other.Maxx+other.x))||// compares the other (current) min/max x location 
	   ((this->Maxx+this->x)>=(other.x+other.Minx) && (this->x+this->Minx)<=(other.x+other.Minx))||
	   ((other.x+other.Maxx)>=(this->Maxx+this->x) && (other.x+other.Minx)<=(this->Maxx+this->x))||// compares our (current) min/max x location 
	   ((other.x+other.Maxx)>=(this->x+this->Minx) && (other.x+other.Minx)<=(this->x+this->Minx)))
		{// below checks z coord 
		if(((this->Maxz+this->z)>=(other.Maxz+other.z) && (this->z+this->Minz)<=(other.Maxz+other.z))||// compares the other (current) min/max z location 
		   ((this->Maxz+this->z)>=(other.z+other.Minz) && (this->z+this->Minz)<=(other.z+other.Minz))||
		   ((other.z+other.Maxz)>=(this->Maxz+this->z) && (other.z+other.Minz)<=(this->Maxz+this->z))||// compares our (current) min/max z location 
		   ((other.z+other.Maxz)>=(this->z+this->Minz) && (other.z+other.Minz)<=(this->z+this->Minz)))
			{// below checks y coord 
			if(((this->Maxy+this->y)>=(other.Maxy+other.y) && (this->y+this->Miny)<=(other.Maxy+other.y))||// compares the other (current) min/max z location 
				((this->Maxy+this->y)>=(other.y+other.Miny) && (this->y+this->Miny)<=(other.y+other.Miny))||
				((other.y+other.Maxy)>=(this->Maxy+this->y) && (other.y+other.Miny)<=(this->Maxy+this->y))||// compares our (current) min/max z location 
				((other.y+other.Maxy)>=(this->y+this->Miny) && (other.y+other.Miny)<=(this->y+this->Miny)))
				{
				return true;
				}
			}
		}
	else // if not true return false 
	{
		return false;
	}
}

void object::giveMinMax(object &other)
{
	Maxx=other.Maxx;
	Minx=other.Minx;
	Maxy=other.Maxy;
	Miny=other.Miny;
	Maxz=other.Maxz;
	Minz=other.Minz;
}

void object::giveMinMaxOther(object &target,object &source)
{
	target.Maxx=source.Maxx;
	target.Minx=source.Minx;
	target.Maxy=source.Maxy;
	target.Miny=source.Miny;
	target.Maxz=source.Maxz;
	target.Minz=source.Minz;
}

void object::loadobject(std::string filename)
{
	std::clock_t Start;
	Start=clock();
	std::ifstream i(filename);
	if (!i)
	{
		std::cout<<"can't open "<<filename;
		
	}

	std::string line;
	int count=0,countv=0;
	while(std::getline(i,line))
	{// does it to the end 
		if (line.substr(0,2) == "v ") 
		{//vertices 
			std::istringstream s(line.substr(2));// strange piece of code here, s if outputted will give address
			Vec v;// store it in the "vec" 

			s >> v.x; 
			s >> v.y;
			s >> v.z; 
			//std::cout<<" "<<v.x<<" y="<<v.y<<" z="<<v.z<<"\n";
			//countv++;
			//std::cout<<countv<<"\n";
			PtrV->push_back(v);//push back the "vec" into the list 
		}
		if (line.substr(0,2) == "vn") 
		{//normals
			std::istringstream s(line.substr(2));// strange piece of code here, s if outputted will give address
			Vec v;// store it in the "vec" 
			s >> v.x; 
			s >> v.y;
			s >> v.z; 
			//std::cout<<v.x<<" y="<<v.y<<" z="<<v.z<<"\n";
			PtrN->push_back(v);//push back the "vec" into the list 
		}
		if (line.substr(0,2) == "vt") 
		{//textures
			std::istringstream s(line.substr(2));// strange piece of code here, s if outputted will give address
			Vec v;// store it in the "vec" 
			s >> v.x; 
			s >> v.y;
			s >> v.z; 
			//std::cout<<v.x<<" y="<<v.y<<" z="<<v.z<<"\n";
			PtrT->push_back(v);//push back the "vec" into the list 
		}
		if (line.substr(0,2) == "f ") 
		{//face 
			char* c_string;// must use this to convert to c string thingy
			c_string = new char [line.size()+1];
			strcpy (c_string, line.c_str());

			int v1,t1,n1,v2,t2,n2,v3,t3,n3,v4,t4,n4;
			// below is the most craziest, but most efficent st!!ring scanning function in c++ O.O HOLY SHIT!
			sscanf_s(c_string,"f %d/%d/%d %d/%d/%d %d/%d/%d",&v1,&t1,&n1,&v2,&t2,&n2,&v3,&t3,&n3);
			//vertex/texture/normal
			OFace tempface;
 
			tempface.vert.first=v1;
			tempface.vert.second=v2;
			tempface.vert.third=v3;
 
			tempface.norm.first=n1;
			tempface.norm.second=n2;
			tempface.norm.third=n3;
 
			tempface.text.first=t1;
			tempface.text.second=t2;
			tempface.text.third=t3;

			PtrF->push_back(tempface);
			count++;
		}
		
	}
	loaded=true;
	//std::cout<<" time="<<((clock()-Start)/ (double)CLOCKS_PER_SEC)<<" secs"<<"\n";
	findMaxMin();

	// for ai 
	b_seek=false ;
  b_flee=false ;
	v_initPos=NULL;
	v_Seek=NULL;
	v_Flee=NULL;
	v_Pos=new Vector3f(0,0,0);
	f_lerptime=0;
}

void object::draw(Texture &textName)
{
	if (loaded==true){
	if ((left==false)&&(right==false))
		theataz=0;
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	glTranslatef(x,y,z);
	glRotatef(theata,0,1,0);
	glRotatef(theataz,0,0,1);
	//glTranslatef(Centx,0,0);
	glBindTexture(GL_TEXTURE_2D, textName.texture_handle);

	for(int i=0;i<((int)PtrF->size());i++)	//
	{// while the object is less than the faces 
		OFace tempFace;
		tempFace=PtrF->at(i);
		//std::cout<<"currently="<<i<<" out of "<<PtrF->size()<<"\n";
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
//			//Recover the vertex, normal and UV from the respective Managers 
//			//For Vertex 1
//			int vertId = faces[i].GetVertexIndex1();
//			int normId = faces[i].GetNormalIndex1();
//			int texId = faces[i].GetUVIndex1();
// 
//			Point3D vert = vertManager.GetVertex(vertId);
//			Normal norm = normManager.GetNormal(normId);
//			UV tex = uvManager.GetUV(texId);
			Vec tempNorm1=PtrN->at(tempFace.norm.first-1);
			Vec tempVert1=PtrV->at(tempFace.vert.first-1);
			Vec temptex1=PtrT->at(tempFace.text.first-1);

//			glTexCoord2f(tex.GetU(),tex.GetV());
			glNormal3f(tempNorm1.x,tempNorm1.y,tempNorm1.z);
			glTexCoord2f(temptex1.x, temptex1.y); 
			glVertex3f(tempVert1.x,tempVert1.y,tempVert1.z);
 
//			//For Vertex 2
//			vertId = faces[i].GetVertexIndex2();
//			normId = faces[i].GetNormalIndex2();
//			texId = faces[i].GetUVIndex2();
// 
//			vert = vertManager.GetVertex(vertId);
			Vec tempNorm2=PtrN->at(tempFace.norm.second-1);
			Vec tempVert2=PtrV->at(tempFace.vert.second-1);
			Vec temptex2=PtrT->at(tempFace.text.second-1);

//			glTexCoord2f(tex.GetU(),tex.GetV());
			glNormal3f(tempNorm2.x,tempNorm2.y,tempNorm2.z);
			glTexCoord2f(temptex2.x, temptex2.y); 
			glVertex3f(tempVert2.x,tempVert2.y,tempVert2.z);

// 
//			//For Vertex 3
//			vertId = faces[i].GetVertexIndex3();
//			normId = faces[i].GetNormalIndex3();
//			texId = faces[i].GetUVIndex3();
// 
//			vert = vertManager.GetVertex(vertId);
			 Vec tempNorm3=PtrN->at(tempFace.norm.third-1);
			 Vec tempVert3=PtrV->at(tempFace.vert.third-1);
			 Vec temptex3=PtrT->at(tempFace.text.third-1);

//			glTexCoord2f(tex.GetU(),tex.GetV());
			glNormal3f(tempNorm3.x,tempNorm3.y,tempNorm3.z);
			glTexCoord2f(temptex3.x, temptex3.y); 
			glVertex3f(tempVert3.x,tempVert3.y,tempVert3.z);
			//std::cout<<"third x="<<tempVert3.x<<" y="<<tempVert3.y<<" z="<<tempVert3.z<<"\n";
// 
		glEnd();
	}
	glPopMatrix();
	}


}

//void object::copyObject(object *copyObject)
//{
//	PtrN=copyObject->PtrN;
//	PtrV=copyObject->PtrV;
//	PtrT=copyObject->PtrT;
//	PtrF=copyObject->PtrF;
//}

void object::Vmorphing(object &PrevKeyFrame, object &NextKeyFrame, float delta)
{// CAT 
	for(int i=0;i<((int)this->PtrV->size());i++)	//
	{
		//std::cout<<"num"<<i<<"\n";
		this->PtrV->at(i).x=PrevKeyFrame.PtrV->at(i).x*(1-MorphTime) + NextKeyFrame.PtrV->at(i).x*(MorphTime);
		this->PtrV->at(i).y=PrevKeyFrame.PtrV->at(i).y*(1-MorphTime) + NextKeyFrame.PtrV->at(i).y*(MorphTime);
		this->PtrV->at(i).z=PrevKeyFrame.PtrV->at(i).z*(1-MorphTime) + NextKeyFrame.PtrV->at(i).z*(MorphTime);
	}
	for(int i=0;i<(this->PtrN->size());i++)	//
	{
		this->PtrN->at(i).x=PrevKeyFrame.PtrN->at(i).x*(1-MorphTime) + NextKeyFrame.PtrN->at(i).x*(MorphTime);
		this->PtrN->at(i).y=PrevKeyFrame.PtrN->at(i).y*(1-MorphTime) + NextKeyFrame.PtrN->at(i).y*(MorphTime);
		this->PtrN->at(i).z=PrevKeyFrame.PtrN->at(i).z*(1-MorphTime) + NextKeyFrame.PtrN->at(i).z*(MorphTime);
	}

	for(int i=0;i<(this->PtrT->size()-2);i++)	//
	{
		this->PtrT->at(i).x=PrevKeyFrame.PtrT->at(i).x*(1-MorphTime) + NextKeyFrame.PtrT->at(i).x*(MorphTime);
		this->PtrT->at(i).y=PrevKeyFrame.PtrT->at(i).y*(1-MorphTime) + NextKeyFrame.PtrT->at(i).y*(MorphTime);
		this->PtrT->at(i).z=PrevKeyFrame.PtrT->at(i).z*(1-MorphTime) + NextKeyFrame.PtrT->at(i).z*(MorphTime);
	}

	MorphTime+=delta;
}

void object::Vmorphing(std::vector<object*> *OVector, float delta)
{
	if (VmorphBool==true)
	{
		Vmorphing(*OVector->at(morphCounter),*OVector->at(morphCounter+1),delta);
		if (MorphTime>1)
		{// stop it from continuing 
			morphCounter++;
			MorphTime=0;
			if ((morphCounter+1)==OVector->size())
			{
				morphCounter=0;
			}
		}
	}
}

void object::startSeek(Vector3f *v_pos)
{
	f_lerptime=0;
	b_seek=true;
	v_Seek=v_pos;
	v_initPos=new Vector3f(x,y,z);
	std::cout<<"start pos:"<<v_initPos->x<<"=x "<<v_initPos->y<<"=y "<<v_initPos->z<<"=z "<<std::endl;
	v_Seek->normalize();
}

void object::startflee(Vector3f *v_pos)
{
	f_lerptime=0;
	b_seek=true;
	v_Seek=v_pos;
	v_initPos=new Vector3f(x,y,z);
	v_Seek->normalize();
}

void object::startBoth(Vector3f *v_pos)
{
	f_lerptime=0;
	b_seek=true;
	v_Seek=v_pos;
	v_initPos=new Vector3f(x,y,z);
	v_Seek->normalize();
}

float object::Lerp(float X0, float X1, float aDeltaT)
{
	return X0+((X1-X0)*(aDeltaT));
}

void object::update(float deltaT)
{// player 
		// lerp stuffff 
	if (b_seek)
	{
		float* Posx= new float(Lerp(v_initPos->x,v_Seek->x,f_lerptime));
		float* Posy= new float(Lerp(v_initPos->y,v_Seek->y,f_lerptime));
		float* Posz= new float(Lerp(v_initPos->z,v_Seek->z,f_lerptime));
		Vector3f *v_velocity=new Vector3f((*Posx)*2,0,(*Posz)*2);
		//*v_velocity=	*v_velocity->operator*(5);
		changeLoc(v_velocity);

		f_lerptime=f_lerptime+deltaT;
		//std::cout<<Posx<<"=x ";
		//std::cout<<Posy<<"=y ";
		//std::cout<<Posz<<"=z "<<std::endl;
		if (f_lerptime>float(1.0))
		{
			std::cout<<x<<"=x ";
			std::cout<<y<<"=y ";
			std::cout<<z<<"=z "<<std::endl;
			b_seek=false;
		}
		//*v_fearPosition=eNode->getPosition();
	}
	v_Pos=new Vector3f(x,y,z);
}

void object::lerpUpdate(Vector3f *v_loc1,Vector3f *v_loc2, float deltaT)
{// player 
		// lerp stuffff 
	if(lerp)
	{
		float Posx,Posy,Posz;
		if (left)
		{
			Posx= Lerp(v_loc1->x,v_loc2->x,f_lerptime);
			Posy= Lerp(v_loc1->y,v_loc2->y,f_lerptime);
			Posz= Lerp(v_loc1->z,v_loc2->z,f_lerptime);
		}
		else
		{
			Posx= Lerp(v_loc2->x,v_loc1->x,f_lerptime);
			Posy= Lerp(v_loc2->y,v_loc1->y,f_lerptime);
			Posz= Lerp(v_loc2->z,v_loc1->z,f_lerptime);
		}
		Vector3f *v_velocity=new Vector3f(Posx*2,0,Posz*2);
		changeLoc(v_velocity);

		f_lerptime=f_lerptime+deltaT;
		//std::cout<<Posx<<"=x ";
		//std::cout<<Posy<<"=y ";
		//std::cout<<Posz<<"=z "<<std::endl;
		if (f_lerptime>float(1.0))
		{
			f_lerptime=0;
			if (left)
			 left=false;
			else
			 left=true;
		}
		//*v_fearPosition=eNode->getPosition();
	}
	v_Pos=new Vector3f(x,y,z);
}

object* object::getObj()
{
	Vector3f* tempVec = new Vector3f(0,0,0);
	object *obj1=new object(tempVec->x,tempVec->y,tempVec->z);
	obj1 = this;
	return obj1;
}

void object::update(Vector3f *v_player,float deltaT,int speed,std::vector<object*> *otherBoids,Vector3f *cage)
{
		/// rotaion setup
	Vector3f *turrentOrient= new Vector3f(0,0,-1);
	Vector3f *turrentVec = new Vector3f(0,0,0);

	Vector3f *PlayerVec1 = new Vector3f(0,0,0);
	PlayerVec1->x=(v_player->x-this->x);
	PlayerVec1->z=(v_player->z-this->z);
	float dist= PlayerVec1->distance();
	float playerlocx=0;
	// comparsion

	turrentVec=new Vector3f(*PlayerVec1);
	playerlocx=v_player->x;
	//turrentVec=new Vector3f(*PlayerVec1);

	// rotation cal
	theata=turrentOrient->angleDotProduct(*turrentVec);
	theata=theata*(180/(float)3.1456);
	if (playerlocx>this->x)
	{// calculates the missing part and fills it in don't know why 
		GLfloat missing=(180-theata);
		theata=theata+missing*2;
	}

	//rotation setting 
	this->theata=theata;
	turrentVec->normalize();
	//turrentVec->operator*(-1);
	this->changeVelocity(turrentVec);
	Velocity->operator*(speed);

	// checking for other enemies 
	for (int i=0; i+1<=otherBoids->size();i++)
	{
		if (boidNum!=i){
		float dx= this->x-otherBoids->at(i)->x;
		float dz= this->z-otherBoids->at(i)->z;
		float radius=( dx * dx )  + ( dz * dz );
		float targetRadius=15;
		if (radius<targetRadius)// ciricle collision 
		{
			Velocity->x+=20*((targetRadius-radius)/targetRadius)*dx;
			Velocity->z+=20*((targetRadius-radius)/targetRadius)*dz;
		}
		}
	}

	//Flee from Cage
	Vector3f *flee= new Vector3f(cage->x-this->x,0,cage->z-this->z);
	float cageRadius= (flee->x*flee->x) +(flee->z*flee->z);
	float cagetargetRadius=50;
	if (cageRadius<cagetargetRadius)// ciricle collision 
	{
			flee->normalize();
			flee->operator*(10);
		//	//flee->operator*(20*((targetRadius-cageRadius)/targetRadius));
			Velocity= new Vector3f(Velocity->operator-(*flee));
	}


	//movement
	x+=(Velocity->x*deltaT);
	z+=Velocity->z*deltaT;
	v_Pos=new Vector3f(x,y,z);
}