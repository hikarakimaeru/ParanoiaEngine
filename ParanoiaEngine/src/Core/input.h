#pragma once

#include <SDL3/SDL.h>

class Input {
public:
	void processEvent(const SDL_Event& event) {
		if (event.type == SDL_EVENT_KEY_DOWN) {
			keys[event.key.scancode] = true;
		}
		if (event.type == SDL_EVENT_KEY_UP) {
			keys[event.key.scancode] = false;
		}

	}

	bool isHeld(SDL_Scancode key) const {
		if (key < 0 || key >= 512) return false;
		return keys[key];
	}

private:
	bool keys[512] = { false };
};