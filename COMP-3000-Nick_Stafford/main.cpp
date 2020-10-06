#include <SDL.h>
#include <iostream>

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) //if SDL fails to initalise output to console
	{
		std::cout << "SDL failed to initialise" << std::endl;
		return false;
	}
	return true;
}
void Init() //create sdl window
{
	SDL_Window* window = SDL_CreateWindow("COMP-3000", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, NULL);
}
void Run()
{
	bool running = true; //to keep the program open 
	while (running)
	{
		SDL_Event event;//contains infomation for last event
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE://if the close button is pressed close window
					running = false;//set running to false to end the program
					break;
				default:
					//std::cout << "Unhandled window event: " << event.type << std::endl;
					break;
				}

			default:
				//std::cout << "Unhandled SDL event: " << event.type << std::endl;
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if (InitSDL())// if SDL initialises run program
	{
		Init();//create window
		Run();//poll events such as window being told to close
	}
	return EXIT_SUCCESS;
}