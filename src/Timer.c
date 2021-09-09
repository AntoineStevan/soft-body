#include "Timer.h"

Timer Timer_init(){
	Timer timer = {0, 0, 0, 0};
	return timer;
}

char Timer_is_started(Timer timer){
	return timer.started;
}

void Timer_start(Timer* timer){
	// the timer starts and is unpaused.
	timer->started = 1;
	timer->paused = 0;

	// the starting ticks are simply given by the GetTicks function from the SDL library.
	timer->start_ticks = SDL_GetTicks();
	timer->paused_ticks = 0;
}

void Timer_stop(Timer* timer){
	timer->started = 0;
	timer->paused = 0;

	timer->start_ticks = 0;
	timer->paused_ticks = 0;
}

char Timer_is_paused(Timer timer){
	return timer.paused;
}

void Timer_pause(Timer* timer){
	if (timer->started && !timer->paused){
		timer->paused = 1;

		timer->paused_ticks = SDL_GetTicks()-timer->start_ticks;
		timer->start_ticks = 0;
	}
}

void Timer_unpause(Timer* timer){
	if (timer->started && timer->paused){
		timer->paused = 0;

		timer->start_ticks = SDL_GetTicks()-timer->paused_ticks;
		timer->paused_ticks = 0;
	}
}

Uint32 Timer_get_ticks(Timer timer){
	Uint32 time = 0;
	if (timer.started){
		if (timer.paused){
			time = timer.paused_ticks;
		} else {
			time = SDL_GetTicks() - timer.start_ticks;
		}
	}
	return time;
}
