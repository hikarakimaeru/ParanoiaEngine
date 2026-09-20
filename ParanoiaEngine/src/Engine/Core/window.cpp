#include "window.h"

#include "glad/glad.h"

Window::Window(const char* title, int width, int height) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		return;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	win = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);

	if (!win) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
		return;
	}

	gl_context = SDL_GL_CreateContext(win);

	if (!gl_context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create OpenGL context", nullptr);
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize GLAD", nullptr);
		return;
	}
	SDL_GL_SetSwapInterval(1);
	glViewport(0, 0, width, height);
}
Window::~Window() {
	if (gl_context) {
		SDL_GL_DestroyContext(gl_context);
	}
	if (win) {
		SDL_DestroyWindow(win);
	}
	SDL_Quit();
}

SDL_Window* Window::getWin() const {
	return win;
}
SDL_GLContext Window::getGl_Context() const {
	return gl_context;
}
