//  texture.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/23/15.
//
//

#include <iostream>
#include <unordered_map>
#include <cassert>

#include "lodepng.h"
#include "texture.h"
#include "main.h"

// Uniform and attribute locations
#define A_POSITION 0
#define A_COLOR 1
#define A_NORMAL 2

// Texture stuff
typedef struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
} Image;

typedef struct RGB {
    GLubyte r;
    GLubyte g;
    GLubyte b;
} RGB;

RGB myimage[64][64];
RGB* g_pixel;

std::unordered_map<std::string, Image *> textures;
int repeats = 0;

int ImageLoad(std::string filename, Image *image);
int loadPNG(std::string filename, Image *image);

GLvoid texture_loadToArray(std::string filename, int texture, int layer, int *width, int *height) {
    Image *img = textures[filename];
    if (!img) {
        img = textures[filename] = (Image *) malloc(sizeof(Image));
        if (img == NULL) {
            printf("Error allocating space for image");
            exit(1);
        }
        //      std::cout << "trying to load " << filename << std::endl;
        if (!ImageLoad(filename, img)) {
            exit(1);
        }
    }
    
    // Check sizes
    *width = img->sizeX;
    *height = img->sizeY;
    assert(*width <= MAX_TEXTURE_SIZE);
    assert(*height <= MAX_TEXTURE_SIZE);
    
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, *width, *height, 1, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    
    //Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    
    GLenum error = glGetError();
    assert(error == 0);
}

// ------------------- TEXTURE LOADING ------------------------
//routines to load in a bmp files - must be 2^nx2^m and a 24bit bmp
GLvoid texture_load(std::string filename, int texture) {
    Image *img = textures[filename];
    if (!img) {
        img = textures[filename] = (Image *) malloc(sizeof(Image));
        if (img == NULL) {
            printf("Error allocating space for image");
            exit(1);
        }
        
        if (!ImageLoad(filename, img)) {
            exit(1);
        }
    }
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    
    //Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);//LINEAR);
    
    GLenum error = glGetError();
    assert(error == 0);
}


/* BMP file loader loads a 24-bit bmp file only */
/*
 * getint and getshort are help functions to load the bitmap byte by byte
 */
static unsigned int getint(FILE *fp) {
    int c, c1, c2, c3;
    
    /*  get 4 bytes */
    c = getc(fp);
    c1 = getc(fp);
    c2 = getc(fp);
    c3 = getc(fp);
    
    return ((unsigned int) c) +
    (((unsigned int) c1) << 8) +
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp){
    int c, c1;
    
    /* get 2 bytes*/
    c = getc(fp);
    c1 = getc(fp);
    
    return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

/*  quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  */
int loadBMP(std::string filename, Image *image) {
    FILE *file;
    unsigned long size;                 /*  size of the image in bytes. */
    unsigned long i;                    /*  standard counter. */
    unsigned short int planes;          /*  number of planes in image (must be 1)  */
    unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
    char temp;                          /*  used to convert bgr to rgb color. */
    
    /*  make sure the file is there. */
    std::cout << "loading " << filename << std::endl;
    if ((file = fopen(filename.c_str(), "rb"))==NULL) {
        std::cerr << "File not found: " << filename << std::endl;
        return ImageLoad("textures/sky.bmp", image);
    }
    
    /*  seek through the bmp header, up to the width height: */
    fseek(file, 18, SEEK_CUR);
    
    /*  No 100% errorchecking anymore!!! */
    
    /*  read the width */    image->sizeX = getint (file);
    
    /*  read the height */
    image->sizeY = getint (file);
    
    /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
    size = image->sizeX * image->sizeY * 3;
    
    /*  read the planes */
    planes = getshort(file);
    if (planes != 1) {
        std::cerr << "Planes from " << filename << " is not 1: " << planes << std::endl;
        return 0;
    }
    
    /*  read the bpp */
    bpp = getshort(file);
    if (bpp != 24) {
        std::cerr << "BPP from " << filename << " is not 24: " << bpp << std::endl;
        return 0;
    }
    
    /*  seek past the rest of the bitmap header. */
    fseek(file, 24 + 66, SEEK_CUR);
    
    /*  read the data.  */
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    
    if ((i = fread(image->data, size, 1, file)) != 1) {
        std::cerr << "Error reading image data from " << filename << std::endl;;
        return 0;
    }
    
    for (i=0;i<size;i+=3) { /*  reverse all of the colors. (bgr -> rgb) */
        temp = image->data[i];
        image->data[i] = image->data[i+1];
        image->data[i+1] = temp;
    }
    
    fclose(file); /* Close the file and release the filedes */
    
    /*  we're done. */
    return 1;
}

int loadPNG(std::string filename, Image *image) {
    std::vector<unsigned char> img; //the raw pixels
    unsigned width, height;
    
    //decode
    unsigned error = lodepng::decode(img, width, height, filename);
    
    //if there's an error, display it
    if (error == 48) {
        image->sizeX = image->sizeY = 1;
        image->data = (char *)calloc(1, sizeof(char) * 4);
        image->data[3] = 255;
        return 1;
    }
    else {
        if (error)
            std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        
        image->data = (char *) malloc(sizeof(char) * img.size());
        
        image->sizeX = width;
        image->sizeY = height;
        for(int i = 0; i < img.size(); i ++)
            image->data[i] = img[i];
        
        return error == 0;
    }
}

int ImageLoad(std::string filename, Image *image) {
    if (filename.find(".png"))
        return loadPNG(filename, image);
    else
        return loadBMP(filename, image);
}


RandomTexture::RandomTexture() {
    m_textureObj = 0;
}

RandomTexture::~RandomTexture()
{
    if (m_textureObj != 0) {
        glDeleteTextures(1, &m_textureObj);
    }
}
bool RandomTexture::InitRandomTexture(unsigned int Size)
{
    glm::vec3 *pRandomData = new glm::vec3[Size];
    for (unsigned int i = 0 ; i < Size ; i++) {
        pRandomData[i].x = randFloat(-1, 1);
        pRandomData[i].y = randFloat(-1, 1);
        pRandomData[i].z = randFloat(-1, 1);
    }
    
    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_1D, m_textureObj);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, Size, 0, GL_RGB, GL_FLOAT, pRandomData);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    delete [] pRandomData;
    
    return glGetError() == GL_NO_ERROR;
}
void RandomTexture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_1D, m_textureObj);
}
Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
}
bool Texture::Load()
{
    Image *img = textures[m_fileName];
    if (!img) {
        img = textures[m_fileName] = (Image *) malloc(sizeof(Image));
        if (img == NULL) {
            printf("Error allocating space for image");
            exit(1);
        }
        std::cout << "trying to load " << m_fileName << std::endl;
        if (!ImageLoad(m_fileName, img)) {
            exit(1);
        }
    }
    
    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(m_textureTarget, 0);
    
    return true;
}
void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}