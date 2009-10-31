#ifndef MY_FONT
#define MY_FONT
#include "../Headers.h"
#include "TextureLoader.h"
#include "../Errors/ErrorLog.h"

class Font{
 private:
    GLuint font,list;
    bool buildFont();

 public:
    Font(void);
    ~Font(void);

    bool initialize(const char *filename);
    void draw(GLint x, GLint y,const char* text, GLfloat size=1.0f);
};

#endif
