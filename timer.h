#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
typedef struct
{
    float Lifetime;
} Timer;

static inline void StartTimer(Timer* timer, float lifetime)
{
    if (timer != NULL)
        timer->Lifetime = lifetime;
}

// Makes the timer dependant on the frametime
static inline void UpdateTimer(Timer* timer)
{
    if (timer != NULL && timer->Lifetime > 0)
        timer->Lifetime -= GetFrameTime();
}

// Checks if the timer is done
static inline bool TimerDone(Timer* timer)
{
    if (timer != NULL)
    {
        return timer->Lifetime <= 0;
    }
    return false;
}

#endif