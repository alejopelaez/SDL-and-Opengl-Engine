#ifndef MY_KEYBOARD
#define MY_KEYBOARD

#include "../Headers.h"

class Keyboard{
 private:
    int numKeys;
    Uint8 *press, *hold;

 public:
    Keyboard(void);
    void updatePress(SDL_keysym key);
    void updateRelease(SDL_keysym key);
    bool isPressed(SDLKey key);
    bool isHold(SDLKey key);
};

#endif
