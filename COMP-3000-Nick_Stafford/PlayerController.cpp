#include "PlayerController.h"
namespace Mer
{
	PlayerController::PlayerController()
	{

	}
	void PlayerController::Initialise(Nation* selectedNation)
	{
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
		

		nation = selectedNation;

		FindCentreofCapital();

		xCentering = (-1.0f + 0.947916687) / 2;
		yCentering = (-1.0f + 0.907407403) / 2;

		playerShader = LoadShaders(cellShaders);
	}
	void PlayerController::Draw(GLuint texture)
	{
		if (soldiersRaised)
		{
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
	}

	void PlayerController::Tick(float dt)
	{
		gameTickAcc += dt;
		if (gameTickAcc >= gameTickTimer)
		{
			goldPerTurn = nation->nationCells.size() / 10;
			gold += goldPerTurn;

			gameTickAcc -= gameTickTimer;
		}

		if (toggleSoldiers)
		{
			soldiersRaised = !soldiersRaised;
			soldierXPos = capitalXPos;
			soldierYPos = capitalYPos;
			UpdateMVP();
			toggleSoldiers = false;
		}
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
			break;
		case 2:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 15.0f;
			gameTickTimer = 15.0f;
			break;
		case 3:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 10.0f;
			gameTickTimer = 10.0f;
			break;
		case 4:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 6.0f;
			gameTickTimer = 6.0f;
			break;
		case 5:
			gameTickAcc = (gameTickAcc / gameTickTimer) * 3.0f;
			gameTickTimer = 3.0f;
			break;
		default:			
			break;
		}
	}

	void PlayerController::RaiseSoldiers()
	{
		toggleSoldiers = true;
	}

	void PlayerController::UpdateMVP()
	{
		soldierXOffset = (soldierXPos + 1) + xCentering;
		soldierYOffset = (soldierYPos + 1) + yCentering;

		// creating the model matrix
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(soldierXOffset, soldierYOffset, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

		// Adding all matrices up to create combined matrix
		mvp = projection * view * model;
	}

	void PlayerController::FindCentreofCapital()
	{
		float xBig = -10.0f, xSmall = 10.0f, yBig = -10.0f, ySmall = 10.0f;

		for (int i = 0; i < nation->capital->coords.size(); i++)
		{
			if (nation->capital->coords[i].x > xBig)
			{
				xBig = nation->capital->coords[i].x;
			}
			if (nation->capital->coords[i].x < xSmall)
			{
				xSmall = nation->capital->coords[i].x;
			}
			if (nation->capital->coords[i].y > yBig)
			{
				yBig = nation->capital->coords[i].y;
			}
			if (nation->capital->coords[i].y < ySmall)
			{
				ySmall = nation->capital->coords[i].y;
			}
		}

		capitalXPos = (xSmall + xBig) / 2;
		capitalYPos = (ySmall + yBig) / 2;

		std::cout << "X: " << capitalXPos << " Y: " << capitalYPos << std::endl;
	}
}