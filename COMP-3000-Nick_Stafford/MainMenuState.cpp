#include "MainMenuState.h"
namespace Mer
{
	MainMenuState::MainMenuState(GameDataRef data) : _data(data)
	{

	}
	void MainMenuState::Init()
	{
		GUI.InitialiseGUI();
		_data->assets.LoadTexture("background", "Assets\\Main_Menu\\main_menu_background.png");
		_data->assets.LoadTexture("singleplayerB", "Assets\\Main_Menu\\singleplayer_button.tga");
		_data->assets.LoadTexture("settingsB", "Assets\\Main_Menu\\settings_button.tga");
		_data->assets.LoadTexture("exitB", "Assets\\Main_Menu\\exit_button.tga");

		glGenBuffers(NumBuffers, backgroundBuffers);

		glBindBuffer(GL_ARRAY_BUFFER, backgroundBuffers[vertex]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), backgroundCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, backgroundBuffers[texture]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), texVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glfwGetWindowSize(_data->window, &windowW, &windowH);
	}
	void MainMenuState::HandleInput()
	{
		int mouseState = glfwGetMouseButton(_data->window, GLFW_MOUSE_BUTTON_LEFT);

		if (mouseState == GLFW_PRESS)
		{
			mousePressed = true;
			glfwGetCursorPos(_data->window, &xpos, &ypos);

			if (xpos > 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{
				xpos -= (windowW / 2);
				xpos = xpos / (windowW / 2);
				ypos -= (windowH / 2);
				ypos = ypos / (windowH / 2);
				ypos *= -1;

				if (GUI.ProcessButtonPress(xpos, ypos))
				{

				}
				else
				{
					
				}
			}
		}
		else if (mousePressed == true && mouseState == GLFW_RELEASE)
		{
			mousePressed = false;
			glfwGetCursorPos(_data->window, &xpos, &ypos);
			if (xpos > 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{
				xpos -= (windowW / 2);
				xpos = xpos / (windowW / 2);
				ypos -= (windowH / 2);
				ypos = ypos / (windowH / 2);
				ypos *= -1;
				if (GUI.ProcessButtonRelease(xpos, ypos))
				{
				}
				else
				{
				}
			}
		}
	}
	void MainMenuState::Update(float dt)
	{
		glfwPollEvents();

	}
	void MainMenuState::Draw(float dt)
	{
		static const float black[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);



		GUI.NewFrame();

		glBindBuffer(GL_ARRAY_BUFFER, backgroundBuffers[vertex]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, backgroundBuffers[texture]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindTexture(GL_TEXTURE_2D, _data->assets.getTexture("background"));
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		if (GUI.Button(320, 480, 320, 60, _data->assets.getTexture("singleplayerB"), "singleplayer"))
		{
			_data->machine.AddState(StateRef(new TestState(_data)));
		}
		if (GUI.Button(800, 480, 320, 60, _data->assets.getTexture("settingsB"), "settings"))
		{
			std::cout << "Settings" << std::endl;
		}
		if (GUI.Button(1280, 480, 320, 60, _data->assets.getTexture("exitB"), "exit"))
		{
			glfwSetWindowShouldClose(_data->window, GLFW_TRUE);
		}
		GUI.EndFrame();

		glBindTexture(GL_TEXTURE_2D, 0);


		glfwSwapBuffers(_data->window);
	}
	void MainMenuState::CleanUp()
	{
		GUI.CleanUp();
		_data->assets.CleanUp();
		glDeleteBuffers(NumBuffers, backgroundBuffers);
	}
}