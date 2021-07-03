#include "GUIManager.h"
namespace Mer
{
	GUIManager::GUIManager()
	{

	}
	void GUIManager::InitialiseGUI(float screenWidth, float screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		glGenBuffers(NumMenus, menuBuffers);
		glGenBuffers(NumTex, textureBuffer);
		ShaderInfo menuShaders[] =
		{
			{ GL_VERTEX_SHADER, "Shaders/menu.vert" },
			{ GL_FRAGMENT_SHADER, "Shaders/menu.frag" },
			{ GL_NONE, NULL }
		};
		menuShader = LoadShaders(menuShaders);

		ShaderInfo textShaders[] =
		{
			{ GL_VERTEX_SHADER, "Shaders/text.vert" },
			{ GL_FRAGMENT_SHADER, "Shaders/text.frag" },
			{ GL_NONE, NULL }
		};
		textShader = LoadShaders(textShaders);

		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), texVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		FT_Error error = FT_Init_FreeType(&library);
		if (error)
		{
			std::cout << "free type init failed" << std::endl;
		}
		error = FT_New_Face(library, "Assets\\Fonts\\Roboto-Black.ttf", 0, &face);
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cout << "free type unknown file type" << std::endl;
		}
		else if (error)
		{
			std::cout << "error with loading face" << std::endl;
		}
		FT_Set_Pixel_Sizes(face, 0, 48);
		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};

			

			//Characters[c] = character;
			Characters.insert(std::pair<char, Character>(c, character));
		}


		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}

	bool GUIManager::ProcessButtonPress(double mouseX, double mouseY)
	{
		mouseX -= (screenWidth / 2);
		mouseX = mouseX / (screenWidth / 2);
		mouseY -= (screenHeight / 2);
		mouseY = mouseY / (screenHeight / 2);
		mouseY *= -1;

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
		mouseX -= (screenWidth / 2);
		mouseX = mouseX / (screenWidth / 2);
		mouseY -= (screenHeight / 2);
		mouseY = mouseY / (screenHeight / 2);
		mouseY *= -1;

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
				if (_buttons[i].active)
				{
					_buttons[i].pressed = true;
					return true;
				}
				else
				{
					return false;
				}
				
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

	void GUIManager::Text(std::string text, float x, float y, float scale, glm::vec3 color)
	{
		glUseProgram(textShader);
		glUniform3f(glGetUniformLocation(textShader, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
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


			glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[uiCount]);
			glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			ButtonElement temp;
			std::vector<glm::vec2> coords;
			coords.push_back({ x1, y1 }); coords.push_back({ x2, y1 }); coords.push_back({ x2, y2 }); coords.push_back({ x1, y2 });
			temp = { uiCount, name, coords };

			_buttons.push_back(temp);
			id = _buttons.size() - 1;
			uiCount++;

		}

		_buttons[id].active = true;

		glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[_buttons[id].bufferID]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		if (_buttons[id].pressed)
		{
			_buttons[id].pressed = false;
			return true;
		}

		return false;
	}


	void GUIManager::Panel(int x, int y, int width, int height, GLuint texture, std::string name)
	{
		bool found = false;
		int id;
		for (int i = 0; i < _panels.size(); i++)
		{
			if (_panels[i].name == name)
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


			glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[uiCount]);
			glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			PanelElement temp;
			std::vector<glm::vec2> coords;
			coords.push_back({ x1, y1 }); coords.push_back({ x2, y1 }); coords.push_back({ x2, y2 }); coords.push_back({ x1, y2 });
			temp = { uiCount, name, coords };

			_panels.push_back(temp);
			id = _panels.size() - 1;
			uiCount++;
			
		}

		_panels[id].active = true;

		glBindBuffer(GL_ARRAY_BUFFER, menuBuffers[_panels[id].bufferID]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void GUIManager::NewFrame()
	{
		glUseProgram(menuShader);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		for (int i = 0; i < _buttons.size(); i++)
		{
			_buttons[i].active = false;
		}
		for (int i = 0; i < _panels.size(); i++)
		{
			_panels[i].active = false;
		}
	}
	void GUIManager::EndFrame()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	void GUIManager::CleanUp()
	{
		glDeleteBuffers(uiCount, menuBuffers);
		glDeleteBuffers(1, textureBuffer);
	}
}