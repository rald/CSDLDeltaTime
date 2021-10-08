#include "timer.h"

Timer *Timer_New() {
	Timer *timer=malloc(sizeof(*timer));

	if(timer) {
		timer->startTicks=0;
		timer->pausedTicks=0;
		timer->started=false;
		timer->paused=false;
	}

	return timer;
}

void Timer_Start(Timer *timer) {
  timer->started = true;
  timer->paused = false;
  timer->startTicks = SDL_GetTicks();
}

void Timer_Stop(Timer *timer) {
  timer->started = false;
  timer->paused = false;
}

void Timer_Pause(Timer *timer) {
	if(timer->started && !timer->paused ) {
	  timer->paused = true;
	  timer->pausedTicks = SDL_GetTicks() - timer->startTicks;
	}
}

void Timer_Unpause(Timer *timer) {
	if(timer->paused) {
	  timer->paused = false;
	  timer->startTicks = SDL_GetTicks() - timer->pausedTicks;
	  timer->pausedTicks = 0;
	}
}

int Timer_GetTicks(Timer *timer) {
	if(timer->started) {
		if(timer->paused) {
			return timer->pausedTicks;
		} else {
			return SDL_GetTicks() - timer->startTicks;
		}
	}
	return 0;
}
