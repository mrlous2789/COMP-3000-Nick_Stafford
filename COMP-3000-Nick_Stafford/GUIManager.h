#pragma once
#include <string>
#include "LoadShaders.h"
#include "GUIElements.h"

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

		bool ProcessButtonPress(double mouseX, double mouseY);
		bool ProcessButtonRelease(double mouseX, double mouseY);

		void CleanUp();

	private:
		enum Buffer_Counts { NumMenus = 100, NumTex = 1 };
		GLuint menuBuffers[NumMenus];
		GLuint textureBuffer[NumTex];

		GLfloat texVertices[4][2] =
		{
			{0.0f,0.0f},//bottom left
			{1.0f,0.0f},//bottom right
			{1.0f,1.0f},//top right
			{0.0f,1.0f}//top left
		};

		GLuint menuShader;

		int buttonCount = 0;

		
		std::vector<ButtonElement> _buttons;

		bool Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2);
	};
}

