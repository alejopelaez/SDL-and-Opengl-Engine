#ifndef ENGINE_H
#define ENGINE_H

//Opengl and SDL headers
#include "Headers.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Utility/Timer.h"
#include "Utility/TextureLoader.h"
#include "Utility/Font.h"
#include "Errors/ErrorLog.h"

/* Struct that holds information about the app state*/
struct EngineState
{
    //Important application states
    bool mouseFocus, keyboardFocus,running,focus,done;
    //Current fps
    GLdouble fps;
    GLint lastTicks;
};

/*
  Main class that handles and controls the engine logic.
*/
class Engine{
private:
    //Canvas
    SDL_Surface* screen;
    //Flags to pass to SDL_SetVideoMode
    int videoFlags;
    //Timer
    Timer timer;
    //Input devices
    Keyboard keyboard;
    Mouse mouse;
    //Current State
    EngineState state;
    //Fonts
    Font *fonts;
    //Test texture
    GLuint text;
    
public:
    Engine();
    ~Engine();
    
    bool initialize();
    bool initializeGL();
    bool loadResources();
    bool terminate(void);
    bool resizeSDL(int w, int h);
    bool resize(int w, int h);
    void handleKeyboard();
    void handleMouse();
    void update();
    void draw3D();
    void draw2D();
    void draw();
    bool run(void);
};
#endif
