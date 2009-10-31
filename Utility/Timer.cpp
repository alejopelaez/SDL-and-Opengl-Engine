#include "Timer.h"
 
/*Constructor*/
Timer::Timer(){
    startTicks=pausedTicks=0;
    started=paused=false;
}

void Timer::start(){
    started = true;
    paused = false;

    //Gets the current clock time
    startTicks = SDL_GetTicks();
    lastElapsed = SDL_GetTicks();
}

void Timer::stop(){
    started = false;
    paused = false;
}
void Timer::pause(){
    //If the timer is running and isn't already paused 
    if(started == true && paused == false){ 
        //Pause the timer 
        paused = true; 
        //Calculate the paused ticks 
        pausedTicks = SDL_GetTicks() - startTicks; 
    
    } 
}
void Timer::unpause(){
    //If the timer is paused
    if(paused == true){
        //Unpause the timer
        paused = false;
        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;
    }
}

GLuint Timer::getTicks(){
    
    //If the timer is running
    if(started){
        //If the timer is not paused
        if(paused){
            return pausedTicks;
        }
        else{  
            //Return current ticks minus starting ticks
            return SDL_GetTicks()-startTicks;
        }  
    }
    
    //If the timer is not running
    return 0;
}

/*
  Return the elapsed time in milliseconds since the last call to this function
  Return 0 if either the timer is paused or not started.
*/
GLuint Timer::elapsed(){
    GLuint elaps = 0;

    if(started && !paused)
        elaps = SDL_GetTicks()-lastElapsed;

    lastElapsed = SDL_GetTicks();
    return elaps;
}

bool Timer::isPaused(){
    return paused;
}

bool Timer::isStarted(){
    return started;
}
