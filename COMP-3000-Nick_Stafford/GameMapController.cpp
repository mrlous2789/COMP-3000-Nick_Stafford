#include "GameMapController.h"
namespace Mer
{


	GameMapController::GameMapController()
	{

	}
	void GameMapController::Initialise(float screenWidth, float screenHeight)
	{
		glGenBuffers(NumCells, cellBuffers);
		glGenBuffers(NumRivers, riverBuffers);

		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		if (LoadFromFile(location,mapname))
		{
			std::cout << "Loaded" << std::endl;
		}

		AddCapitalToNations();

		selectedCell = &cells[0];

		for (int i = 0; i < cells.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, cellBuffers[i]);
			glBufferData(GL_ARRAY_BUFFER, cells[i].coords.size() * sizeof(glm::vec3), &cells[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
		for (int i = 0; i < rivers.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, riverBuffers[i]);
			glBufferData(GL_ARRAY_BUFFER, rivers[i].coords.size() * sizeof(glm::vec3), &rivers[i].coords.front(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		ShaderInfo  cellShaders[] =
		{
			{ GL_VERTEX_SHADER, "Shaders/cells.vert" },
			{ GL_FRAGMENT_SHADER, "Shaders/cells.frag" },
			{ GL_NONE, NULL }
		};

		cellShader = LoadShaders(cellShaders);

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
		int mvpLoc = glGetUniformLocation(cellShader, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));


		std::cout << "starting calc" << std::endl;
		CalculatePathingWeights();
		std::cout << "done" << std::endl;
	}

	bool GameMapController::LoadFromFile(std::string location, std::string mapname)
	{
		//construct file locations and load them
		cells = reader.ReadCellFile(location + mapname + ".geojson");
		rivers = reader.ReadRiversFile(location  + mapname + "_rivers.geojson");
		nations = reader.ReadNationFile(location  + mapname + "_nations.mer");
		cultures = reader.ReadCutlureFile(location + mapname + "_cultures.mer");
		religions = reader.ReadReligionFile(location + mapname + "_religions.mer");

		


		for (int i = 0; i < cells.size(); i++)
		{		
			if (getNationPointerById(cells[i].state) != nullptr )
			{
				getNationPointerById(cells[i].state)->nationCells.push_back(&cells[i]);
			}
		}

		if (cells.empty())
			return false;
		else
			return true;
	}

	Cell* GameMapController::getCellbyID(int id)
	{
		for (int i = 0; i < cells.size(); i++)
		{
			if (cells[i].id == id)
			{
				return &cells[i];
			}
		}
	}
	void GameMapController::AddCapitalToNations()
	{
		for (int i = 0; i < nations.size(); i++)
		{
			nations[i].capital = getCellbyID(nations[i].capitalId);
		}
	}

	void GameMapController::SetZoomIn(bool inZoom)
	{
		isZoomIn = inZoom;
	}
	void GameMapController::SetZoomOut(bool inZoom)
	{
		isZoomOut = inZoom;
	}

	void GameMapController::ProcessKeyPresses(bool KeysPressed[])
	{

		if (KeysPressed[GLFW_KEY_W])
			isMoveUp = true;
		if (KeysPressed[GLFW_KEY_S])
			isMoveDown = true;
		if (KeysPressed[GLFW_KEY_A])
			isMoveLeft = true;
		if (KeysPressed[GLFW_KEY_D])
			isMoveRight = true;
	}
	bool GameMapController::ProcessMousePress(double mouseX, double mouseY)
	{
		mouseX -= (screenWidth / 2);
		mouseX = mouseX / (screenWidth / 2);
		mouseY -= (screenHeight / 2);
		mouseY = mouseY / (screenHeight / 2);
		mouseY *= -1;

		mouseX /= zoomLevel;

		mouseY /= zoomLevel;

		mouseX -= xoffset;
		mouseY -= yoffset;

		
		Cell* temp = getCellAtCoords(mouseX, mouseY);

		if (selectedCell != temp && temp != nullptr)
		{
			selectedCell = temp;
		
			std::cout << selectedCell->id << std::endl;
			//std::cout << getNationPointerById(selectedCell->state)->name << std::endl;
		}

		if (temp != nullptr)
		{
			return true;
		}

		return false;
	}
	void GameMapController::UpdateMap()
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
	}

	void GameMapController::Draw()
	{
		glEnableVertexAttribArray(0);

		glUseProgram(cellShader);


		for (int i = 0; i < cells.size(); i++)
		{
			if (drawMode == DrawNations)
			{
				Nation nat = getNationById(cells[i].state);
				if (cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else if (nat.id == -1 && cells[i].type == "ocean")
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
					color[3] = 1.0f;
				}
				else if (nat.id == -1 && cells[i].type != "ocean")
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else
				{
					color[0] = nat.colour[0];
					color[1] = nat.colour[1];
					color[2] = nat.colour[2];
					color[3] = 1.0f;
				}
			}
			else if (drawMode == DrawCultures)
			{
				Culture cult = getCultureById(cells[i].culture);
				if (cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else if (cult.id == -1 && cells[i].type == "ocean")
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
					color[3] = 1.0f;
				}
				else if (cult.id == -1 && cells[i].type != "ocean")
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else
				{
					color[0] = cult.colour[0];
					color[1] = cult.colour[1];
					color[2] = cult.colour[2];
					color[3] = 1.0f;
				}

			}
			else if (drawMode == DrawReligions)
			{
				Religion rel = getReligionById(cells[i].religion);
				if (cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else if (rel.id == -1 && cells[i].type == "ocean")
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
					color[3] = 1.0f;
				}
				else if (rel.id == -1 && cells[i].type != "ocean")
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else
				{
					color[0] = rel.colour[0];
					color[1] = rel.colour[1];
					color[2] = rel.colour[2];
					color[3] = 1.0f;
				}
			}
			else if (drawMode == 4)
			{
				if (cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else if (cells[i].id == 782)
				{
					color[0] = 1.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
					color[3] = 1.0f;
				}
				else if (cells[i].id > 8000)
				{
					color[0] = 1.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else if (cells[i].height > 6000)
				{
					color[0] = 1.0f;
					color[1] = 0.18f;
					color[2] = 0.18f;
					color[3] = 1.0f;
				}
				else if (cells[i].height > 3000)
				{
					color[0] = 1.0f;
					color[1] = 0.4f;
					color[2] = 0.4f;
					color[3] = 1.0f;
				}
				else if (cells[i].height > 1000)
				{
					color[0] = 1.0f;
					color[1] = 0.7f;
					color[2] = 0.7f;
					color[3] = 1.0f;
				}
				else if (cells[i].height > 0)
				{
					color[0] = 0.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
					color[3] = 1.0f;
				}
			}
			else
			{
				if (cells[i].id == selectedCell->id)
				{
					color[0] = 1.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
					color[3] = 1.0f;
				}
				else if (cells[i].biome == 4)
				{
					color[0] = 0.61f;
					color[1] = 0.84f;
					color[2] = 0.56f;
					color[3] = 1.0f;
				}
				else if (cells[i].biome == 9)
				{
					color[0] = 0.29f;
					color[1] = 0.42f;
					color[2] = 0.2f;
					color[3] = 1.0f;
				}
				else if (cells[i].biome == 8)
				{
					color[0] = 0.25f;
					color[1] = 0.61f;
					color[2] = 0.26f;
					color[3] = 1.0f;
				}
				else if (cells[i].biome == 10)
				{
					color[0] = 0.59f;
					color[1] = 0.47f;
					color[2] = 0.29f;
					color[3] = 1.0f;
				}
				else if (cells[i].biome == 11)
				{
					color[0] = 0.84f;
					color[1] = 0.91f;
					color[2] = 0.92f;
					color[3] = 1.0f;
				}
				else if (cells[i].biome == 6)
				{
					color[0] = 0.16f;
					color[1] = 0.74f;
					color[2] = 0.34f;
					color[3] = 1.0f;
				}
				else
				{
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
					color[3] = 1.0f;
				}
			}

			GLint myLoc = glGetUniformLocation(cellShader, "color");
			glProgramUniform4fv(cellShader, myLoc, 1, color);

			int mvpLoc = glGetUniformLocation(cellShader, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			//draw cells
			glBindBuffer(GL_ARRAY_BUFFER, cellBuffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, cells[i].coords.size());

			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
			color[3] = 1.0f;

			glLineWidth(1.0f);

			if (showCellBorders)
			{
				myLoc = glGetUniformLocation(cellShader, "color");
				glProgramUniform4fv(cellShader, myLoc, 1, color);

				glDrawArrays(GL_LINE_LOOP, 0, cells[i].coords.size());
			}
		}

		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 1.0f;
		color[3] = 1.0f;

		

		for (int i = 0; i < rivers.size(); i++)
		{
			glLineWidth(rivers[i].width * zoomLevel * 2.0f);
			GLint myLoc = glGetUniformLocation(cellShader, "color");
			glProgramUniform4fv(cellShader, myLoc, 1, color);

			int mvpLoc = glGetUniformLocation(cellShader, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			glBindBuffer(GL_ARRAY_BUFFER, riverBuffers[i]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_LINE_STRIP, 0, rivers[i].coords.size());
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
	}

	void GameMapController::UpdateDrawMode(int drawMode)
	{
		this->drawMode = drawMode;
	}

	Nation* GameMapController::getNationPointerById(int id)
	{
		Nation nat;
		nat.id = -1;
		nat.capitalId = -1;

		if (id >= 0 && id < nations.size())//if nation at index of id is correct nation return it
			if (nations[id].id == id)
				return &nations[id];

		for (int i = 0; i < nations.size(); i++)//loop through all nations to find correct one
			if (nations[i].id == id)
				return &nations[i];

		return nullptr;
	}

	Nation GameMapController::getNationById(int id)
	{
		Nation nat;
		nat.id = -1;
		nat.capitalId = -1;

		if (id == -1)
		{
			return nat;
		}

		if (id >= 0 && id < nations.size())//if nation at index of id is correct nation return it
			if (nations[id].id == id)
				return nations[id];

		for (int i = 0; i < nations.size(); i++)//loop through all nations to find correct one
			if (nations[i].id == id)
				return nations[i];

		return nat;
	}
	Culture GameMapController::getCultureById(int id)
	{
		Culture cult;
		cult.id = -1;
		if (id >= 0 && id < cultures.size())//if culture at index of id is correct culture return it
			if (cultures[id].id == id)
				return cultures[id];

		for (int i = 0; i < cultures.size(); i++)//loop through all cultures to find correct one
			if (cultures[i].id == id)
				return cultures[i];


		return cult;
	}
	Religion GameMapController::getReligionById(int id)
	{
		Religion rel;
		rel.id = -1;

		if (id >= 0 && id < religions.size())//if religion at index of id is correct religion return it
			if (religions[id].id == id)
				return religions[id];

		for (int i = 0; i < religions.size(); i++)//loop through all religions to find correct one
			if (religions[i].id == id)
				return religions[i];


		return rel;
	}

	Cell* GameMapController::getCellAtCoords(double xpos, double ypos)
	{
		float height = 0;

		for (int i = 0; i < cells.size(); i++)
		{
			int counter = 0;
			for (int j = 0, k = cells[i].coords.size() - 1; j < cells[i].coords.size(); k = j++)
			{
				if (Intersects(xpos, ypos, cells[i].coords[j].x, cells[i].coords[j].y, cells[i].coords[k].x, cells[i].coords[k].y))
				{
					counter++;
				}
			}
			if (counter % 2 != 0)
			{
				return &cells[i];
			}
		}
		//if cell is not found e.g. mouse if off screen return a nullptr
		return nullptr;
	}
	bool GameMapController::Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2)
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

	//map movements functions
	void GameMapController::ZoomIn()
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
	void GameMapController::ZoomOut()
	{
		if (zoomLevel - zoomRate > minZoom)
		{
			zoomLevel -= zoomRate;

			moved = true;

			isZoomOut = false;


			//if the new zoom level causes off map to be shown change offset
			if ((yoffset - (1.0f / zoomLevel)) < -1.1f)
			{
				yoffset = -1.1f + (1.0f / zoomLevel);
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

	void GameMapController::MoveUp()
	{
		if ((yoffset - (1.0f / zoomLevel)) > -1.1f)
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
	void GameMapController::MoveDown()
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
	void GameMapController::MoveLeft()
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
	void GameMapController::MoveRight()
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
	void GameMapController::CalculatePathingWeights()
	{
		for (int i = 0; i < cells.size(); i++)
		{
			for (int j = 0; j < cells[i].neighbors.size(); j++)
			{
				double distance;

				double a = abs(abs(cells[i].centre.x) - abs(getCellbyID(cells[i].neighbors[j].first)->centre.x));
				double b = abs(abs(cells[i].centre.y) - abs(getCellbyID(cells[i].neighbors[j].first)->centre.y));

				distance = sqrt((a * a) + (b * b));

				cells[i].neighbors[j].second = distance;
			}
		}
	}
	std::vector<Cell>* GameMapController::getCells()
	{
		return &cells;
	}

	float GameMapController::getZoomLevel()
	{
		return zoomLevel;
	}
	float GameMapController::getXOffset()
	{
		return xoffset;
	}
	float GameMapController::getYOffset()
	{
		return yoffset;
	}

	int GameMapController::getNationCount()
	{
		return nations.size();
	}
	std::string GameMapController::getNationName(int id)
	{
		return nations[id].name;
	}

	std::string GameMapController::getSelectedCellNationName()
	{
		if (selectedCell->state != -1)
		{
			return getNationById(selectedCell->state).name;
		}
		return "None";
	}
	std::string GameMapController::getSelectedCellReligionName()
	{
		if (selectedCell->religion != -1)
		{
			return getReligionById(selectedCell->religion).name;
		}
		return "None";
	}
	std::string GameMapController::getSelectedCellCultureName()
	{
		if (selectedCell->culture != -1)
		{
			return getCultureById(selectedCell->culture).name;
		}
		return "None";
	}

	std::vector<Nation>* GameMapController::getNations()
	{
		return &nations;
	}
}