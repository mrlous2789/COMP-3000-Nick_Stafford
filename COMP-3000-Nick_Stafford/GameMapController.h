#pragma once
#include "Reader.h"
#include "LoadShaders.h"
#include "glm/glm.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#include <GLFW/glfw3.h>

namespace Mer
{
	enum Drawing_Modes { DrawTerrain, DrawNations, DrawCultures, DrawReligions };

	class GameMapController
	{
	public:
		GameMapController();


		void Initialise(float screenWidth, float screenHeight);
		bool ProcessMousePress(double mouseX, double mouseY);
		void Draw();

		void ProcessKeyPresses(bool KeysPressed[]);
		void UpdateMap();


		

		void UpdateDrawMode(int drawMode);

		Nation getNationById(int id);
		Nation* getNationPointerById(int id);

		float getZoomLevel();
		float getXOffset();
		float getYOffset();

		void SetZoomIn(bool inZoom = true);
		void SetZoomOut(bool inZoom = true);


		std::vector<Cell>* getCells();

		Cell* getCellAtCoords(double xpos, double ypos);
	private:
		

		Cell* getCellbyID(int id);
		void AddCapitalToNations();

		Culture getCultureById(int id);
		Religion getReligionById(int id);

		bool LoadFromFile(std::string location, std::string mapname);

		bool Intersects(double mouseX, double mouseY, double edgeX1, double edgeY1, double edgeX2, double edgeY2);
		
		void CalculatePathingWeights();

		bool showCellBorders = true;

		float screenWidth = 0, screenHeight = 0;
		

		std::vector<Cell> cells;
		std::vector<River> rivers;
		std::vector<Nation> nations;
		std::vector<Culture> cultures;
		std::vector<Religion> religions;

		int drawMode = DrawTerrain;

		Reader reader;

		enum Buffer_Counts { NumCells = 12000, NumRivers = 200};

		GLuint cellBuffers[NumCells];
		GLuint riverBuffers[NumRivers];
		

		//MAP VARIABLES
		std::string location = ".\\Map_Files\\";
		std::string mapname = "dibenay";

		GLuint cellShader;


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

		bool isZoomOut;
		bool isZoomIn;

		bool moved = false;

		//MAP MOVEMENT FUNCTIONS
		void ZoomOut();
		void ZoomIn();

		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();

		//COLOUR FOR CELLS SHADER
		GLfloat color[4] = { 0.0f,0.0f,1.0f,1.0f };
	};
}


