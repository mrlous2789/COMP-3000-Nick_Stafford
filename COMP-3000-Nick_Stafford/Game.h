#pragma once
#include "GL/glew.h"

#include "GL/freeglut.h"

#include "GameData.h"

#include <string>

namespace Mer
{


	class Game
	{
	public:
		Game();
	private:
		UINT32 dt = 1000 / 60;

		bool Init();
		void Run();

		void CleanUp();

		GLFWimage icons[2];
		char icon16x16Filename[128] = "Assets\\Icons\\icon16x16.png";//icon file paths
		char icon32x32Filename[128] = "Assets\\Icons\\icon32x32.png";
		

		GameDataRef _data = std::make_shared<GameData>();
	};
}


