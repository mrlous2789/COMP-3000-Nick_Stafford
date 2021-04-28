#include "PlayerController.h"
namespace Mer
{
	PlayerController::PlayerController()
	{

	}
	void PlayerController::Initialise()
	{
		glGenBuffers(NumBuffers, playerBuffers);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[Vertex]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(GLfloat)), Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[Texture]);
		glBufferData(GL_ARRAY_BUFFER, 4 * (2 * sizeof(GLfloat)), Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		ShaderInfo  cellShaders[] =
		{
			{ GL_VERTEX_SHADER, "Shaders/player.vert" },
			{ GL_FRAGMENT_SHADER, "Shaders/player.frag" },
			{ GL_NONE, NULL }
		};

		playerShader = LoadShaders(cellShaders);
	}
	void PlayerController::Draw(GLuint texture)
	{
		glUseProgram(playerShader);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[Vertex]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, playerBuffers[Texture]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	int PlayerController::getNationID()
	{
		return nationID;
	}
}