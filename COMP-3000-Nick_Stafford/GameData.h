#pragma once

#include "StateMachine.h"
#include <GLFW/glfw3.h>
#include "AssetManager.h"

namespace Mer
{
	struct GameData
	{
		StateMachine machine;
		GLFWwindow* window;
		AssetManager assets;
	};

	typedef std::shared_ptr<GameData> GameDataRef;
}
