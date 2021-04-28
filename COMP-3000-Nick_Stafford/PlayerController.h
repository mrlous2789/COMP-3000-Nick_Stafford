#pragma once
#include "LoadShaders.h"
#include <iostream>

namespace Mer
{
	class PlayerController
	{
	public:
		PlayerController();
		void Initialise();
		void Draw(GLuint texture);

		int getNationID();

	private:

		int nationID = 11;

		enum Buffer_Counts { Vertex, Texture, NumBuffers = 2 };
		GLuint playerBuffers[NumBuffers];

		

		GLfloat texVertices[4][2] =
		{
			{0.0f,0.0f},//bottom left
			{1.0f,0.0f},//bottom right
			{1.0f,1.0f},//top right
			{0.0f,1.0f}//top left
		};
		GLfloat Vertices[4][3]
		{
			{-1.0f,-1.0f,0.0f},//bottom left
			{1.0f,-1.0f,0.0f},//bottom right
			{1.0f,1.0f,0.0f},//top right
			{-1.0f,1.0f,0.0f}//top left
		};

		GLuint playerShader;
	};
}


