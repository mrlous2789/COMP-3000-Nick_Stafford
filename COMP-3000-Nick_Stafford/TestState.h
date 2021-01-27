#pragma once
#include "State.h"
#include "Game.h"
#include "GameMapController.h"
#include "LoadShaders.h"
#include <glm/glm.hpp>
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr

namespace Mer
{
	class TestState : public State
	{
	public:
		TestState(GameDataRef data);
		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);
		void CleanUp();

	private:
		GameDataRef _data;//global data used by program

		enum VAO_IDs { Cells, NumVAOs = 2 };
		enum Buffer_Counts { NumCells = 12000, NumRivers = 200};

		GLuint VAOs[NumVAOs];
		GLuint cellBuffers[NumCells];		
		GLuint riverBuffers[NumRivers];

		//MAP VARIABLES
		GameMapController GMC;
		std::string location = ".\\Map_Files\\";
		std::string mapname = "dibenay";

		GLuint shader;

	

		//CELL SELECTION VARIABLES
		double xpos = 0, ypos = 0;
		int windowH = 0, windowW = 0;

		Cell* selectedCell;


		//MAP MOVEMENT VARIABLES
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 mvp;

		float zoomLevel = 1.0f;
		float zoomRate = 0.05f;
		float minZoom = 1.0f;
		float maxZoom = 3.0f;

		float xoffset = 0.0f;
		float yoffset = 0.0f;
		float moveSpeed = 0.01f;

		bool isMoveUp = false;
		bool isMoveDown = false;
		bool isMoveRight = false;
		bool isMoveLeft = false;

		static bool isZoomOut;
		static bool isZoomIn;

		bool moved = false;

		//MAP MOVEMENT FUNCTIONS
		void ZoomOut();
		void ZoomIn();

		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		//COLOUR FOR CELLS SHADER
		GLfloat color[3] = { 0.0f,0.0f,1.0f };

	};
}


