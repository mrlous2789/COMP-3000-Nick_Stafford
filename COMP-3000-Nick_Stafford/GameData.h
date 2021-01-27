#pragma once

#include "StateMachine.h"
#include <GLFW/glfw3.h>

namespace Mer
{
	struct GameData
	{
		StateMachine machine;
		GLFWwindow* window;
	};

	typedef std::shared_ptr<GameData> GameDataRef;
}
