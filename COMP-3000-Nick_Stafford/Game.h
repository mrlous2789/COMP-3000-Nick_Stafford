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

		GameDataRef _data = std::make_shared<GameData>();
	};
}


