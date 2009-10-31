#include "Font.h"

/* Constructors */
Font::Font(){
    font = 0;
    list = 0;
}

/* Destructor, free the display list and textures */
Font::~Font(){
    glDeleteLists(list,256);
    glDeleteTextures(1,&font);
}

bool Font::initialize(const char *filename){
    font = loadTexture(filename);

    if(font == 0)
    {
        logError("Could't Load Font");
        return false;
    }

    buildFont();
    if(list == 0){
        logError("Could't load the display list for the font %s %s",filename,SDL_GetError());
        return false;
    }

    return true;
}

/* Build a display list for each of the 256 ascii characters.
   This is done to speed up the drawing process as each character
   can be drawn a lot of times.
*/
bool Font::buildFont(){
    list = 0;
    list = glGenLists(256);

    //Loop through the 256 ascii characters
    //We go from the lower left corner to the upper right corner
    GLfloat tx, ty;
    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 16; ++j){

            //Current texture position
            tx = (GLfloat)(j+1)/16.0f;
            ty = (GLfloat)(i+1)/16.0f; 

            glNewList(list + 16*i + j, GL_COMPILE);
            
            glBegin(GL_QUADS);
            //Bottom left
            glTexCoord2f(tx-0.0625,ty);
            glVertex2i(0,0);

            //Bottom right
            glTexCoord2f(tx,ty);
            glVertex2i(16,0);

            //Top right
            glTexCoord2f(tx,ty-0.0625);
            glVertex2i(16,16);

            //Top Left
            glTexCoord2f(tx-0.0625,ty-0.0625);
            glVertex2i(0,16);

            glEnd();

            //Move the character a little bit to the left
            glTranslated(10, 0, 0);

            glEndList();
        }
    }

    return true;
}
void Font::draw(GLint x, GLint y, const char *text, GLfloat size){
    //Saves the last state
    glPushAttrib(GL_ENABLE_BIT);
    glPushAttrib(GL_COLOR_BUFFER_BIT);

    //Enable some states
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_TEXTURE_2D);
    
    //Select our texture
    glBindTexture(GL_TEXTURE_2D, font);

    //Resets the matrix
    glPushMatrix();
    glLoadIdentity();

    //Draw the text
    glTranslated(x, y, 0);
    glScalef(size,size,1.0f);
    glListBase(list);
    glCallLists(strlen(text),GL_BYTE,text);

    //Restores the matrix
    glPopMatrix();

    //Loads the state
    glPopAttrib();
    glPopAttrib();
}

