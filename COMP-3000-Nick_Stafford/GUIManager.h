#pragma once
#include <string>
#include "LoadShaders.h"
#include "GUIElements.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <ft2build.h>


#include FT_FREETYPE_H

#include <map>
#include <iostream>

namespace Mer
{
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	class GUIManager
	{
	public:
		GUIManager();

		void InitialiseGUI();

		void NewFrame();
		void EndFrame();

		void Panel(int x, int y, int width, int height, GLuint texture, std::string name);
		bool Button(int x, int y, int width, int height, GLuint texture, std::string name);
		void Text(std::string text, float x, float y, float scale, glm::vec3 color);

		bool ProcessButtonPress(double mouseX, double mouseY);
		bool ProcessButtonRelease(double mouseX, double mouseY);

		void CleanUp();

	private:
		enum Buffer_Counts { NumMenus = 100, NumTex = 1 };
		GLuint menuBuffers[NumMenus];
		GLuint textureBuffer[NumTex];

		unsigned int VAO, VBO;


		GLfloat texVertices[4][2] =
		{
			{0.0f,0.0f},//bottom left
			{1.0f,0.0f},//bottom right
			{1.0f,1.0f},//top right
			{0.0f,1.0f}//top left
		};

		GLuint menuShader;
		GLuint textShader;


		FT_Library library;
		FT_Face face;

		glm::mat4 projection;



		std::map<char, Character> Characters;

		int uiCount = 0;

		
		std::vector<ButtonElement> _buttons;
		std::vector<PanelElement> _panels;

		bool Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2);
	};
}

