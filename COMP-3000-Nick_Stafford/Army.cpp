#include "Army.h"

namespace Mer
{
	Army::Army()
	{

	}
	void Army::Initialise(float red, float green, float blue, int maxSoldiers, float soldierXpos, float soldierYpos, int nationID)
	{
		this->maxSoldiers = maxSoldiers;
		totalSoldiers = this->maxSoldiers;
		attack = maxSoldiers / 10.0f;
		defence = maxSoldiers / 20.0f;

		this->nationID = nationID;


		glGenBuffers(NumBuffers, playerBuffers);
		glGenBuffers(NumBuffers, textureBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), texVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		ShaderInfo  soldierShaders[] =
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

		color[0] = red;
		color[1] = green;
		color[2] = blue;

		soldierShader = LoadShaders(soldierShaders);

		xCentering = (-1.0f + 0.991666675) / 2;
		yCentering = (-1.0f + 0.985185206) / 2;
	}
	void Army::Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture)
	{
		UpdateMVP(zoomLevel, xOffset, yOffset);


		GLint myLoc = glGetUniformLocation(soldierShader, "color");
		glProgramUniform4fv(soldierShader, myLoc, 1, color);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glUseProgram(soldierShader);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		int mvpLoc = glGetUniformLocation(soldierShader, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
	void Army::UpdateMVP(float zoomLevel, float xOffset, float yOffset)
	{
		soldierXOffset = ((soldierXPos)+xCentering) + xOffset;
		soldierYOffset = ((soldierYPos)+yCentering) + yOffset;

		// creating the model matrix
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(zoomLevel, zoomLevel, 1.0f));
		model = glm::translate(model, glm::vec3(soldierXOffset, soldierYOffset, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

		// Adding all matrices up to create combined matrix
		mvp = projection * view * model;
	}
	void Army::Move(float xPos, float yPos, int id)
	{
		soldierXPos = xPos;
		soldierYPos = yPos;

		locationID = id;
	}

	void Army::Selected()
	{
		if (color[3] == 1.0f)
		{
			color[3] = 0.8f;
		}
		else
		{
			color[3] = 1.0f;
		}
	}
}