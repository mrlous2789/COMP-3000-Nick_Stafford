#include "Game.h"
namespace Mer
{
	Game::Game()
	{
		if (Init())
		{
			Run();
		}
	}
	
	bool Game::Init()
	{
		if (!glfwInit())
		{
			return false;
		}
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		window = glfwCreateWindow(1920, 1080, "Comp-3000", NULL, NULL);
		glfwMakeContextCurrent(window);

		glewInit();

		return true;
	}

	void Game::Run()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}

		CleanUp();
	}

	void Game::CleanUp()
	{
		glfwDestroyWindow(window);

		glfwTerminate();
	}
}