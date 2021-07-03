#include "PlayerController.h"
namespace Mer
{
	bool PlayerController::KeysPressed[348];


	void PlayerController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

	bool PlayerController::isZoomIn = false;
	bool PlayerController::isZoomOut = false;

	void PlayerController::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
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

	PlayerController::PlayerController()
	{

	}
	void PlayerController::Initialise(int screenWidth, int screenHeight)
	{
		GMC.Initialise(screenWidth, screenHeight);
		nation = GMC.getNationPointerById(0);
		AIC.Initialise(GMC.getNations(), nation->id);
		PF.Initialise(GMC.getCells());
		

		glGenBuffers(NumBuffers, playerBuffers);
		glGenBuffers(NumBuffers, textureBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), texVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		ShaderInfo  cellShaders[] =
		{
			{ GL_VERTEX_SHADER, "Shaders/player.vert" },
			{ GL_FRAGMENT_SHADER, "Shaders/player.frag" },
			{ GL_NONE, NULL }
		};

		// creating the model matrix
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(soldierXOffset, soldierYOffset, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));


		// creating the view matrix
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

		// creating the projection matrix
		projection = glm::perspective(89.535f, 1.0f, 0.1f, 20.0f);

		// Adding all matrices up to create combined matrix
		mvp = projection * view * model;

		//adding the Uniform to the shader
		int mvpLoc = glGetUniformLocation(playerShader, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;


		xCentering = (-1.0f + 0.991666675) / 2;
		yCentering = (-1.0f + 0.985185206) / 2;

		playerShader = LoadShaders(cellShaders);

		soldiersTotal = nation->nationCells.size();
	}

	void PlayerController::HandleInput()
	{
		if (isZoomIn)
			GMC.SetZoomIn(); isZoomIn = false;
		if (isZoomOut)
			GMC.SetZoomOut(); isZoomOut = false;

		GMC.ProcessKeyPresses(KeysPressed);
	}

	void PlayerController::Draw(GLuint texture)
	{
		GMC.Draw();

		if (soldiersRaised)
		{
			UpdateMVP(GMC.getZoomLevel(), GMC.getXOffset(), GMC.getYOffset());
	

			GLint myLoc = glGetUniformLocation(playerShader, "color");
			glProgramUniform4fv(playerShader, myLoc, 1, color);


			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glUseProgram(playerShader);

			glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[0]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			int mvpLoc = glGetUniformLocation(playerShader, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			glBindTexture(GL_TEXTURE_2D, texture);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}

		AIC.Draw(GMC.getZoomLevel(), GMC.getXOffset(), GMC.getYOffset(), texture);
	}
	void PlayerController::UpdateMapDrawMode(int drawMode)
	{
		GMC.UpdateDrawMode(drawMode);
	}

	void PlayerController::Update(float dt)
	{

		GMC.UpdateMap();

		AIC.Update();

		gameTickAcc += dt;
		if (gameTickAcc >= gameTickTimer)
		{
			Tick();
		}
		if (toggleSoldiers)
		{
			soldiersRaised = !soldiersRaised;
			if (!soldiersRaised)
			{
				soldiersSelected = false;
				soldierMoving = false;
				color[3] = 0.8f;
			}
			soldierXPos = nation->capital->centre.x + 1;
			soldierYPos = nation->capital->centre.y + 1;
			soldierCellID = nation->capital->id;
			toggleSoldiers = false;
		}

		if (soldierMoving)
		{
			smProgress += soldierMoveSpeed * gameSpeed;

			if (smProgress >= 0.2f)
			{
				routePos++;
				smProgress = 0.0f;
				soldierXPos = route[routePos]->centre.x + 1;
				soldierYPos = route[routePos]->centre.y + 1;
				soldierCellID = route[routePos]->id;

			}

			if (routePos == route.size() - 1)
			{
				soldierMoving = false;
			}
		}


	}
	
	void PlayerController::Tick()
	{
		goldPerTurn = nation->nationCells.size() / 10;
		gold += goldPerTurn;

		AIC.Tick();

		gameTickAcc -= gameTickTimer;
	}

	int PlayerController::getNationID()
	{
		return nation->id;
	}
	std::string PlayerController::getNationName()
	{
		return nation->name;
	}

	std::string PlayerController::getGold()
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << gold;

		std::string temp = stream.str();
		return temp;
	}
	std::string PlayerController::getGoldPerTurn()
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << goldPerTurn;
			
		std::string temp = stream.str();
		return "+ " + temp;
	}

	void PlayerController::SetTickSpeed(int speed)
	{
		switch (speed)
		{
		case 1:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 20.0f;
			gameTickTimer = 20.0f;
			gameSpeed = 1;
			break;
		case 2:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 15.0f;
			gameTickTimer = 10.0f;
			gameSpeed = 2;
			break;
		case 3:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 10.0f;
			gameTickTimer = 6.5f;
			gameSpeed = 3;
			break;
		case 4:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 6.0f;
			gameTickTimer = 5.0f;
			gameSpeed = 4;
			break;
		case 5:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 3.0f;
			gameTickTimer = 4.0f;
			gameSpeed = 5;
			break;
		default:			
			break;
		}
	}

	void PlayerController::RaiseSoldiers()
	{
		toggleSoldiers = true;
	}

	void PlayerController::UpdateMVP(float zoomLevel, float xOffset, float yOffset)
	{
		soldierXOffset = ((soldierXPos) + xCentering) + xOffset;
		soldierYOffset = ((soldierYPos) + yCentering) + yOffset;

		// creating the model matrix
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(zoomLevel, zoomLevel, 1.0f));
		model = glm::translate(model, glm::vec3(soldierXOffset, soldierYOffset, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

		// Adding all matrices up to create combined matrix
		mvp = projection * view * model;
	}

	bool PlayerController::ProcessMouseClick(double mouseX, double mouseY)
	{
		double correctedMouseX = mouseX, correctedMouseY = mouseY;
		
		correctedMouseX -= (screenWidth / 2);
		correctedMouseX = correctedMouseX / (screenWidth / 2);
		correctedMouseY -= (screenHeight / 2);
		correctedMouseY = correctedMouseY / (screenHeight / 2);
		correctedMouseY *= -1;

		correctedMouseX /= GMC.getZoomLevel();
		correctedMouseY /= GMC.getZoomLevel();

		correctedMouseX -= soldierXOffset;
		correctedMouseY -= soldierYOffset;


		if (correctedMouseX <= -0.991666675f && correctedMouseX >= -1.0f && correctedMouseY <= -0.985185206f && correctedMouseY >= -1.0f && soldiersRaised)
		{
			return true;
		}
		else if (soldiersSelected)
		{

		}
		else if (GMC.ProcessMousePress(mouseX, mouseY))
		{
			return true;
		}

		return false;
	}

	bool PlayerController::ProcessLeftMouseRelease(double mouseX, double mouseY)
	{
		double correctedMouseX = mouseX, correctedMouseY = mouseY;

		correctedMouseX -= (screenWidth / 2);
		correctedMouseX = correctedMouseX / (screenWidth / 2);
		correctedMouseY -= (screenHeight / 2);
		correctedMouseY = correctedMouseY / (screenHeight / 2);
		correctedMouseY *= -1;

		correctedMouseX /= GMC.getZoomLevel();
		correctedMouseY /= GMC.getZoomLevel();


		correctedMouseX -= soldierXOffset;
		correctedMouseY -= soldierYOffset;

		if (correctedMouseX <= -0.991666675f && correctedMouseX >= -1.0f && correctedMouseY <= -0.985185206f && correctedMouseY >= -1.0f && soldiersRaised)
		{
			std::cout << "clicked soldiers " << std::endl;
			soldiersSelected = !soldiersSelected;
			if (soldiersSelected)
			{
				color[3] = 1.0f;
			}
			else
			{
				color[3] = 0.8f;
			}
			return true;
		}
		else
		{
			if (soldiersSelected)
			{
				soldiersSelected = false;
				color[3] = 0.8f;
			}
			
		}
		return false;
	}

	bool PlayerController::ProcessRightMouseRelease(double mouseX, double mouseY)
	{
		if (soldiersSelected)
		{
			double origMouseX = mouseX, origMouseY = mouseY;

			mouseX -= (screenWidth / 2);
			mouseX = mouseX / (screenWidth / 2);
			mouseY -= (screenHeight / 2);
			mouseY = mouseY / (screenHeight / 2);
			mouseY *= -1;

			mouseX /= GMC.getZoomLevel();
			mouseY /= GMC.getZoomLevel();

			mouseX -= GMC.getXOffset();
			mouseY -= GMC.getYOffset();

			Cell* destination = GMC.getCellAtCoords(mouseX, mouseY);

			soldierMoving = true;

			route = PF.CalculatePath(&GMC.getCells()->at(soldierCellID), destination);
			routePos = 0;

			return true;
		}
		return false;
	}

	bool PlayerController::areSoldiersSelected()
	{
		return soldiersSelected;
	}

	void PlayerController::WarWith(int id)
	{
		if (!AlreadyAtWar(id))
		{
			nation->atWar.push_back(GMC.getNationPointerById(id));
			GMC.getNationPointerById(id)->atWar.push_back(nation);
		}
	}
	void PlayerController::PeaceWith(int id)
	{
		for (int i = 0; i < nation->atWar.size(); i++)
		{
			if (nation->atWar[i]->id == id)
			{
				nation->atWar.erase(nation->atWar.begin() + i);
				
			}
		}

		Nation* temp;
		temp = GMC.getNationPointerById(id);
		for (int i = 0; i < temp->atWar.size(); i++)
		{
			if (temp->atWar[i]->id == id)
			{
				temp->atWar.erase(temp->atWar.begin() + i);
			}
		}
	}

	bool PlayerController::AlreadyAtWar(int id)
	{
		for (int i = 0; i < nation->atWar.size(); i++)
		{
			if (nation->atWar[i]->id == id)
			{
				return true;
			}
		}
		return false;
	}

	int PlayerController::getNationsCount()
	{
		return GMC.getNationCount();
	}
	std::string PlayerController::getNationsName(int id)
	{
		return GMC.getNationName(id);
	}

	float PlayerController::getSoldierScreenX()
	{		
		return (((soldierXOffset - 1) * GMC.getZoomLevel()) / 2) * screenWidth;
		
	}
	float PlayerController::getSoldierScreenY()
	{
		return (((soldierYOffset - 1) * GMC.getZoomLevel()) / 2)  * screenHeight;
	}

	std::string PlayerController::getSelectedCellNationName()
	{
		return GMC.getSelectedCellNationName();
	}
	std::string PlayerController::getSelectedCellReligionName()
	{
		return GMC.getSelectedCellReligionName();
	}
	std::string PlayerController::getSelectedCellCultureName()
	{
		return GMC.getSelectedCellCultureName();
	}

	int PlayerController::getSoldiersTotal()
	{
		return soldiersTotal;
	}
}