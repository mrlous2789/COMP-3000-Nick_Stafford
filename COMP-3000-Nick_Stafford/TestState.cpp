#include "TestState.h"
namespace Mer
{
	bool TestState::isZoomIn = false;
	bool TestState::isZoomOut = false;

	TestState::TestState(GameDataRef data) : _data(data)
	{
	}
	void TestState::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (yoffset < 0)
		{
			isZoomOut = true;
		}
		else if (yoffset > 0)
		{
			isZoomIn = true;
		}
	}
	void TestState::Init()
	{
		glGenVertexArrays(NumVAOs, VAOs);
		glGenBuffers(NumCells, cellBuffers);
		glGenBuffers(NumRivers, riverBuffers);

		GMC.LoadFromFile(location, mapname);

		selectedCell = &GMC.cells[0];

		for (int i = 0; i < GMC.cells.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, cellBuffers[i]);
			glBufferData(GL_ARRAY_BUFFER, GMC.cells[i].coords.size() * sizeof(glm::vec3), &GMC.cells[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		for (int i = 0; i < GMC.rivers.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, riverBuffers[i]);
			glBufferData(GL_ARRAY_BUFFER, GMC.rivers[i].coords.size() * sizeof(glm::vec3), &GMC.rivers[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		ShaderInfo  shaders[] =
		{
			{ GL_VERTEX_SHADER, "Shaders/cells.vert" },
			{ GL_FRAGMENT_SHADER, "Shaders/cells.frag" },
			{ GL_NONE, NULL }
		};

		shader = LoadShaders(shaders);

		glUseProgram(shader);

		// creating the model matrix
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(zoomLevel, zoomLevel, 1.0f));
		model = glm::translate(model, glm::vec3(xoffset, yoffset, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

		
		// creating the view matrix
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

		// creating the projection matrix
		projection = glm::perspective(89.535f, 1.0f, 0.1f, 20.0f);

		// Adding all matrices up to create combined matrix
		mvp = projection * view * model;

		//adding the Uniform to the shader
		int mvpLoc = glGetUniformLocation(shader, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		glfwGetWindowSize(_data->window, &windowW, &windowH);
		glfwSetScrollCallback(_data->window, scroll_callback);
	}
	void TestState::HandleInput()
	{
		if (glfwGetKey(_data->window, GLFW_KEY_W) == 1)
			isMoveUp = true;
		if (glfwGetKey(_data->window, GLFW_KEY_S) == 1)
			isMoveDown = true;
		if (glfwGetKey(_data->window, GLFW_KEY_A) == 1)
			isMoveLeft = true;
		if (glfwGetKey(_data->window, GLFW_KEY_D) == 1)
			isMoveRight = true;

		if (glfwGetMouseButton(_data->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{

			glfwGetCursorPos(_data->window, &xpos, &ypos);
			if (xpos > 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{
				xpos -= (windowW / 2);
				xpos = xpos / (windowW / 2);
				ypos -= (windowH / 2);
				ypos = ypos / (windowH / 2);
				ypos *= -1;

				xpos /= zoomLevel;
				ypos /= zoomLevel;

				xpos -= xoffset;
				ypos -= yoffset;

				Cell* temp = GMC.getCellAtCoords(xpos, ypos);

				if (selectedCell != temp && temp != nullptr)
				{
					selectedCell = temp;
				}
			}
		}
	}
	void TestState::Update(float dt)
	{
		if (isZoomIn)
			ZoomIn();
		if (isZoomOut)
			ZoomOut();
		if (isMoveUp)
			MoveUp();
		if (isMoveDown)
			MoveDown();
		if (isMoveLeft)
			MoveLeft();
		if (isMoveRight)
			MoveRight();

		if (moved)//mvp is calculated after all offsets and zoomlevels so it only has to happen once rather than on every zoom or move
		{
			// creating the model matrix
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(zoomLevel, zoomLevel, 1.0f));
			model = glm::translate(model, glm::vec3(xoffset, yoffset, 0.0f));
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

			// Adding all matrices up to create combined matrix
			mvp = projection * view * model;

			moved = false;
		}

		glfwPollEvents();
	}
	void TestState::Draw(float dt)
	{
		static const float black[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		glUseProgram(shader);

		for (int  i = 0; i < GMC.cells.size(); i++)
		{
			if (GMC.cells[i].id == selectedCell->id)
			{
				color[0] = 1.0f;
				color[1] = 1.0f;
				color[2] = 0.0f;
			}
			else if (GMC.cells[i].height > 8000)
			{
				color[0] = 1.0f;
				color[1] = 0.0f;
				color[2] = 0.0f;
			}
			else if (GMC.cells[i].height > 6000)
			{
				color[0] = 1.0f;
				color[1] = 0.18f;
				color[2] = 0.18f;
			}
			else if (GMC.cells[i].height > 3000)
			{
				color[0] = 1.0f;
				color[1] = 0.4f;
				color[2] = 0.4f;
			}
			else if (GMC.cells[i].height > 1000)
			{
				color[0] = 1.0f;
				color[1] = 0.7f;
				color[2] = 0.7f;
			}
			else if (GMC.cells[i].height > 0)
			{
				color[0] = 0.0f;
				color[1] = 1.0f;
				color[2] = 0.0f;
			}
			else
			{
				color[0] = 0.0f;
				color[1] = 0.0f;
				color[2] = 1.0f;
			}

			GLint myLoc = glGetUniformLocation(shader, "color");
			glProgramUniform3fv(shader, myLoc, 1, color);

			int mvpLoc = glGetUniformLocation(shader, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			//draw cells
			glBindBuffer(GL_ARRAY_BUFFER, cellBuffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, GMC.cells[i].coords.size());
		}

		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 1.0f;

		for (int i = 0; i < GMC.rivers.size(); i++)
		{
			glLineWidth(GMC.rivers[i].width * zoomLevel * 2.0f);
			GLint myLoc = glGetUniformLocation(shader, "color");
			glProgramUniform3fv(shader, myLoc, 1, color);

			int mvpLoc = glGetUniformLocation(shader, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			glBindBuffer(GL_ARRAY_BUFFER, riverBuffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_LINE_STRIP, 0, GMC.rivers[i].coords.size());
		}

		//std::cout << glGetError() << std::endl;
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(_data->window);
		

	}
	void TestState::CleanUp()
	{
		glDeleteBuffers(NumCells, cellBuffers);
		glDeleteBuffers(NumRivers, riverBuffers);
		glDeleteVertexArrays(NumVAOs, VAOs);
	}

	//map movements functions
	void TestState::ZoomIn()
	{
		if (zoomLevel + zoomRate < maxZoom)
		{
			zoomLevel += zoomRate;

			moved = true;

			isZoomIn = false;
		}
		else
		{
			isZoomIn = false;
		}

	}
	void TestState::ZoomOut()
	{
		if (zoomLevel - zoomRate > minZoom)
		{
			zoomLevel -= zoomRate;

			moved = true;

			isZoomOut = false;


			//if the new zoom level causes off map to be shown change offset
			if ((yoffset - (1.0f / zoomLevel)) < -0.99f)
			{
				yoffset = -0.99f + (1.0f / zoomLevel);
			}
			else if ((yoffset + (1.0f / zoomLevel)) > 0.99f)
			{
				yoffset = 0.99f - (1.0f / zoomLevel);
			}
			if ((xoffset + (1.0f / zoomLevel)) > 0.99f)
			{
				xoffset = 0.99f - (1.0f / zoomLevel);
			}
			else if ((xoffset - (1.0f / zoomLevel)) < -0.99f)
			{
				xoffset = -0.99f + (1.0f / zoomLevel);
			}
		}
		else
		{
			isZoomOut = false;
		}
	}

	void TestState::MoveUp()
	{
		if ((yoffset - (1.0f / zoomLevel)) > -0.99f)
		{
			yoffset -= moveSpeed;

			moved = true;

			isMoveUp = false;
		}
		else
		{
			isMoveUp = false;
		}
	}
	void TestState::MoveDown()
	{
		if ((yoffset + (1.0f / zoomLevel)) < 0.99f)
		{
			yoffset += moveSpeed;

			moved = true;

			isMoveDown = false;
		}
		else
		{
			isMoveDown = false;
		}

	}
	void TestState::MoveLeft()
	{
		if ((xoffset + (1.0f / zoomLevel)) < 0.99f)
		{
			xoffset += moveSpeed;

			moved = true;

			isMoveLeft = false;
		}
		else
		{
			isMoveLeft = false;
		}

	}
	void TestState::MoveRight()
	{
		if ((xoffset - (1.0f / zoomLevel)) > -0.99f)
		{
			xoffset -= moveSpeed;

			moved = true;

			isMoveRight = false;
		}
		else
		{
			isMoveRight = false;
		}
	}
}