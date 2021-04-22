#include "Game.h"
#include "MainMenuState.h"


#include "stb_image.h"
namespace Mer
{
	Game::Game()
	{
		if (Init())
		{
			this->_data->machine.AddState(StateRef(new MainMenuState(_data)));
			Run();
		}
	}
	
	bool Game::Init()//Initialise glfw and glew Setup window
	{


		glfwInit();


		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		this->_data->window = glfwCreateWindow(1920, 1080, "Comp-3000", NULL, NULL);
		glfwMakeContextCurrent(this->_data->window);
		
		glewInit();
		
		this->_data->assets.LoadAnIcon("32x32", icon32x32Filename);

		glfwSetWindowIcon(_data->window, 1, this->_data->assets.getIcon("32x32"));




		return true;
	}

	void Game::Run()
	{
		UINT32 newTime, frameTime, interpolation;//used for timing Update and HandleInput calls

		UINT32 currentTime = glfwGetTime();//Time since glfw was initialised

		UINT32 accumaltor = 0;//accumalation of time

		while (!glfwWindowShouldClose(this->_data->window))
		{
			this->_data->machine.ProcessStateChanges();

			newTime = glfwGetTime();
			frameTime = newTime - currentTime;

			if (frameTime > 250)
				frameTime = 250;

			currentTime = newTime;//change current time to new time
			accumaltor += frameTime;


			this->_data->machine.GetActiveState()->HandleInput();
			this->_data->machine.GetActiveState()->Update(frameTime);



			interpolation = accumaltor / dt;
			this->_data->machine.GetActiveState()->Draw(interpolation);		
		}

		CleanUp();
	}

	void Game::CleanUp()
	{
		glfwDestroyWindow(this->_data->window);

		glfwTerminate();	
	}
}