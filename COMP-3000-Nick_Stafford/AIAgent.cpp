#include "AIAgent.h"
namespace Mer
{
	AIAgent::AIAgent()
	{

	}

	void AIAgent::Initialise(Nation* nation)
	{
		this->nation = nation;

		goldPerTurn = this->nation->nationCells.size() / 10.0f;
		soldiersTotal = this->nation->nationCells.size();
		soldierCellID = this->nation->capitalId;

		glGenBuffers(NumBuffers, playerBuffers);
		glGenBuffers(NumBuffers, textureBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), texVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		ShaderInfo  agentShaders[] =
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

		agentShader = LoadShaders(agentShaders);

		color[0] = nation->colour[0];
		color[1] = nation->colour[1];
		color[2] = nation->colour[2];

		xCentering = (-1.0f + 0.991666675) / 2;
		yCentering = (-1.0f + 0.985185206) / 2;
	}
	void AIAgent::Update()
	{
		if (!nation->atWar.empty() && !isAtWar)
		{
			isAtWar = true;
			toggleSoldiers = true;
		}
		if (isAtWar && nation->atWar.empty())
		{
			isAtWar = false;
			toggleSoldiers = true;
		}

		if (toggleSoldiers)
		{
			soldiersRaised = !soldiersRaised;
			soldierXPos = nation->capital->centre.x + 1;
			soldierYPos = nation->capital->centre.y + 1;
			soldierCellID = nation->capital->id;
			toggleSoldiers = false;
		}
	}
	void AIAgent::Tick()
	{

	}
	void AIAgent::Draw(float zoomLevel, float xOffset, float yOffset, GLuint texture)
	{
		if (soldiersRaised)
		{
			UpdateMVP(zoomLevel, xOffset, yOffset);


			GLint myLoc = glGetUniformLocation(agentShader, "color");
			glProgramUniform4fv(agentShader, myLoc, 1, color);


			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glUseProgram(agentShader);

			glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[0]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, textureBuffer[0]);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			int mvpLoc = glGetUniformLocation(agentShader, "mvp");
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

			glBindTexture(GL_TEXTURE_2D, texture);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}
	}
	void AIAgent::UpdateMVP(float zoomLevel, float xOffset, float yOffset)
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
}