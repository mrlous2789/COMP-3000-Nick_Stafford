#include "GameState.h"
namespace Mer
{
	bool GameState::KeysPressed[348];

	GameState::GameState(GameDataRef data) : _data(data)
	{
	}

	void GameState::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
	void GameState::Init()
	{
		



		GUI.InitialiseGUI();
		GMC.Initialise();
		PLC.Initialise(GMC.getNationPointerById(11));

		_data->assets.LoadTexture("player", "Assets\\Player\\temp_player.tga");
		_data->assets.LoadTexture("terrainB", "Assets\\Game_Map_Buttons\\terrain_map_button.tga");
		_data->assets.LoadTexture("nationB", "Assets\\Game_Map_Buttons\\nation_map_button.tga");
		_data->assets.LoadTexture("religionB", "Assets\\Game_Map_Buttons\\religion_map_button.tga");
		_data->assets.LoadTexture("cultureB", "Assets\\Game_Map_Buttons\\culture_map_button.tga");
		_data->assets.LoadTexture("mainPanel", "Assets\\Game_UI\\game_ui_main_panel.tga");
		_data->assets.LoadTexture("speed1B", "Assets\\Game_Map_Buttons\\speed_one_button.tga");
		_data->assets.LoadTexture("speed2B", "Assets\\Game_Map_Buttons\\speed_two_button.tga");
		_data->assets.LoadTexture("speed3B", "Assets\\Game_Map_Buttons\\speed_three_button.tga");
		_data->assets.LoadTexture("speed4B", "Assets\\Game_Map_Buttons\\speed_four_button.tga");
		_data->assets.LoadTexture("speed5B", "Assets\\Game_Map_Buttons\\speed_five_button.tga");

		glfwGetWindowSize(_data->window, &windowW, &windowH);
		glfwSetScrollCallback(_data->window, GMC.scroll_callback);
		glfwSetKeyCallback(_data->window, key_callback);
	}
	void GameState::HandleInput()
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
	void GameState::Update(float dt)
	{
		GMC.UpdateMap();

		PLC.Tick(dt);

		glfwPollEvents();
	}
	void GameState::Draw(float dt)
	{
		static const float black[] = { 0.0f, 0.0f, 1.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		//glClear(GL_COLOR_BUFFER_BIT);

		GMC.Draw();
		


		GUI.NewFrame();
		GUI.Panel(0, 0, 1920, 1080, _data->assets.getTexture("mainPanel"), "MainPanel");
		if (GUI.Button(1600, 150, 50, 50, _data->assets.getTexture("terrainB"), "TerrainB"))
		{
			GMC.UpdateDrawMode(GMC.DrawTerrain);
		}
		if (GUI.Button(1655, 150, 50, 50, _data->assets.getTexture("nationB"), "NationB"))
		{
			GMC.UpdateDrawMode(GMC.DrawNations);
		}
		if (GUI.Button(1710, 150, 50, 50, _data->assets.getTexture("cultureB"), "CultureB"))
		{
			GMC.UpdateDrawMode(GMC.DrawCultures);
		}
		if (GUI.Button(1765, 150, 50, 50, _data->assets.getTexture("religionB"), "ReligionB"))
		{
			GMC.UpdateDrawMode(GMC.DrawReligions);
		}
		if (GUI.Button(1600, 1020, 50, 50, _data->assets.getTexture("speed1B"), "Speed1"))
		{
			PLC.SetTickSpeed(1);
		}
		if (GUI.Button(1655, 1020, 50, 50, _data->assets.getTexture("speed2B"), "Speed2"))
		{
			PLC.SetTickSpeed(2);
		}
		if (GUI.Button(1710, 1020, 50, 50, _data->assets.getTexture("speed3B"), "Speed3"))
		{
			PLC.SetTickSpeed(3);
		}
		if (GUI.Button(1765, 1020, 50, 50, _data->assets.getTexture("speed4B"), "Speed4"))
		{
			PLC.SetTickSpeed(4);
		}
		if (GUI.Button(1825, 1020, 50, 50, _data->assets.getTexture("speed5B"), "Speed5"))
		{
			PLC.SetTickSpeed(5);
		}
		if (GUI.Button(600, 1020, 50, 50, _data->assets.getTexture("religionB"), "RaiseSoldiersB"))
		{
			PLC.RaiseSoldiers();
		}
		//if (GUI.Button(650, 1020, 50, 50, _data->assets.getTexture("player"), "playerbutton"))
		//{
		//	
		//}
		GUI.EndFrame();
		GUI.Text(GMC.getNationById(PLC.getNationID()).name, 5.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text("Gold: ", 65.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text(PLC.getGold(), 100.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text(PLC.getGoldPerTurn(), 100.0f, 575.0f, 0.15f, glm::vec3(0.0f, 0.0f, 0.0f));

		

		PLC.Draw(_data->assets.getTexture("player"));

		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(_data->window);


	}
	void GameState::CleanUp()
	{
		//glDeleteBuffers(NumCells, cellBuffers);
		//glDeleteBuffers(NumRivers, riverBuffers);
		glDeleteVertexArrays(NumVAOs, VAOs);
	}
}