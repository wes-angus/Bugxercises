#pragma once
#include <math.h>
#include <iostream>

class Vector3f
{
public:
	float x, y, z;
	
	Vector3f();
	Vector3f(float _x, float _y, float _z)
	{
		x = initX = _x;
		y = initY = _y;
		z = initZ = _z;
		
	}
	Vector3f(Vector3f &otherVector)
	{
		x = otherVector.x;
		y = otherVector.y;
		z = otherVector.z;
	}
	
	void reset()
	{
		x = initX;
		y = initY;
		z = initZ;
	}

	void set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	
	float dotProduct(Vector3f &otherVector);
	Vector3f crossProduct (Vector3f &otherVector);

	float angleDotProduct(Vector3f &otherVector);
	
	void operator* (float scalar);
	Vector3f operator+(Vector3f &otherVector);
	Vector3f operator-(Vector3f &otherVector);
	void operator=(Vector3f otherVector)
	{
		x = otherVector.x;
		y = otherVector.y;
		z = otherVector.z;
	}

	float findNorm();
	void normalize();
	Vector3f returnUnit();
	float distance();
	
private:
	float initX, initY, initZ;
};