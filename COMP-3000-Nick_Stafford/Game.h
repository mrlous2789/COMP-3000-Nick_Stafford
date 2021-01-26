#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3.h"


#include <string>

namespace Mer
{
	class Game
	{
	public:
		Game();
	private:
		bool Init();
		void Run();

		void CleanUp();

		GLFWwindow* window;
	};
}


