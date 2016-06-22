#include "Vector3f.h"

float Vector3f::dotProduct(Vector3f &otherVector)
{
	//std::cout<< x<<"*"<<otherVector.x<<" + "<<y<<"*"<<otherVector.y<<" + "<<z<<"*"<<otherVector.z;
	return x*otherVector.x + y*otherVector.y + z*otherVector.z;
}

void Vector3f::operator* (float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}
	
Vector3f Vector3f::crossProduct (Vector3f &otherVector)
{
	/*float _x, _y, _z;

	_x = y*otherVector.z - z*otherVector.y;
	_y = z*otherVector.x - x*otherVector.z;
	_z = x*otherVector.y - y*otherVector.x;
	return Vector3f(x,y,z);*/
	//This runs faster, but its not neat.
	return Vector3f(y*otherVector.z - z*otherVector.y, z*otherVector.x - x*otherVector.z, x*otherVector.y - y*otherVector.x);
}

float Vector3f::angleDotProduct(Vector3f &otherVector)
{
	float numerator = this->dotProduct(otherVector);
	float denominator = this->findNorm() * otherVector.findNorm();
	return acos(numerator/denominator);
}

Vector3f Vector3f::operator+ (Vector3f &otherVector)
{
	return Vector3f(x + otherVector.x, y + otherVector.y, z + otherVector.z);
}

Vector3f Vector3f::operator-(Vector3f &otherVector)
{
	return Vector3f(x - otherVector.x, y - otherVector.y, z - otherVector.z);
}

float Vector3f::findNorm()
{
	return sqrt(x*x + y*y + z*z);
}

void Vector3f::normalize()
{
	float norm = sqrt(x*x + y*y + z*z);
	x /= norm;
	y /= norm;
	z /= norm;
}

Vector3f Vector3f::returnUnit()
{
	Vector3f unit = *this;
	unit.normalize();
	return unit;
}
float Vector3f::distance()
{
	return sqrt(x*x +  z*z);
}