#pragma once

#include <SDL3/SDL.h>

class Time {
public:
	static void update() {
		float current_frame = SDL_GetTicks() / 1000.0f;
		delta_time = current_frame - last_frame;

		if (delta_time > 0.05f) {
			delta_time = 0.05f;
		}

		last_frame = current_frame;
	}

	static float getDeltaTime() {
		return delta_time;
	}

private:
	inline static float delta_time = 0.0f;
	inline static float last_frame = 0.0f;
};