#pragma once
#include <iostream>
#include <cstring>
#include <SFML/OpenGL.hpp>
#include "include\SFML\Window.hpp"
//#include <SFML/Window.hpp>
#include "Vector3f.h"
#include "include\SFML\Graphics\Image.hpp"
//#include <SFML/Graphics/Image.hpp>

class Texture
{
public :
	Texture ();
	Texture (std::string filename)
	{
		if (!img_data.loadFromFile(filename))
		{
			std::cout<<"can't open "<<filename;
			system("Pause");
			exit(1);
		}
		glEnable(GL_TEXTURE_2D);
		img_data.loadFromFile(filename);
		//UoitWater.LoadFromFile(file2);

		//Generate OpenGL texture object
	
		glGenTextures(1, &texture_handle);
		glBindTexture(GL_TEXTURE_2D, texture_handle);

		// Upload data to GPU
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			img_data.getSize().x, img_data.getSize().y, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());
		// some texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// better results, but slower
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//linear interp
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	~Texture ();

	sf::Image img_data;
	GLuint texture_handle;
};