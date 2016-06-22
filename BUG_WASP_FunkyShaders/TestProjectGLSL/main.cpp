/*
	Test Project GLSL

	Sample code that tests GLUT, GLEW and GLSL Cheezburger.
*/



#include <time.h>
#include <iostream>
using namespace std;


#include <GL/glew.h>
#include <GL/glut.h>

#include <GL/GLSLCheezburger.h>

#include <IL/ilut.h>


#include "OBJCheezburger.h"




//-----------------------------------------------------------------------------
// variables

static double fps = 60.0;
static double spf = 1.0 / fps;

static unsigned int windowPosX;
static unsigned int windowPosY;
static unsigned int windowWidth = 1280;
static unsigned int windowHeight = 720;

static double aspectRatio = (double)windowWidth/(double)windowHeight;
static double fovy = 30.0;



//-----------------------------------------------------------------------------
// OpenGL callbacks

void Display();
void Resize(int x, int y);
void Timer(int value);

void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);


void Init();
void Update(double dt);
void Render();



//-----------------------------------------------------------------------------
// handles and objects


// FBO
unsigned int sceneFBO;
unsigned int sceneColour[2];	// colour(0) and normals(1)
unsigned int sceneDepth;


// programs
unsigned int toonProgram;
int toon_inputImage;
int toon_qmap;

unsigned int edgeProgram;
int edge_inputColour;
int edge_inputNormal;
int edge_inputDepth;
int edge_pixelSize;


// loaded image handles
unsigned int HoG_bot_tex;
unsigned int HoG_bot_toon_tex;
unsigned int HoG_tower_tex;
unsigned int smiley_tex;
unsigned int blue_tex;
unsigned int qMap;


// objects
Cheezburger::OBJ HoG_bot, HoG_tower, sphereOBJ;



// returns FBO handle
// arguments: option to use colour, depth
// pointers to our texture handles
// dimensions
//****REMEMBER: all we are storing is DATA!
//****ENCAPSULATE THIS PROCEDURE IN A STRUCT/CLASS
//	- STORE RELEVANT INFORMATION ABOUT YOUR FBO
unsigned int CreateFBO(unsigned int numColourTargets, bool useDepth, 
			unsigned int *colourTextureListOut, 
			unsigned int *depthTextureOut, 
			unsigned int width, unsigned int height, 
			bool useLinearFiltering, bool useHDR)
{
	if (!numColourTargets && !useDepth)
		return 0;


	unsigned int fboHandle;

	// generate FBO in graphics memory
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);


	GLint texParam = useLinearFiltering ? GL_LINEAR : GL_NEAREST;
	GLint internalFormat = useHDR ? GL_RGBA16 : GL_RGBA8;
	GLint internalType = useHDR ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;


	// create a texture for colour storage
	if (numColourTargets && colourTextureListOut)
	{
		//****QUERY THE MAXIMUM NUMBER OF RENDER TARGETS!
		// implement this!

		// proceed with texture creation
		glGenTextures(numColourTargets, colourTextureListOut);
		for ( unsigned int i = 0; i < numColourTargets; ++i )
		{
			glBindTexture(GL_TEXTURE_2D, colourTextureListOut[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, 
				GL_RGBA, internalType, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, (GL_COLOR_ATTACHMENT0 + i), 
				GL_TEXTURE_2D, colourTextureListOut[i], 0);
		}
		//**** NOT USING MRT ****
		// EXTEND THIS PART TO INCLUDE MULTIPLE COLOUR TARGETS
		// - back buffer is only capable of storing ONE render target; this 
		//	is why we use FBO's: so that we can have more if needed
	}

	// create a texture for depth storage
	if (useDepth && depthTextureOut)
	{
		glGenTextures(1, depthTextureOut);
		glBindTexture(GL_TEXTURE_2D, *depthTextureOut);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, 
			GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
			GL_TEXTURE_2D, *depthTextureOut, 0);
	}


	// clear for takeoff... sry :p
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("\n You fail. No cheezburger for you.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteFramebuffers(1, &fboHandle);
		glDeleteTextures(numColourTargets, colourTextureListOut);
		glDeleteTextures(1, depthTextureOut);
		memset(colourTextureListOut, 0, numColourTargets*sizeof(int));
		*depthTextureOut = 0;
		return 0;
	}

	// finished: good practice is to disable stuff because this is the state 
	// that we had when we started this process
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return fboHandle;
}



//-----------------------------------------------------------------------------
// simulation functions and variables

struct Object
{
	float tx, ty, tz;	// translation
	float rx, ry, rz;	// rotation
	float time;			// animation parameter
};

// scene objects
Object bot = { 0 };
Object sphere = { 0 };

// viewer objects
Object light = { 5.0f, 5.0f, 5.0f, 0.0f };		// fixed position
Object camera = { 0 };


// update/animate the objects
void BOT_UPDATE(float dt)
{
	bot.time += dt;
	bot.tx = 4.0f * cosf(bot.time);	// make it orbit around the center
	bot.ty = 5.0f + 0.25f * cosf(bot.time);	// and bob up and down
	bot.tz = -4.0f * sinf(bot.time);
	bot.ry = 150.0f * bot.time;	// and spin
}

void SPHERE_UPDATE(float dt)
{
	sphere.time += dt;
	sphere.tx = 8.0f * cosf(0.2f*sphere.time);
	sphere.ty = 2.0f + sinf(sphere.time);
	sphere.tz = 8.0f * sinf(0.2f*sphere.time);
}

void CAMERA_UPDATE(float dt)
{
	camera.time += 0.25f * dt;
	camera.tx = 25.0f * cosf(camera.time);
	camera.ty = 25.0f;
	camera.tz = -25.0f * sinf(camera.time);
}


// render the objects
void BOT_RENDER()
{
	glBindTexture(GL_TEXTURE_2D, HoG_bot_toon_tex);

	HoG_bot.Render();
}

void SPHERE_RENDER()
{
	glBindTexture(GL_TEXTURE_2D, smiley_tex);

	sphereOBJ.Render();
}

void LIGHT_RENDER()
{
	glBindTexture(GL_TEXTURE_2D, blue_tex);

	sphereOBJ.Render();
}

void GROUND_RENDER()
{
	glColor3f(0.5f, 0.5f, 0.5f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-8.0f, 0.0f,  8.0f);
		glVertex3f( 8.0f, 0.0f,  8.0f);
		glVertex3f(-8.0f, 0.0f, -8.0f);
		glVertex3f( 8.0f, 0.0f, -8.0f);
	glEnd();
}

void TOWER_RENDER()
{
	glBindTexture(GL_TEXTURE_2D, HoG_tower_tex);

	HoG_tower.Render();
}


// precomputed model transforms for each object
void BOT_MULT_MODEL_MATRIX()
{
	glTranslatef(bot.tx, bot.ty, bot.tz);
	glRotatef(bot.ry, 0.0f, 1.0f, 0.2f);
}

void SPHERE_MULT_MODEL_MATRIX()
{
	glTranslatef(sphere.tx, sphere.ty, sphere.tz);
	glRotatef(100.0f * sphere.time, sphere.tx, sphere.ty, sphere.tz);
}

void LIGHT_MULT_MODEL_MATRIX()
{
	glTranslatef(light.tx, light.ty, light.tz);
}


// pre-defined view matrices for different viewpoints: light and observer
void LIGHT_MULT_VIEW_MATRIX()
{
	gluLookAt(light.tx, light.ty, light.tz,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
}

void CAMERA_MULT_VIEW_MATRIX()
{
	gluLookAt(camera.tx, camera.ty, camera.tz,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
}


// pre-defined projection matrices for different viewpoints
void LIGHT_MULT_PROJECTION_MATRIX()
{
	gluPerspective(90.0, 1.0, 7.0, 1000.0);
}

void CAMERA_MULT_PROJECTION_MATRIX()
{
	gluPerspective(fovy, aspectRatio, 7.0, 1000.0);
}


// multiply bias matrix: scales by half and translates by half on all axes
void GLOBAL_MULT_BIAS_MATRIX()
{
	const float bias[16] = {
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0, 
		0.0, 0.0, 0.5, 0.0, 
		0.5, 0.5, 0.5, 1.0 
	};
	glMultMatrixf(bias);
}



//-----------------------------------------------------------------------------
// main functions

void InitGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Test Project GLSL");


	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutTimerFunc(0, Timer, 1);

	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);


	// other GL stuff
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);


	// initialize extensions
	glewInit();


	// check OpenGL version
	const unsigned char *version = glGetString(GL_VERSION);

}


int main(int argc, char **argv)
{
	// setup rendering
	InitGL(argc, argv);


	// initial load
	Init();


	// start
	glutMainLoop();
}



//-----------------------------------------------------------------------------
// callback definitions

void Display()
{
}

void Resize(int x, int y)
{
	windowWidth = x;
	windowHeight = y;
	aspectRatio = (double)x / (double)y;
}

void Timer(int value)
{
	Update(spf);
	Render();

	glutTimerFunc(1000.0*spf, Timer, value);
	glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		// quit app
	case 27: // esc
		exit(0);
		break;
	}
}

void KeyboardUp(unsigned char key, int x, int y)
{
}

void Mouse(int button, int state, int x, int y)
{
}

void Motion(int x, int y)
{
}



//-----------------------------------------------------------------------------
// init, update and render

void Init()
{
	// FBOs and new shaders
	sceneFBO = CreateFBO(2, 1, sceneColour, &sceneDepth, 
		windowWidth, windowHeight, 1, 1);

	{
		const char *vert = "resource/shaders/toon_v.glsl";
		const char *frag = "resource/shaders/toon_f.glsl";

		toonProgram = GLSLProgramFromFiles(1,&vert,0,  1,&frag,0);

		toon_inputImage = glGetUniformLocation(toonProgram, "inputImage");
		toon_qmap = glGetUniformLocation(toonProgram, "qmap");
	}

	{
		const char *frag = "resource/shaders/edge_f.glsl";

		edgeProgram = GLSLProgramFromFiles(0,0,0,  1,&frag,0);

		edge_inputColour = glGetUniformLocation(edgeProgram, "inputColour");
		edge_inputNormal = glGetUniformLocation(edgeProgram, "inputNormal");
		edge_inputDepth = glGetUniformLocation(edgeProgram, "inputDepth");
		edge_pixelSize = glGetUniformLocation(edgeProgram, "pixelSize");
	}



	// init image library and load images
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);


	HoG_bot_tex = ilutGLLoadImage("resource/images/HoG_bot.png");
	HoG_bot_toon_tex = ilutGLLoadImage("resource/images/HoG_bot_toon.png");
	HoG_tower_tex = ilutGLLoadImage("resource/images/HoG_tower.png");
	smiley_tex = ilutGLLoadImage("resource/images/smiley.png");
	blue_tex = ilutGLLoadImage("resource/images/blue.png");

	qMap = ilutGLLoadImage("resource/images/qMap.png");
	glBindTexture(GL_TEXTURE_2D, qMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);



	// load obj files
	HoG_bot.Load("resource/models/HoG_bot.obj");
	HoG_tower.Load("resource/models/HoG_tower.obj");
	sphereOBJ.Load("resource/models/sphereOBJ.obj");

}

void Update(double dt)
{
	// update all objects
	BOT_UPDATE(dt);
	SPHERE_UPDATE(dt);
	CAMERA_UPDATE(dt);
}

void FullScreenQuad()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f( 1.0f,  1.0f, -1.0f);
	glEnd();
}

void Render()
{
	const unsigned int buffs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	// SCENE PASS
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glDrawBuffers(2, buffs);


	// new frame, clear buffers
	glViewport(0, 0, windowWidth, windowHeight);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	

	// load projection and view matrices for current viewer
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	CAMERA_MULT_PROJECTION_MATRIX();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	CAMERA_MULT_VIEW_MATRIX();


	glUseProgram(toonProgram);
	
	glUniform1i(toon_qmap, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, qMap);

	glUniform1i(toon_inputImage, 0);
	glActiveTexture(GL_TEXTURE0);


	// for each object, multiply model matrix and draw
	glPushMatrix();
		SPHERE_MULT_MODEL_MATRIX();
		SPHERE_RENDER();
	glPopMatrix();
	glPushMatrix();
		LIGHT_MULT_MODEL_MATRIX();
		LIGHT_RENDER();
	glPopMatrix();
	glPushMatrix();
		BOT_MULT_MODEL_MATRIX();
		BOT_RENDER();
	glPopMatrix();
	TOWER_RENDER();
	GROUND_RENDER();



	// POST-PROCESSING PASS (and output to screen)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	
	// send uniforms
	// set respective textures according to the uniforms
	glUseProgram(edgeProgram);

	glUniform1i(edge_inputColour, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneColour[0]);
	
	glUniform1i(edge_inputNormal, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sceneColour[1]);
	
	glUniform1i(edge_inputDepth, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, sceneDepth);

	glUniform2f(edge_pixelSize, 
		1.0f/(float)windowWidth, 1.0f/(float)windowHeight);
	

	// APPLY THE EFFECT AND OUTPUT
	FullScreenQuad();



	// reset all the things
	glUseProgram(0);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);


	// finished
	glutSwapBuffers();
}
