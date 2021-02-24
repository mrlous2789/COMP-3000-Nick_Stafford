#pragma once
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

namespace Mer
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() { };

		void LoadTexture(std::string name, std::string filename);
		GLuint getTexture(std::string name);

		void LoadAnIcon(std::string name, std::string filename);
		GLFWimage* getIcon(std::string name);

		void CleanUp();
		void DeleteTexture(std::string name);

	private:
		std::map<std::string, GLuint> _textures;
		std::map<std::string, GLFWimage> _icons;

	};
}


