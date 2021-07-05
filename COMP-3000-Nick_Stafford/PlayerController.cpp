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
		
		army.Initialise(nation->colour[0], nation->colour[1], nation->colour[2], nation->nationCells.size(), nation->capital->centre.x + 1, nation->capital->centre.y + 1);


		
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

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
			army.Draw(GMC.getZoomLevel(), GMC.getXOffset(), GMC.getYOffset(), texture);
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
				army.Selected();
			}
			army.Move(nation->capital->centre.x + 1, nation->capital->centre.y + 1, nation->capital->id);
			toggleSoldiers = false;
		}

		if (soldierMoving)
		{
			smProgress += soldierMoveSpeed * gameSpeed;

			if (smProgress >= 0.2f)
			{
				routePos++;
				smProgress = 0.0f;
				army.Move(route[routePos]->centre.x + 1, route[routePos]->centre.y + 1, route[routePos]->id);
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

		correctedMouseX -= army.soldierXOffset;
		correctedMouseY -= army.soldierYOffset;


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


		correctedMouseX -= army.soldierXOffset;
		correctedMouseY -= army.soldierYOffset;

		if (correctedMouseX <= -0.991666675f && correctedMouseX >= -1.0f && correctedMouseY <= -0.985185206f && correctedMouseY >= -1.0f && soldiersRaised)
		{
			std::cout << "clicked soldiers " << std::endl;
			soldiersSelected = !soldiersSelected;
			if (soldiersSelected)
			{
				army.Selected();
			}
			else
			{
				army.Selected();
			}
			return true;
		}
		else
		{
			if (soldiersSelected)
			{
				soldiersSelected = false;
				army.Selected();
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

			route = PF.CalculatePath(&GMC.getCells()->at(army.locationID), destination);
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

	//float PlayerController::getSoldierScreenX()
	//{		
	//	return (((soldierXOffset - 1) * GMC.getZoomLevel()) / 2) * screenWidth;
	//	
	//}
	//float PlayerController::getSoldierScreenY()
	//{
	//	return (((soldierYOffset - 1) * GMC.getZoomLevel()) / 2)  * screenHeight;
	//}

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
		return army.totalSoldiers;
	}
}