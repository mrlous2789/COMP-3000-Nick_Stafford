#include "AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace Mer
{
	AssetManager::AssetManager()
	{

	}
	void AssetManager::LoadTexture(std::string name, std::string filename)
	{
		stbi_set_flip_vertically_on_load(true);

		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLint width, height, nrChannels;

		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glewExperimental = GL_TRUE;
			
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);
		this->_textures[name] = tex;
	}

	GLuint AssetManager::getTexture(std::string name)
	{
		return this->_textures[name];
	}

	void AssetManager::LoadAnIcon(std::string name, std::string filename)
	{
		this->_icons[name].pixels = stbi_load(filename.c_str(), &this->_icons[name].width, &this->_icons[name].height, 0, 4);
	}
	GLFWimage* AssetManager::getIcon(std::string name)
	{
		return &this->_icons[name];
	}

	void AssetManager::DeleteTexture(std::string name)
	{
		if (_textures.find(name) != _textures.end())
		{
			_textures.erase(name);
		}
	}

	void AssetManager::CleanUp()
	{
		//_textures.clear();
	}
}