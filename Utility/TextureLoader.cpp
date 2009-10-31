#include "TextureLoader.h"

int loadTexture(const char *filename){
    SDL_Surface *image = IMG_Load(filename);
    GLuint texture;

    //If we could't load the specified image we try to load the default one
    if(!image){
        logError("Error loading texture %s", SDL_GetError());
    
        image = IMG_Load("Resources/noimage.png");
        //If we could't even load the default image there is something wrong
        if(!image){
            logError("Unable to load the DEFAULT image %s", filename, SDL_GetError());
            exit(1);
        }
    }

    //Checks the image format
    GLenum textureFormat;
    GLint nOfColors = image->format->BytesPerPixel;
    if (nOfColors == 4)              //contains an alpha channel
    {
        if (image->format->Rmask == 0x000000ff)
            textureFormat = GL_RGBA;
        else
            textureFormat = GL_BGRA;
    }else if (nOfColors == 3){       //no alpha channel
        if (image->format->Rmask == 0x000000ff)
            textureFormat = GL_RGB;
        else
            textureFormat = GL_BGR;
    }else{
        logError("Image is not true color, it may have a format error");
        exit(1);
    }

    // Create The Texture
    glGenTextures(1, &texture);
    
    //Binds the texture 
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Nearest Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate The Texture
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, image->w, image->h, 0, 
                 textureFormat, GL_UNSIGNED_BYTE, image->pixels);

    //Free up any memory we may have used
    if (image)
	SDL_FreeSurface(image);

    return texture;
}
