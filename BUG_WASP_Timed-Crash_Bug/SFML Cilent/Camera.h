#pragma once
#include "Vector3f.h"
class GameCamera
{
	public:
		GameCamera (void)
		{
			///////////////CAMERA//////////////////////
			//below is for camera's location aka eyes 
			cx = 0.0f;
			cy = 0.0f;
			cz = 0.0f;
			//below is for camera center/ the point its looking at
			Px = 0.0f;
			Py = 0.0f;
			Pz = -500.0f;
			//below is for camera normal 
			Nx = 0.0f;
			Ny = 1.0f;
			Nz = 0.0f;
			movingCamSet=false;
			doneMoving=false;
			lifeTime=0;
		}
		~GameCamera (void)
		{
		}
		void move(float x,float y,float z)
		{
			cx +=x;
			cy +=y;
			cz +=z;

			Px +=x;
			Py +=y;
			Pz +=z;

		}
		void directSet(float x,float y,float z)
		{
			cx =x;
			cy =y;
			cz =z;

			Px =x;
			Py =y-150;
			Pz =z;
		}
		void SetMovingCam(float x1,float y1,float z1,float x2,float y2,float z2 )
		{
			doneMoving=false;
			movingCamSet=true;
			initCam= new Vector3f(x1,y1,z1);
			targetCam= new Vector3f(x2,y2,z2);
			lifeTime=0;
		}
		void update (float delta)
		{// does it on if there is a camera transition
			if (movingCamSet==true)
			{// transition using LERP!
				lifeTime+=delta*3;
				float tempx,tempy,tempz;
				tempx=initCam->x*(1-lifeTime) + targetCam->x*lifeTime;
				tempy=initCam->y*(1-lifeTime) + targetCam->y*lifeTime;
				tempz=initCam->z*(1-lifeTime) + targetCam->z*lifeTime;
				directSet(tempx,tempy,tempz);
				if (lifeTime > (float)1)
				{
					movingCamSet=false;
					doneMoving=true;
				}
			}
		}
		bool returnmovingCamSet()
		{
			return movingCamSet;
		}
		bool returndoneMoving()
		{
			return doneMoving;
		}
		void frontView()
		{//reset to front view 
			//below is for camera's location aka eyes 
			cx = 0.0f;
			cy = 0.0f;
			cz = 0.0f;
			//below is for camera center/ the point its looking at
			Px = 0.0f;
			Py = 0.0f;
			Pz = -500.0f;
			//below is for camera normal 
			Nx = 0.0f;
			Ny = 1.0f;
			Nz = 0.0f;
		}
		void topDownview()
		{
			//below is for camera's location aka eyes 
			cx = 0.0f;
			cy = 100.0f;
			cz = 0.0f;
			//below is for camera center/ the point its looking at
			Px = 0.0f;
			Py = -100.0f;
			Pz = 0.0f;
			//below is for camera normal 
			Nx = 0.0f;
			Ny = 0.0f;
			Nz = -1.0f;
		}
		void returnValues() 
		{
			std::cout<<cx<<" y="<<cy<<" z="<<cz<<" px="<<Px<<" py="<<Py<<" pz="<<Pz<<"\n";
		}
		void use()
		{
			gluLookAt(cx,cy,cz,Px,Py,Pz,Nx,Ny,Nz);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		void useInverse()
		{
			gluLookAt(cx,cy,cz,Px,Py,Pz,Nx,Ny,-Nz);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	private:
		float lifeTime;
		bool movingCamSet,doneMoving;
		Vector3f *initCam,*targetCam;
		//below is for camera's location aka eyes 
		GLfloat cx ;
		GLfloat cy ;
		GLfloat cz ;
		//below is for camera center/ the point its looking at
		GLfloat Px ;
		GLfloat Py ;
		GLfloat Pz ;
		//below is for camera normal 
		GLfloat Nx ;
		GLfloat Ny ;
		GLfloat Nz ;
};
