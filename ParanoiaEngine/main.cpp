#include <glad/glad.h>
#include <iostream>
#include <format>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void cleanup(SDL_Window* win, SDL_GLContext gl_context);

int main(int argc, char* argv[]) {
	std::cout << std::format("Welcome to the engine!\n");
	
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* win = SDL_CreateWindow("ParanoiaEngine", 800, 600, SDL_WINDOW_OPENGL);
	if (!win) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
		cleanup(win, nullptr);
		return 1;
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(win);
	
	if (!gl_context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create OpenGL context", nullptr);
		cleanup(win, nullptr);
		return 1;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize GLAD", nullptr);
		cleanup(win, gl_context);
		return 1;
	}

	SDL_GL_SetSwapInterval(1);
	
	glViewport(0, 0, 800, 600);

	float vertices[] = { 0.0f,  0.5f, -0.5f, -0.5f, 0.5f, -0.5f };

	GLuint VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	GLuint VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	bool running = true;

	while (running) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		glClearColor(0.1f, 0.1f, 0.12f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(win);

	}
	
	cleanup(win, gl_context);
	return 0;
}

void cleanup(SDL_Window* win, SDL_GLContext gl_context) {
	if (gl_context) {
		SDL_GL_DestroyContext(gl_context);
	}
	if (win) {
		SDL_DestroyWindow(win);
	}
	SDL_Quit();
}