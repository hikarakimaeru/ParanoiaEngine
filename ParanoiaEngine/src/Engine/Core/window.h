#pragma once

#include <SDL3/SDL.h>

class Window {

public:
	Window(const char* title, int width, int height);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	SDL_Window* getWin() const;
	SDL_GLContext getGl_Context() const;

private:
	SDL_Window* win = nullptr;
	SDL_GLContext gl_context = nullptr;
};