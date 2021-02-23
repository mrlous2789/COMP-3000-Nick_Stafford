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

	bool GUIManager::ProcessButtonPress(double mouseX, double mouseY)
	{
		for (int i = 0; i < _buttons.size(); i++)
		{
			int counter = 0;
			for (int j = 0, k = _buttons[i].vertices.size() - 1; j < _buttons[i].vertices.size(); k = j++)
			{
				if (Intersects(mouseX, mouseY, _buttons[i].vertices[j].x, _buttons[i].vertices[j].y, _buttons[i].vertices[k].x, _buttons[i].vertices[k].y))
				{
					counter++;
				}
			}
			if (counter % 2 != 0)
			{
				return true;
			}
		}

		return false;
	}
	bool GUIManager::ProcessButtonRelease(double mouseX, double mouseY)
	{
		for (int i = 0; i < _buttons.size(); i++)
		{
			int counter = 0;
			for (int j = 0, k = _buttons[i].vertices.size() - 1; j < _buttons[i].vertices.size(); k = j++)
			{
				if (Intersects(mouseX, mouseY, _buttons[i].vertices[j].x, _buttons[i].vertices[j].y, _buttons[i].vertices[k].x, _buttons[i].vertices[k].y))
				{
					counter++;
				}
			}
			if (counter % 2 != 0)
			{
				_buttons[i].pressed = true;
				return true;
			}
		}

		return false;
	}

	bool GUIManager::Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2)
	{
		if ((mouseY <= edgeY1) != (mouseY <= edgeY2))
		{
			if (mouseX <= (edgeX2 - edgeX1) * (mouseY - edgeY1) / (edgeY2 - edgeY1) + edgeX1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	bool GUIManager::Button(int x, int y, int width, int height, GLuint texture, std::string name)
	{
		bool found = false;
		int id;
		for (int i = 0; i < _buttons.size(); i++)
		{
			if (_buttons[i].name == name)
			{
				found = true;
				id = i;
			}
		}

		if (!found)
		{
			float x1, x2, y1, y2;
			x1 = (x - 960.0f) / 960.0f;
			x2 = ((x + width) - 960.0f) / 960.0f;
			y1 = (y - 540.0f) / 540.0f;
			y2 = ((y + height) - 540.0f) / 540.0f;

			GLfloat vertices[4][3]
			{
				{x1, y1, 0.0f},
				{x2, y1, 0.0f},
				{x2, y2, 0.0f},
				{x1, y2, 0.0f},
			};


			glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[buttonCount]);
			glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			ButtonElement temp;
			std::vector<glm::vec2> coords;
			coords.push_back({ x1, y1 }); coords.push_back({ x2, y1 }); coords.push_back({ x2, y2 }); coords.push_back({ x1, y2 });
			temp = { buttonCount, name, coords };

			_buttons.push_back(temp);
			id = buttonCount;
			buttonCount++;

		}

		glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[_buttons[id].bufferID]);
		glBindTexture(GL_TEXTURE_2D, texture);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		if (_buttons[id].pressed)
		{
			_buttons[id].pressed = false;
			return true;
		}

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