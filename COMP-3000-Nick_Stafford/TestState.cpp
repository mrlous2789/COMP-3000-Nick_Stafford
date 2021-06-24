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

		glfwGetWindowSize(_data->window, &windowW, &windowH);

		GUI.InitialiseGUI(windowW, windowH);
		GMC.Initialise(windowW, windowH);
		//PLC.Initialise(GMC.getNationPointerById(11));

		_data->assets.LoadTexture("terrainB", "Assets\\Game_Map_Buttons\\terrain_map_button.tga");
		_data->assets.LoadTexture("nationB", "Assets\\Game_Map_Buttons\\nation_map_button.tga");
		_data->assets.LoadTexture("religionB", "Assets\\Game_Map_Buttons\\religion_map_button.tga");
		_data->assets.LoadTexture("cultureB", "Assets\\Game_Map_Buttons\\culture_map_button.tga");
		_data->assets.LoadTexture("mainPanel", "Assets\\Game_UI\\game_ui_main_panel.tga");


		//glfwSetScrollCallback(_data->window, GMC.scroll_callback);
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
		//glClear(GL_COLOR_BUFFER_BIT);

		GMC.Draw();
		//PLC.Draw(_data->assets.getTexture("player"));


		//GUI.NewFrame();
		//if (GUI.Button(1600, 150, 50, 50,_data->assets.getTexture("terrainB"),"TerrainB"))
		//{
		//	GMC.UpdateDrawMode(GMC.DrawTerrain);
		//}
		//if (GUI.Button(1655, 150, 50, 50, _data->assets.getTexture("nationB"), "NationB"))
		//{
		//	GMC.UpdateDrawMode(GMC.DrawNations);
		//}
		//if (GUI.Button(1710, 150, 50, 50, _data->assets.getTexture("cultureB"), "CultureB"))
		//{
		//	GMC.UpdateDrawMode(GMC.DrawCultures);
		//}
		//if (GUI.Button(1765, 150, 50, 50, _data->assets.getTexture("religionB"), "ReligionB"))
		//{
		//	GMC.UpdateDrawMode(GMC.DrawReligions);
		//}

		GUI.Panel(0, 0, 1920, 1080, _data->assets.getTexture("mainPanel"), "MainPanel");

		GUI.EndFrame();
		GUI.Text(GMC.getNationById(PLC.getNationID()).name, 5.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text("Gold: ", 65.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text(PLC.getGold(), 100.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text(PLC.getGoldPerTurn(), 122.0f, 585.0f, 0.15f, glm::vec3(0.0f, 0.0f, 0.0f));
		
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