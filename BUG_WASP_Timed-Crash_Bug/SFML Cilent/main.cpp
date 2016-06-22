#include <SFML/Window.hpp>
#include "Objects.h"

#include <SFML/OpenGL.hpp>
#include <SFML\Graphics\Image.hpp>

#include "Camera.h"
#include "randomNumberGenerator.h"

#include <string.h>

/////////

void init();
void myDrawFunction();
void floor();
void KeyInput();
void Update();


GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zRot = 0.0f;
GLfloat xTrans = 0.0f;
GLfloat zTrans = 0.0f;
GLfloat tempx = 0.0f, tempy = 0.0f;

sf::Image tempImgData;
// Handles for 3 textures
GLuint texture_handle[3];

sf::Image mipmap_data;

GLfloat LightAmbient[]=		{ 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ 100.0f, 100.0f, 100.0f, 1.0f };

////////////
GameCamera *OurCam;
std::vector<object*> *PtrBoids,boids;
std::vector<object*> *PtrBoids2,boids2;
object *O1,*Boid1,*Boid2,*Boid3,*Boid4,*Boid5,*Cage;
object *Boid6,*Boid7,*Boid8,*Boid9,*Boid10, *Boid11;
Texture *Texture1,*Texture2;
bool seek, b_ClickCoolDown;
Vector3f *testing;
std::vector<object*> *PtrAnimating,v_Animating;
RNG ranNumGen;

// timers 
float GameTime;
sf::Clock gameTimer,ElapsedTime,c_ClickCD,loadCD;


int main()
{
	// Create the main window
	sf::Window App(sf::VideoMode(1200, 800, 32), "Debugging assignment 1",2);//, sf::Style::Fullscreen, 2); // 2 levels of antialiasing
	init(); //Init OpenGL states 

	sf::Event Event;

	App.setFramerateLimit(65);

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	///// MAIN LOOP /////
	while (App.isOpen())
	{

		// Clear color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		KeyInput();

		while (App.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
				App.close();

			// Escape key : exit
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				App.close();

			// Resize event : adjust viewport
			if (Event.type == sf::Event::Resized) {
				glViewport(0, 0, Event.size.width, Event.size.height);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(45.f, Event.size.width/Event.size.height, 1.f, 500.f);
				glMatrixMode(GL_MODELVIEW);
			}
		}

		App.setActive();

		// Setup a perspective projection
		glViewport(0, 0, 1200, 800);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.f, 1.f, 1.f, 10000.f);
		OurCam->use();

		myDrawFunction();
		floor();

		/////////////
		//update
		Update();
		GameTime=gameTimer.getElapsedTime().asSeconds();
		gameTimer.restart();

		// Finally, display rendered frame on screen
		App.display();

	}


	return EXIT_SUCCESS;
}

void init()
{
	///// INIT OpenGL /////
	// Set color and depth clear value
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations


	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Enable texture
	glEnable(GL_TEXTURE_2D);

	ranNumGen = RNG();

	//int *crazyp, crazy= INT_MAX ;
	//crazyp = &crazy ;//.MaxValue;  //2,147,483,647
	//*crazyp=crazy;
	//*crazyp+=780000000000;                   //-2,147,483,648
	//crazyp++;
	////std::cout<<*crazyp;
	//int lol= *crazyp;

	//int * q = myarray+20;

	//////////////////////
	OurCam = new GameCamera();
	OurCam->topDownview();
	O1= new object (0,0,0);
	O1->loadobject("Bird.obj");

	Boid1= new object (O1->x+ranNumGen.ReturnNumber(5,15),0,O1->x+ranNumGen.ReturnNumber(5,15));
	Boid1->loadobject("Bird.obj");
	Boid1->VmorphBool=true;
	Boid1->boidNum=0;

	Boid2= new object (O1->x-ranNumGen.ReturnNumber(5,17),0,O1->x-ranNumGen.ReturnNumber(3,17));
	Boid2->loadobject("Bird.obj");
	Boid2->VmorphBool=true;
	Boid2->boidNum=1;

	Boid3= new object (O1->x-ranNumGen.ReturnNumber(3,17),0,O1->x+ranNumGen.ReturnNumber(3,17));
	Boid3->loadobject("Bird.obj");
	Boid3->VmorphBool=true;
	Boid3->boidNum=2;

	Boid4= new object (O1->x+16,0,O1->x-6);
	Boid4->loadobject("Bird.obj");
	Boid4->VmorphBool=true;
	Boid4->boidNum=3;

	Boid5= new object (O1->x+5,0,O1->x-15);
	Boid5->loadobject("Bird.obj");
	Boid5->VmorphBool=true;
	Boid5->boidNum=4;

	Boid6= new object (O1->x+10,0,O1->x+10);
	Boid6->loadobject("Bird.obj");
	Boid6->VmorphBool=true;
	Boid6->boidNum=0;

	Boid7= new object (O1->x-14,0,O1->x-14);
	Boid7->loadobject("Bird.obj");
	Boid7->VmorphBool=true;
	Boid7->boidNum=1;

	Boid8= new object (O1->x-14,0,O1->x+10);
	Boid8->loadobject("Bird.obj");
	Boid8->VmorphBool=true;
	Boid8->boidNum=2;

	Boid9= new object (O1->x+14,0,O1->x-10);
	Boid9->loadobject("Bird.obj");
	Boid9->VmorphBool=true;
	Boid9->boidNum=3;

	Boid10= new object (O1->x+5,0,O1->x-15);
	Boid10->loadobject("Bird.obj");
	Boid10->VmorphBool=true;
	Boid10->boidNum=4;

	Boid11= new object (O1->x+5,0,O1->x-15);
	Boid11->loadobject("Bird.obj");
	Boid11->VmorphBool=true;
	Boid11->boidNum=4;

	Cage= new object (0,0,0);
	Cage->loadobject("Cage.obj");
	Cage->lerp=true;
	Texture1= new Texture("images/Bird.png");
	Texture2= new Texture("images/checker.jpg");
	seek=false;
	c_ClickCD.restart();
	loadCD.restart();

	//// Following boids 
	PtrBoids=&boids;
	PtrBoids->push_back(Boid1);
	PtrBoids->push_back(Boid2);
	PtrBoids->push_back(Boid3);
	PtrBoids->push_back(Boid4);
	PtrBoids->push_back(Boid5);
	PtrBoids->push_back(Boid6);
	PtrBoids->push_back(Boid7);
	PtrBoids->push_back(Boid8);
	PtrBoids->push_back(Boid9);
	PtrBoids->push_back(Boid10);
	//PtrBoids->push_back(Boid11);

	///// ANIMATIONS 
	PtrAnimating=&v_Animating;
	object *AB1,*AB2,*AB3;
	///1
	AB1= new object (0,0,0);
	AB1->loadobject("Bird.obj");
	PtrAnimating->push_back(AB1);
	///2
	AB2= new object (0,0,0);
	AB2->loadobject("Bird2.obj");
	PtrAnimating->push_back(AB2);
	///3
	AB3= new object (0,0,0);
	AB3->loadobject("Bird3.obj");
	PtrAnimating->push_back(AB3);
	//4 to morph back to 1st 
	PtrAnimating->push_back(AB2);
	//5 extra
	PtrAnimating->push_back(AB1);
}

void myDrawFunction() {

	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//animate 
	for (int i=0; i+1<=PtrBoids->size();i++)
		PtrBoids->at(i)->Vmorphing(PtrAnimating,GameTime*10);

	/////phil
	O1->draw(*Texture1);
	for (int i=0; i+1<=PtrBoids->size();i++)
		PtrBoids->at(i)->draw(*Texture1);

	O1->draw(*Texture1);

	Cage->draw(*Texture1);
}

void floor() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 4 ///
	glBindTexture(GL_TEXTURE_2D,Texture2->texture_handle);//texture_handle[2]);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glScalef(12.f, 2.f, 1.f);
	glBegin(GL_QUADS);
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-500.0, -500.0, 5000.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(500.0, -500.0, 5000.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(500.0, -500.0, -5000.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-500.0, -500.0, -5000.0);
	glEnd();

	glPopMatrix();
}

void KeyInput()
{
	///// Real-time input handling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		yRot += .1f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		yRot -= .1f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		xRot += .1f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		xRot -= .1f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		O1->transx(-0.1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		O1->transx(0.1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		O1->transz(-0.1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		O1->transz(0.1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		OurCam->move(0,0.5,0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		OurCam->move(0,-0.5,0);
}

void Update()
{
	if (b_ClickCoolDown)
	{
		b_ClickCoolDown=false;
	}

	int numOfBoids = PtrBoids->size();

	for (int i=0; i+1<=numOfBoids;i++)
	{
		object** temp = new object*[numOfBoids];
		for (int j=0; j<numOfBoids; j++)
		{
			temp[j] = PtrBoids->at(j)->getObj();
		}
		temp[i]->update(O1->v_Pos, GameTime,4,PtrBoids,Cage->v_Pos);
	}

	O1->update(GameTime);

	Vector3f *Loc1=new Vector3f(-2,0,0);
	Vector3f *Loc2=new Vector3f(2,0,0);

	Cage->lerpUpdate(Loc1,Loc2,GameTime/2);
}