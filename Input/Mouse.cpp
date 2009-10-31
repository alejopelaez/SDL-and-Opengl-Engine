#include "Mouse.h"

/* Contructor */
Mouse::Mouse(){
    text = 0; list = 0;
    custom = false;
    visible = true;
    SDL_GetMouseState(&x,&y);
    lastx = x; lasty = y;

    memset(press,false,6);
    memset(hold,false,6);
}

Mouse::Mouse(const char* filename){
    text = 0; list = 0;
    custom = true;
    visible = true;

    //Load the custom texture
    setTexture(filename);
}

/* Destructor */
Mouse::~Mouse(){
    if(text)
        glDeleteTextures(1,&text);
    if(list)
        glDeleteLists(list,1);
}

/*
  Sets a custom texture
*/
bool Mouse::setTexture(const char *filename){
    if(text)
        glDeleteTextures(1,&text);
    text = loadTexture(filename);

    if(!text){
        logError("Couldn't load the texture for the cursor");
        return false;
    }

    if(!list)
        buildList();

    if(!list){
        logError("Couldn't create the display list for the cursor");
        return false;
    }

    return true;
}

void Mouse::buildList(){
    list = glGenLists(1);
    
    glNewList(list,GL_COMPILE);

    glBegin(GL_QUADS);
    //Top left
    glTexCoord2i(0,0);
    glVertex2i(0, 0);

    //Bottom left
    glTexCoord2i(0,1);
    glVertex2i(0, -25);

    //Bottom right
    glTexCoord2i(1,1);
    glVertex2i(25, -25);

    //Top Right
    glTexCoord2i(1,0);
    glVertex2i(25, 0);
    glEnd();

    glEndList();
}

void Mouse::setCustom(bool cust){
    custom = cust;
    if(cust)
        SDL_ShowCursor(SDL_DISABLE);
    else
        SDL_ShowCursor(SDL_ENABLE);
}

/*
  Sets the mouse to visible or not
  This works differently dependig wether we are
  using a custom texture or not
*/
void Mouse::setVisible(bool vis){
    if(!custom){
        if(visible && !vis)
            SDL_ShowCursor(SDL_DISABLE);
        else if(!visible && vis)
            SDL_ShowCursor(SDL_ENABLE);
    }

    visible = vis;
}

bool Mouse::isVisible(){
    return visible;
}

/*
  Updates the mouse position
*/
void Mouse::updatePos(){
    lastx = x; lasty = y;
    SDL_GetMouseState(&x,&y);
}

/*
  Sets in the parameters the current relative movement in x and y
*/
void Mouse::getMovement(int &retx, int &rety){
    retx = x - lastx;
    rety = y - lasty;
}
/*
  Sets in the parameters the current position of the mouse
*/
void Mouse::getPos(int &retx, int &rety){
    retx = x;
    rety = y;
}

/*
  Updates the mouse state
*/
void Mouse::updatePress(Uint8 button){
    press[button]=true;
    if(button < 4)
        hold[button]=true;
}

/*
  Updates the mouse state
*/
void Mouse::updateRelease(Uint8 button){
    if(button < 4){
        press[button]=false;
        hold[button]=false;
    }
}

/*
  Checks if a button is pressed but not being hold
*/
bool Mouse::isPressed(Uint8 button){
    bool ans = press[button];
    press[button] = false;
    return ans;
}

/*
  Checks if a button is being hold
*/
bool Mouse::isHold(Uint8 button){
    return hold[button];
}

/*
  If we have a custom texture, we draw the mouse
  at the specified position
*/
void Mouse::draw(GLint h){
    //Just draw if we are using custom textures
    if(custom){
        //Saves the last state
        glPushAttrib(GL_ENABLE_BIT);
        glPushAttrib(GL_COLOR_BUFFER_BIT);

        //Saves the matrix
        glPushMatrix();
        glLoadIdentity();

        //Enable some states
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.1);
        glEnable(GL_TEXTURE_2D);
    
        glTranslatef(x,h-y,0);

        //Select our texture
        glBindTexture(GL_TEXTURE_2D, text);
        //Call the list
        glCallList(list);

        //We are done
        glPopMatrix();
        glPopAttrib();
        glPopAttrib();
    }
}
