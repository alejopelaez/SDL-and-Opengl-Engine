#include "Keyboard.h"

/* Constructor */
Keyboard::Keyboard(){
    press = SDL_GetKeyState(NULL);
    hold = SDL_GetKeyState(&numKeys);
}

/* Updates the keyboard state
 */
void Keyboard::updatePress(SDL_keysym key){
    press[key.sym] = true;
    hold[key.sym] = true;
}

/* Updates the keyboard state
 */
void Keyboard::updateRelease(SDL_keysym key){
    press[key.sym] = false;
    hold[key.sym] = false;
}

/*
  Checks if a keys has been pressed
  but is not being hold
 */
bool Keyboard::isPressed(SDLKey key){
    bool ans = press[key];
    press[key] = false;
    return ans;
}

/*
  Check if a key is being held
 */
bool Keyboard::isHold(SDLKey key){
    return hold[key];
}



