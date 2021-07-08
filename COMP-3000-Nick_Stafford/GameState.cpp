#include "GameState.h"
namespace Mer
{


	GameState::GameState(GameDataRef data) : _data(data)
	{
	}

	void GameState::Init()
	{
		glfwGetWindowSize(_data->window, &windowW, &windowH);



		GUI.InitialiseGUI(windowW, windowH);
		PLC.Initialise(windowW, windowH);

		_data->assets.LoadTexture("player", "Assets\\Player\\temp_player.tga");
		_data->assets.LoadTexture("heightB", "Assets\\Game_Map_Buttons\\height_map_button.tga");
		_data->assets.LoadTexture("terrainB", "Assets\\Game_Map_Buttons\\terrain_map_button.tga");
		_data->assets.LoadTexture("nationB", "Assets\\Game_Map_Buttons\\nation_map_button.tga");
		_data->assets.LoadTexture("religionB", "Assets\\Game_Map_Buttons\\religion_map_button.tga");
		_data->assets.LoadTexture("cultureB", "Assets\\Game_Map_Buttons\\culture_map_button.tga");
		_data->assets.LoadTexture("mainPanel", "Assets\\Game_UI\\game_ui_main_panel.tga");
		_data->assets.LoadTexture("speed0B", "Assets\\Game_Map_Buttons\\speed_zero_button.tga");
		_data->assets.LoadTexture("speed1B", "Assets\\Game_Map_Buttons\\speed_one_button.tga");
		_data->assets.LoadTexture("speed2B", "Assets\\Game_Map_Buttons\\speed_two_button.tga");
		_data->assets.LoadTexture("speed3B", "Assets\\Game_Map_Buttons\\speed_three_button.tga");
		_data->assets.LoadTexture("speed4B", "Assets\\Game_Map_Buttons\\speed_four_button.tga");
		_data->assets.LoadTexture("speed5B", "Assets\\Game_Map_Buttons\\speed_five_button.tga");
		_data->assets.LoadTexture("diplomacyB", "Assets\\Game_UI\\game_ui_diplomacy_button.tga");
		_data->assets.LoadTexture("diplomacyP", "Assets\\Game_UI\\game_ui_diplomacy_panel.tga");
		_data->assets.LoadTexture("warP", "Assets\\Game_UI\\game_ui_war_panel.tga");
		_data->assets.LoadTexture("upArrowB", "Assets\\Game_UI\\game_ui_up_arrow.tga");
		_data->assets.LoadTexture("downArrowB", "Assets\\Game_UI\\game_ui_down_arrow.tga");
		_data->assets.LoadTexture("warB", "Assets\\Game_UI\\game_ui_diplomacy_war.tga");
		_data->assets.LoadTexture("peaceB", "Assets\\Game_UI\\game_ui_diplomacy_peace.tga");
		_data->assets.LoadTexture("cellPanel", "Assets\\Game_UI\\game_ui_cell_panel.tga");
		_data->assets.LoadTexture("acceptPeaceB", "Assets\\Game_UI\\game_ui_peace_accept.tga");
		_data->assets.LoadTexture("cancelPeaceB", "Assets\\Game_UI\\game_ui_peace_cancel.tga");
		_data->assets.LoadTexture("playB", "Assets\\Game_UI\\game_ui_play_button.tga");


		std::cout << "Loaded textures" << std::endl;

		glfwSetScrollCallback(_data->window, PLC.scroll_callback);
		glfwSetKeyCallback(_data->window, PLC.key_callback);
	}
	void GameState::HandleInput()
	{

		PLC.HandleInput();
		int mouseState = glfwGetMouseButton(_data->window, GLFW_MOUSE_BUTTON_LEFT);

		if (mouseState == GLFW_PRESS)
		{
			leftMousePressed = true;
			glfwGetCursorPos(_data->window, &xpos, &ypos);

			if (xpos > 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{

				if (GUI.ProcessButtonPress(xpos, ypos))
				{

				}
				else if (PLC.ProcessMouseClick(xpos,ypos))
				{

				}
				else
				{
					
				}
			}
		}
		else if (leftMousePressed == true && mouseState == GLFW_RELEASE)
		{
			leftMousePressed = false;
			glfwGetCursorPos(_data->window, &xpos, &ypos);
			if (xpos > 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{
				if (GUI.ProcessButtonRelease(xpos, ypos))
				{
				}
				else if (PLC.ProcessLeftMouseRelease(xpos,ypos))
				{

				}
				else
				{
				}
			}
		}
		mouseState = glfwGetMouseButton(_data->window, GLFW_MOUSE_BUTTON_RIGHT);

		if (mouseState == GLFW_PRESS)
		{
			rightMousePressed = true;
		}
		else if (rightMousePressed == true && mouseState == GLFW_RELEASE)
		{
			rightMousePressed = false;
			glfwGetCursorPos(_data->window, &xpos, &ypos);
			if (xpos > 0 && xpos <= windowW && ypos >= 0 && ypos <= windowH)
			{
				if (PLC.ProcessRightMouseRelease(xpos, ypos))
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

		PLC.Update(dt);

		glfwPollEvents();
	}
	void GameState::Draw(float dt)
	{
		static const float black[] = { 0.16f, 0.53f, 0.70f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, black);
		//glClear(GL_COLOR_BUFFER_BIT);
		
		PLC.Draw(_data->assets.getTexture("player"));


		GUI.NewFrame();
		GUI.Panel(0, 0, 1920, 1080, _data->assets.getTexture("mainPanel"), "MainPanel");
		GUI.Panel(10, 10, 384, 144, _data->assets.getTexture("cellPanel"), "CellPanel");
		if (PLC.isMakingPeace())
		{
			if (GUI.Button(1710, 10, 100, 100, _data->assets.getTexture("cancelPeaceB"), "CancelPeaceB"))
			{
				PLC.CancelPeaceDeal();
			}
			if (GUI.Button(1810, 10, 100, 100, _data->assets.getTexture("acceptPeaceB"), "AcceptPeaceB"))
			{
				PLC.AcceptPeaceDeal();
			}
			GUI.Panel(394, 10, 384, 144, _data->assets.getTexture("cellPanel"), "WarScorePanel");
		}
		else if (!PLC.getNationChosen())
		{
			if (GUI.Button(1620, 10, 300, 100, _data->assets.getTexture("playB"), "PlayB"))
			{
				PLC.PlayAsNation();
			}
			GUI.Panel(394, 10, 384, 144, _data->assets.getTexture("cellPanel"), "ChooseNationPanel");
		}
		else
		{
			if (GUI.Button(1600, 150, 50, 50, _data->assets.getTexture("terrainB"), "TerrainB"))
			{
				PLC.UpdateMapDrawMode(DrawTerrain);
			}
			if (GUI.Button(1655, 150, 50, 50, _data->assets.getTexture("nationB"), "NationB"))
			{
				PLC.UpdateMapDrawMode(DrawNations);
			}
			if (GUI.Button(1710, 150, 50, 50, _data->assets.getTexture("cultureB"), "CultureB"))
			{
				PLC.UpdateMapDrawMode(DrawCultures);
			}
			if (GUI.Button(1765, 150, 50, 50, _data->assets.getTexture("religionB"), "ReligionB"))
			{
				PLC.UpdateMapDrawMode(DrawReligions);
			}
			if (GUI.Button(1820, 150, 50, 50, _data->assets.getTexture("heightB"), "HeightB"))
			{
				PLC.UpdateMapDrawMode(DrawHeight);
			}
			if (GUI.Button(1545, 1020, 50, 50, _data->assets.getTexture("speed0B"), "Speed0"))
			{
				PLC.SetTickSpeed(0);
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
			if (GUI.Button(1820, 1020, 50, 50, _data->assets.getTexture("speed5B"), "Speed5"))
			{
				PLC.SetTickSpeed(5);
			}
			if (GUI.Button(600, 1020, 50, 50, _data->assets.getTexture("religionB"), "RaiseSoldiersB"))
			{
				PLC.RaiseSoldiers();
			}
			if (GUI.Button(16, 928, 64, 64, _data->assets.getTexture("diplomacyB"), "DiplomacyB"))
			{
				showDiplomacyPanel = !showDiplomacyPanel;
				showWarsPanel = false;
				diplomacyOffset = 0;
			}
			if (GUI.Button(16, 864, 64, 64, _data->assets.getTexture("warB"), "WarPB"))
			{
				showWarsPanel = !showWarsPanel;
				showDiplomacyPanel = false;
				diplomacyOffset = 0;
			}
			if (PLC.areSoldiersSelected())
			{
				GUI.Panel(10, 144, 144, 144, _data->assets.getTexture("cellPanel"), "SoldiersPanel");
				if (PLC.isArmyEnganged())
				{
					GUI.Panel(154, 144, 144, 144, _data->assets.getTexture("cellPanel"), "BattlePanel");
				}
			}
			if (showDiplomacyPanel)
			{
				GUI.Panel(80, 416, 336, 576, _data->assets.getTexture("diplomacyP"), "DiplomacyP");

				if (GUI.Button(232, 960, 32, 32, _data->assets.getTexture("upArrowB"), "UpArrowB"))
				{
					if (diplomacyOffset > 0)
					{
						diplomacyOffset--;
					}
				}
				if (GUI.Button(232, 416, 32, 32, _data->assets.getTexture("downArrowB"), "DownArrowB"))
				{
					if (diplomacyOffset + 8 < PLC.getNationsCount())
					{
						diplomacyOffset++;
					}
				}

				for (int i = 0; i < 8; i++)
				{
					float yOffset = 64.0f * i;

					if (i + diplomacyOffset != PLC.getNationID())
					{
						std::string name = "WarB" + std::to_string(i);
						if (GUI.Button(256, (904 - yOffset), 48, 48, _data->assets.getTexture("warB"), name))
						{
							PLC.WarWith(i + diplomacyOffset);
						}
						name = "PeaceB" + std::to_string(i);
						if (GUI.Button(336, (904 - yOffset), 48, 48, _data->assets.getTexture("peaceB"), name))
						{
							PLC.StartPeaceDeal(i + diplomacyOffset);
						}
					}

				}
			}
			if (showWarsPanel)
			{
				GUI.Panel(80, 416, 336, 576, _data->assets.getTexture("warP"), "WarP");

				if (GUI.Button(232, 960, 32, 32, _data->assets.getTexture("upArrowB"), "UpArrowB"))
				{
					if (diplomacyOffset > 0)
					{
						diplomacyOffset--;
					}
				}
				if (GUI.Button(232, 416, 32, 32, _data->assets.getTexture("downArrowB"), "DownArrowB"))
				{
					if (diplomacyOffset + 8 < PLC.getWarsSize())
					{
						diplomacyOffset++;
					}
				}
			}
		}

		GUI.EndFrame();


		GUI.Text("Nation:", 10.0f, 67.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text("Culture:", 10.0f, 43.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text("Religion:", 10.0f, 19.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));

		GUI.Text(PLC.getSelectedCellNationName(), 50.0f, 67.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text(PLC.getSelectedCellCultureName(), 50.0f, 43.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
		GUI.Text(PLC.getSelectedCellReligionName(), 50.0f, 19.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));

		if (PLC.isMakingPeace())
		{
			GUI.Text("Making Peace With " + PLC.getMakingPeaceWithName(), 280.0f, 575.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
			

			GUI.Text("War Score ", 177.0f, 67.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
			GUI.Text(PLC.getPeaceWarScoreWith(), 237.0f, 67.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
			GUI.Text("Score Cost", 177.0f, 24.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
			GUI.Text(PLC.getCurrentWarScoreCost(), 237.0f, 24.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
		}
		else if (!PLC.getNationChosen())
		{
			GUI.Text("Choose Nation", 280.0f, 575.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
			GUI.Text("Play As " + PLC.getSelectedCellNationName(), 177.0f, 67.0f, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
		}
		else 
		{
			GUI.Text(PLC.getNationName(), 5.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
			GUI.Text("Gold: ", 65.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
			GUI.Text(PLC.getGold(), 100.0f, 580.0f, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f));
			GUI.Text(PLC.getGoldPerTurn(), 100.0f, 575.0f, 0.15f, glm::vec3(0.0f, 0.0f, 0.0f));

			if (showDiplomacyPanel)
			{
				for (int i = 0; i < 8; i++)
				{
					float yOffset = (72.0f * i) / 2;
					GUI.Text(PLC.getNationsName(i + diplomacyOffset), 42.0f, (512 - yOffset), 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
				}
			}

			if (showWarsPanel)
			{
				GUI.Text("War With", 42.0f, 520.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
				GUI.Text("War Score", 128.0f, 520.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));

				if (PLC.getWarsSize() > 8)
				{
					for (int i = 0; i < 8; i++)
					{
						float yOffset = (72.0f * i) / 2;
						GUI.Text(PLC.getWarsNationName(i + diplomacyOffset), 42.0f, (500 - yOffset), 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
						GUI.Text(PLC.getWarsWarScore(i + diplomacyOffset), 136.0f, (500 - yOffset), 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
					}
				}
				else
				{
					for (int i = 0; i < PLC.getWarsSize(); i++)
					{
						float yOffset = (72.0f * i) / 2;
						GUI.Text(PLC.getWarsNationName(i), 42.0f, (500 - yOffset), 0.25f, glm::vec3(0.0f, 0.0f, 0.0f));
						GUI.Text(PLC.getWarsWarScore(i), 136.0f, (500 - yOffset), 0.25f, glm::vec3(0.0f, 0.0f, 0.0f));
					}
				}
			}

			if (PLC.areSoldiersSelected())
			{
				GUI.Text("Army", 10.0f, 140.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
				GUI.Text("Total: ", 10.0f, 120.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
				GUI.Text(std::to_string(PLC.getSoldiersTotal()), 35.0f, 120.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
				if (PLC.isArmyEnganged())
				{

					GUI.Text(PLC.getAttackerName(), 65.0f, 140.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
					GUI.Text(PLC.getAttackerMorale(), 100.0f, 140.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
					GUI.Text(PLC.getDefenderName(), 65.0f, 120.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
					GUI.Text(PLC.getDefenderMorale(), 100.0f, 120.0f, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
				}
			}



		}
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