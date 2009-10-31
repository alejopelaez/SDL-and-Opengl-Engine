#include "Engine.h"

/* Contructor */
Engine::Engine(){
    initialize();
    initializeGL();

    //Start the timer
    timer.start();
    state.lastTicks = timer.getTicks();

    //Initializes the error log
    initErrorLog();

    //Initializes the engine state
    state.mouseFocus = true;
    state.keyboardFocus = true;
    state.running = true;
    state.focus = true;
    state.done = false;

    loadResources();
    mouse.setCustom(true);
}

/* Destructor */
Engine::~Engine(){
    //Delete the fonts
    delete[] fonts;

    SDL_FreeSurface(screen);
    
    //clean up the window
    SDL_Quit();
}

/* Initalizes some SDL and engine attributes */
bool Engine::initialize(){
    //This holds some info about our display
    const SDL_VideoInfo *videoInfo;

    //Initializes SDl
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        logError("Unable to initialize SDL: %s\n", SDL_GetError());
	return false;
    }

    //Get the video information
    videoInfo = SDL_GetVideoInfo();
    if(!videoInfo){
        logError("Unable to query video: %s\n", SDL_GetError());
	return false;
    }

    //The flags to pass to SDL_SetVideoMode
    videoFlags  = SDL_OPENGL;          //Enable OpenGL in SDL
    videoFlags |= SDL_GL_DOUBLEBUFFER; // Enable double buffering
    videoFlags |= SDL_HWPALETTE;       // Store the palette in hardware
    videoFlags |= SDL_RESIZABLE;       // Enable window resizing


    //This checks to see if surfaces can be stored in memory
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    //This checks if hardware blits can be done
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    //Sets the title
    SDL_WM_SetCaption("SDL Example",NULL);

    //Sets some SDL attributes to use with opengl
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    resizeSDL(DEFAULT_WIDTH,DEFAULT_HEIGHT);

    //Disable the key repeating event
    SDL_EnableKeyRepeat(0,0);

    //SDL has been initialized, so we ensure that if the program
    //terminates abruptly, SDL quits
    atexit(SDL_Quit);

    return true;
}

/* Initializes some opengl values */
bool Engine::initializeGL(){
    
    //Cornflower blue
    glClearColor(0.39062, 0.58203, 0.92578, 0.0);
    glClearDepth(1.0);

    //We enable face culling, counterclockwise to follow the standar
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Enable and set the Type Of Depth Testing (Less Or Equal)
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    //Enable and set the type of alpha testing
    glAlphaFunc(GL_GREATER,0.8);
    glEnable(GL_ALPHA_TEST);

    // Select Smooth Shading
    glShadeModel(GL_SMOOTH);
    // Set Perspective Calculations To Most Accurate
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //Enable Textures
    glEnable(GL_TEXTURE_2D);
    
    //Resizes the screen
    if(!resize(screen->w, screen->h))return false;

    return true;
}

/*
  Loads all the resources needed by the engine
 */
bool Engine::loadResources(){
    //Load the fonts
    fonts = new Font[1];
    for(int i = 0; i < 1; ++i)
    {
        fonts[i] = Font();
        if(!fonts[i].initialize("Resources/Fonts/font.png")){
            logError("Could't initialize the font %d",i);
            return false;
        }
    }

    text = loadTexture("Resources/Fonts/prueba ke no existe.png");

    if(!mouse.setTexture("Resources/Cursor/cursor.png")){
        
        return false;
    }

    return true;
}

/* Terminates the application */
bool Engine::terminate(){
    static SDL_Event quit;
    quit.type=SDL_QUIT;
    //if we cant quit we report the error
    if(SDL_PushEvent(&quit) == -1){
        logError("SDL_QUIT event couldn't be pushed: %s",SDL_GetError());
        exit(1);
    }
    
    return true; 
}

/* Resizes the SDL surface */
bool Engine::resizeSDL(int w, int h){
    //Initializes the screen
    screen = SDL_SetVideoMode(w, h, 32, videoFlags);

    //Checks if everything went ok
    if(!screen){
        logError("Video mode set failed %s",SDL_GetError());
        return false;
    }

    //Fill the screen with something
    SDL_FillRect(screen, NULL, SDL_MapRGBA(screen->format,0,0,0,0));

    return true;
}

/* Resizes the screen */
bool Engine::resize(int w, int h){

    if(h == 0)h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Calculates the aspect ratio and perspective
    gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,1.0f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
}

/*
  Handles all the mouse input
*/
void Engine::handleMouse(){
    if(mouse.isPressed(M_LEFT))
        printf("Left pressed\n");
    if(mouse.isPressed(M_MIDDLE))
        printf("Middle pressed\n");
    if(mouse.isPressed(M_RIGHT))
        printf("Right pressed\n");
    if(mouse.isPressed(M_WUP))
        printf("Wheel up pressed\n");
    if(mouse.isPressed(M_WDOWN))
        printf("Wheel down pressed\n");
}

/*
  Handles all the keybard input
*/
void Engine::handleKeyboard(){
    //Exit key
    if(keyboard.isPressed(SDLK_ESCAPE))
        terminate();

    
}

/* Updates the engine logic, input and state of the objects */
void Engine::update(){
    //Calculates how many ticks has passed since the last uptade call
    state.fps = 1000/((GLdouble)timer.elapsed());

    //And then handles the input
    handleKeyboard();
    handleMouse();
}

void Engine::draw2D(){
    //Calcuates the fps as a string
    char sfps[20];
    sprintf(sfps,"FPS: %.2lf",state.fps);

    //Resets the matrix so the new drawing area is equal in size to the windows
    //Also it follows the cartesian plane, so the origin (0,0)
    //is the lower left corner
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen->w, 0, screen->h,-1,1);
    glMatrixMode(GL_MODELVIEW);

    //Disable depth, in 2D wo don have any depth
    glDisable(GL_DEPTH_TEST);

    glColor3f(1.0f, 1.0f, 1.0f);
    //Draws some text
    fonts[0].draw(40,screen->h-40,sfps,1.75);
    fonts[0].draw(screen->w/2-100,screen->h-40,"Ejemplo de SDL", 1.75);

    //Draw the mouse
    mouse.draw(screen->h);

    //restores the depth test
    glEnable(GL_DEPTH_TEST);

    //Restores the matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/* Draw the 3D scene */
void Engine::draw3D(){

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,text);
    glBegin(GL_TRIANGLES);
    
    glColor3f(0.2f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.8f, -7.0f);
    glVertex3f(-2.0f, -1.0f, -7.0f);
    glVertex3f(2.0f, -1.0f, -7.0f);
    
    glEnd();
}

/* Calls the 3d and 2d drawing functions */
void Engine::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    draw3D();
    draw2D();
    
    SDL_GL_SwapBuffers();
}

/*
  Main loop, controlls all the run logic.
  Also manages all the SDL events.
*/
bool Engine::run(){
    //Events
    SDL_Event events[20];
    int numEvents = 0, i = 0;

    //SDL Event loop
    while(!state.done){
        //Check for events wif we are out events to process
        if(numEvents == 0)
        {
            //Updates the event queue
            SDL_PumpEvents();
            numEvents = SDL_PeepEvents(events, 20, SDL_GETEVENT,SDL_ALLEVENTS);
            i = 0;
            if(numEvents == 0 && !state.focus)SDL_Delay(10);
        }

        //If there are no events, we are probably idle
        if(numEvents > 0){
            switch(events[i].type){
        
                //WE lost focus
            case SDL_ACTIVEEVENT:
                if(events[i].active.gain)
                    state.focus = true;
                else
                    state.focus = false;
                break;

                //Keyboard input
            case SDL_KEYDOWN:
                keyboard.updatePress(events[i].key.keysym);
                break;

            case SDL_KEYUP:
                keyboard.updateRelease(events[i].key.keysym);
                break;

                //Mosue input
            case SDL_MOUSEMOTION:
                mouse.updatePos();
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouse.updatePress(events[i].button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                mouse.updateRelease(events[i].button.button);
                break;

            case SDL_VIDEORESIZE:
                if(!resizeSDL(events[i].resize.w,events[i].resize.h))state.done = true;
                if(!resize(events[i].resize.w,events[i].resize.h))state.done = true;
                break;

                //We are asked to redraw ourselves
            case SDL_VIDEOEXPOSE:
                draw();
                break;
        
                //Bye bye
            case SDL_QUIT:
                state.done = true;
                //Close the error log before exit
                closeErrorLog();
                break;
            }
            i++;
            numEvents--;
        }
        
        //Check if its time to update the game
        if(timer.getTicks() - state.lastTicks > 15){
            state.lastTicks = timer.getTicks();
            //Update the engine logic, and the redraw the scene
            if(state.focus && state.running)
                update();
            draw();
        }
    }
    return true;
}

