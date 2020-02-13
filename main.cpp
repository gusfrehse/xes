#include <stdio.h>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#define VSYNC 0
#define SWIDTH 800
#define SHEIGHT 450


// Initialize external libraries and create opengl context
bool init();

// Initialize OpenGl things
bool initGL();

SDL_Window* window;

bool init()
{
	bool success = true;
	//Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Unable to initialize SDL2. SDL error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Set openGL version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		
		// Create window
		window = SDL_CreateWindow("Xes", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SWIDTH, SHEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Unable to create window. SDL error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create OpenGL context
			SDL_GLContext context = SDL_GL_CreateContext(window);
			if (context == NULL)
			{
				printf("Unable to create OpenGL context. SDL error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize Glew
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
				printf("Unable to initialize glew. Glew error: %s\n", glewGetErrorString(glewError));
				success = false;
				}

				// Vsync
				if (SDL_GL_SetSwapInterval(VSYNC) < 0)
				{
					printf("Unable to set swap interval (vsync). Continuing. SDL error: %s\n", SDL_GetError());
				}

				if (!initGL())
				{
					printf("Unable to initialize OpenGL\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL()
{
	return true;
}

int main(int argc, char** argv)
{
	printf("hello world\n");
	init();
	return 0;
}