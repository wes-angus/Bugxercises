/*
	OBJ Cheezburger
	By Dan Buckstein

	A really bad simple class to load and render an object from an OBJ file.
	If you want to use this, please be sure that your model is TRIANGULATED.
	Rendering takes place in IMMEDIATE MODE.
*/


#ifndef _OBJ_CHEEZBURGER_H
#define _OBJ_CHEEZBURGER_H


namespace Cheezburger
{

class OBJ
{
public:

	// constructor
	OBJ();
	OBJ(const char *objFile, int *resultOut = 0);

	// load OBJ
	// returns 1 if success, 0 if failure (invalid path, bad read...)
	int Load(const char *objFile);

	// release data
	// returns 1 if success, 0 if failure (i.e. no data to release)
	int Release();

	// render in immediate mode
	void Render() const;

	// get data sizes
	inline unsigned int GetVertexCount() const	{return vertexCount;}
	inline unsigned int GetTexcoordCount() const{return texcoordCount;}
	inline unsigned int GetNormalCount() const	{return normalCount;}
	inline unsigned int GetFaceCount() const	{return faceCount;}

private:

	// attributes
	float *vertex;
	float *texcoord;
	float *normal;

	// triangle structure to help with drawing
	struct tri
	{
		float *v0,  *v1,  *v2;
		float *vt0, *vt1, *vt2;
		float *vn0, *vn1, *vn2;
	} *face;

	// counts
	unsigned int vertexCount;
	unsigned int texcoordCount;
	unsigned int normalCount;
	unsigned int faceCount;

	// internal functions
	void Reset();
	void ParseTri(char *, tri *, int);
	int Partition(const char *);

};


}


#endif	// _OBJ_CHEEZBURGER