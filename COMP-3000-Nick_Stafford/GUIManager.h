#pragma once
#include "GL/glew.h"
#include <string>
#include <map>
#include "LoadShaders.h"

namespace Mer
{
	class GUIManager
	{
	public:
		GUIManager();

		void InitialiseGUI();

		void NewFrame();
		void EndFrame();

		void Panel();
		bool Button(int x, int y, int width, int height, GLuint texture, std::string name);

	private:
		enum Buffer_Counts { NumMenus = 100, NumTex = 1 };
		GLuint menuBuffers[NumMenus];
		GLuint textureBuffer[NumTex];

		GLfloat texVertices[4][2] =
		{
			{0.0f,1.0f},//top left
			{1.0f,1.0f},//top right
			{1.0f,0.0f},//bottom right
			{0.0f,0.0f}//bottom left
		};

		GLuint menuShader;

		int buttonCount = 0;

		std::map<std::string, int> _buttons;
	};
}

