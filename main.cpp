#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#define VSYNC 0
#define SWIDTH 800
#define SHEIGHT 450


// Initialize external libraries and create opengl context
bool init();

// Render
void render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

}

// Initialize OpenGl things
GLint createShader(std::string shaderSource);

// Helper function to read the shader sources from one file
// Use "arandomstring\n" to separate the two shaders.
std::pair<std::string, std::string> source2shader(std::string path);

// Debug for when shaders fail
void logShader(GLuint shader);

// Debug for when shader programs fail
void logProgram(GLuint program);

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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
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
			}
		}
	}

	return success;
}

GLint createShader(std::string shaderSource)
{
	// Create a program
	GLint shaderProgram = glCreateProgram();

	// Create shaders
	GLint vertexS = glCreateShader(GL_VERTEX_SHADER);
	GLint fragS = glCreateShader(GL_FRAGMENT_SHADER);

	// Find source files
	std::pair<std::string, std::string> sources = source2shader(shaderSource);
	const GLchar* vertexSource = sources.first.c_str();
	glShaderSource(vertexS, 1, &vertexSource, NULL);
	const GLchar* fragmentSource = sources.second.c_str();
	glShaderSource(fragS, 1, &fragmentSource, NULL);

	// Compile vertex shader
	glCompileShader(vertexS);
	GLint status = GL_FALSE;
	glGetShaderiv(vertexS, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		printf("Vertex shader compilation failed. Index: %d\n", vertexS);
		logShader(vertexS);
	}

	// Compile fragment shader
	glCompileShader(fragS);
	status = GL_FALSE;
	glGetShaderiv(fragS, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		printf("Fragment shader compilation failed. Index: %d\n", fragS);
		logShader(fragS);
	}

	// Attach shader to program and link them
	glAttachShader(shaderProgram, vertexS);
	glAttachShader(shaderProgram, fragS);
	
	glLinkProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		printf("Shader program linking failed. Program index: %d\n", shaderProgram);
		logProgram(shaderProgram);
	}

	return shaderProgram;
}

std::pair<std::string, std::string> source2shader(std::string path)
{
	std::string sources[2];
	std::string vs;
	std::string fs;
	std::stringstream buffer;

	std::ifstream source(path);
	if (!source.is_open())
	{
		printf("Unable to read shader source.");
		return std::make_pair("error","error");
	}
	else
	{
		buffer << source.rdbuf();
		size_t pos = buffer.str().find("arandomstring");
		if (pos == std::string::npos)
		{ 
			printf("Invalid shader file. Recall the vertex shader should be first,"
				"then the string \"arandomstring\", then the fragment shader");
			return std::make_pair("ERROR","CHECK LOGS");
		}
		else
		{
		vs = sources[0] = buffer.str().substr(0, pos);
		fs = sources[1] = buffer.str().substr(pos + 14); // 14 == size_in_chars("arandomstring\n")

		return std::make_pair(vs,fs);
		}
	}
}

void logShader(GLuint shader)
{
	if (glIsShader(shader))
	{
		int logLength = 0;
		int maxLength = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		char* log = new char[maxLength];

		glGetShaderInfoLog(shader, maxLength, &logLength, log);
		if (logLength > 0)
		{
			printf("%s\n", log);
		}

		delete[] log;
	}
	else
	{
		printf("The shader %d is not a shader, but was requested to be logged as one\n", shader);
	}
}

void logProgram(GLuint program)
{
	if (glIsProgram(program))
	{
		int logLength = 0;
		int maxLength = 0;
		
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		char* log = new char[maxLength];

		glGetProgramInfoLog(program, maxLength, &logLength, log);
		if (logLength > 0)
		{
			printf("%s\n", log);
		}

		delete[] log;
	}
	else
	{
		printf("The program %d is not a program, but was requested to be logged as one\n", program);
	}
}


int main(int argc, char** argv)
{
	printf("hello world\n");
	init();
	GLint shaderProgram = createShader("test.glsl");

	float vertices[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f, 0.5f
	};

	// Gen VBO and VAO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Main loop quit flag
	bool shouldQuit = false;
	SDL_Event e;
	while (!shouldQuit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				shouldQuit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_SPACE:
					printf("the space key was pressed\n");
					break;
				default:
					printf("a key was presssed\n");
					break;
				}
			}
		}

		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}