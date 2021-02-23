#include "TestState.h"
namespace Mer
{

	bool TestState::KeysPressed[348];

	TestState::TestState(GameDataRef data) : _data(data)
	{
	}

	void TestState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			KeysPressed[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			KeysPressed[key] = false;
		}
	}
	void TestState::Init()
	{
		glGenVertexArrays(1, VAOs);


		_data->assets.LoadTexture("singleplayerB", "Assets\\Main_Menu\\singleplayer_button.tga");
		_data->assets.LoadTexture("exitB", "Assets\\Main_Menu\\exit_button.tga");


		GUI.InitialiseGUI();
		GMC.Initialise();

		glfwGetWindowSize(_data->window, &windowW, &windowH);
		glfwSetScrollCallback(_data->window, GMC.scroll_callback);
		glfwSetKeyCallback(_data->window, key_callback);
	}
	void TestState::HandleInput()
	{

		GMC.ProcessKeyPresses(KeysPressed);
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
					GMC.ProcessMousePress(xpos, ypos);
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
	void TestState::Update(float dt)
	{

		GMC.UpdateMap();

		glfwPollEvents();
	}
	void TestState::Draw(float dt)
	{
		static const float black[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT);

		GMC.Draw();

		GUI.NewFrame();
		if (GUI.Button(700, 300, 500, 100, _data->assets.getTexture("singleplayerB"), "singleplayer"))
		{
			std::cout << "Singleplayer" << std::endl;
		}
		if (GUI.Button(700, 400, 500, 100, _data->assets.getTexture("exitB"), "exit"))
		{
			std::cout << "Exit" << std::endl;
		}
		GUI.EndFrame();

		glBindTexture(GL_TEXTURE_2D, 0);


		glfwSwapBuffers(_data->window);


	}
	void TestState::CleanUp()
	{
		//glDeleteBuffers(NumCells, cellBuffers);
		//glDeleteBuffers(NumRivers, riverBuffers);
		glDeleteVertexArrays(NumVAOs, VAOs);
	}
}