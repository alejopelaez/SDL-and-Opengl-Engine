#ifndef MY_MOUSE
#define MY_MOUSE
#include "../Headers.h"
#include "../Utility/TextureLoader.h"

class Mouse{
 private:
    bool press[6],hold[6];
    int x,y,lastx,lasty;
    bool visible;
    
    //This is used if we are going to have a custom mouse texture
    bool custom;
    GLuint text, list;
 public:
    Mouse();
    Mouse(const char *filename);
    ~Mouse();
    
    //Custom mouse
    bool setTexture(const char *filename);
    void setCustom(bool custom);
    void buildList();

    //Visiblity
    void setVisible(bool visible);
    bool isVisible();
    //Position
    void updatePos(void);
    void getMovement(int &x, int &y);
    void getPos(int &x, int &y);
    //Input
    void updatePress(Uint8 button);
    void updateRelease(Uint8 button);
    bool isPressed(Uint8 button);
    bool isHold(Uint8 button);
    
    //Drawing
    void draw(GLint h);
};

#endif
