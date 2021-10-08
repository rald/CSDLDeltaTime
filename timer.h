#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdbool.h>

typedef struct Timer Timer;

struct Timer {
	int startTicks;
	int pausedTicks;
	bool paused;
	bool started;
};

Timer *Timer_New();
void Timer_Start(Timer *timer);
void Timer_Stop(Timer *timer);
void Timer_Pause(Timer *timer);
void Timer_Unpause(Timer *timer);
int Timer_GetTicks(Timer *timer);

#endif
