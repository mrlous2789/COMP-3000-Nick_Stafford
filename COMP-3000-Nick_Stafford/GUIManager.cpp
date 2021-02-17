#include "GUIManager.h"
namespace Mer
{
	GUIManager::GUIManager()
	{

	}
	void GUIManager::InitialiseGUI()
	{
		glGenBuffers(NumMenus, menuBuffers);
		glGenBuffers(NumTex, textureBuffer);
		ShaderInfo  menuShaders[] =
		{
			{ GL_VERTEX_SHADER, "Shaders/menu.vert" },
			{ GL_FRAGMENT_SHADER, "Shaders/menu.frag" },
			{ GL_NONE, NULL }
		};
		menuShader = LoadShaders(menuShaders);

		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), texVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	bool GUIManager::Button(int x, int y, int width, int height, GLuint texture, std::string name)
	{
		if (_buttons.find(name) == _buttons.end())
		{
			//x -= (1920.0f / 2);
			//y += (1080.0f / 2);
			GLfloat vertices[4][3]
			{
				{(x / 960.0f) - 1.0f,(y / 540.0f) - 1.0f ,0.0f},
				{((x / 960.0f) - 1.0f) + (width / 1920.0f),(y / 540.0f) - 1.0f,0.0f},
				{((x / 960.0f) - 1.0f) + (width / 1920.0f),((y / 540.0f) - 1.0f) - (height / 1080.0f),0.0f},
				{(x / 960.0f - 1.0f),((y / 540.0f) - 1.0f) - (height / 1080.0f),0.0f},
			};


			glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[buttonCount]);
			glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			_buttons[name] = buttonCount;
			buttonCount++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[_buttons[name]]);
		glBindTexture(GL_TEXTURE_2D, texture);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		return false;
	}


	void GUIManager::NewFrame()
	{
		glUseProgram(menuShader);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
	void GUIManager::EndFrame()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}