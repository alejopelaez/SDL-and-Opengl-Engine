#ifndef MY_TIMER
#define MY_TIMER
#include "../Headers.h"

/*
  Timer class, keep track of the time elapsed since it was started
  can be paused and unpaused at will.
 */
class Timer{
 private:
    GLuint startTicks, pausedTicks, lastElapsed;
    bool paused, started;

 public:
    Timer();
    void start();
    void stop();
    void pause();
    void unpause();
    
    GLuint getTicks();
    GLuint elapsed();

    bool isPaused();
    bool isStarted();
};

#endif
