//
//  texture.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/23/15.
//
//

#include <iostream>
#include <cassert>

#include "lodepng.h"
#include "texture.h"

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

Image *TextureImage = NULL;

typedef struct RGB {
   GLubyte r;
   GLubyte g;
   GLubyte b;
} RGB;

RGB myimage[64][64];
RGB* g_pixel;

int ImageLoad(std::string filename, Image *image);

GLvoid texture_loadToArray(std::string filename, int texture, int layer, int *width, int *height) {
   Image *TextureImage = (Image *) malloc(sizeof(Image));
   if (TextureImage == NULL) {
      printf("Error allocating space for image");
      exit(1);
   }
   std::cout << "trying to load " << filename << std::endl;
   if (!ImageLoad(filename, TextureImage)) {
      exit(1);
   }
   
   // Check sizes
   *width = TextureImage->sizeX;
   *height = TextureImage->sizeY;
   assert(*width <= MAX_TEXTURE_SIZE);
   assert(*height <= MAX_TEXTURE_SIZE);
   
   glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
   GLenum error = glGetError();
   assert(error == 0);
   //Upload pixel data.
   //The first 0 refers to the mipmap level (level 0, since there's only 1)
   //The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
   //The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
   //Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
   glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, *width, *height, 1, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage->data);
    error = glGetError();
   assert(error == 0);
   
   //Always set reasonable texture parameters
   glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
   
    error = glGetError();
   assert(error == 0);
}

// ------------------- TEXTURE LOADING ------------------------
//routines to load in a bmp files - must be 2^nx2^m and a 24bit bmp
GLvoid LoadTexture(std::string image_file, int texID) {
   GLenum err;
   const char *imagepath = "textures/sky.bmp";
   printf("Reading image %s\n", imagepath);
   
   // Data read from the header of the BMP file
   unsigned char header[54];
   unsigned int dataPos;
   unsigned int imageSize;
   unsigned int width, height;
   // Actual RGB data
   unsigned char * data;
   
   // Open the file
   FILE * file = fopen(imagepath,"rb");
   if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}
   
   // Read the header, i.e. the 54 first bytes
   
   // If less than 54 bytes are read, problem
   if ( fread(header, 1, 54, file)!=54 ){
      printf("Not a correct BMP file\n");
      return 0;
   }
   // A BMP files always begins with "BM"
   if ( header[0]!='B' || header[1]!='M' ){
      printf("Not a correct BMP file\n");
      return 0;
   }
   // Make sure this is a 24bpp file
   if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
   if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}
   
   // Read the information about the image
   dataPos    = *(int*)&(header[0x0A]);
   imageSize  = *(int*)&(header[0x22]);
   width      = *(int*)&(header[0x12]);
   height     = *(int*)&(header[0x16]);
   
   // Some BMP files are misformatted, guess missing information
   if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
   if (dataPos==0)      dataPos=54; // The BMP header is done that way
   
   // Create a buffer
   data = new unsigned char [imageSize];
   
   // Read the actual data from the file into the buffer
   fread(data,1,imageSize,file);
   
   // Everything is in memory now, the file wan be closed
   fclose (file);
   
   // Create one OpenGL texture
   GLuint textureID = texID;
   glGenTextures(1, &textureID);
   
   // "Bind" the newly created texture : all future texture functions will modify this texture
   glBindTexture(GL_TEXTURE_2D, textureID);
   
   // Give the image to OpenGL
   glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
   
   // OpenGL has now copied the data. Free our own version
   delete [] data;
   
   // Poor filtering, or ...
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   
   // ... nice trilinear filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
   GLenum error = glGetError();
   assert(error == 0);
   
   return;
   
   TextureImage = (Image *) malloc(sizeof(Image));
   if (TextureImage == NULL) {
      printf("Error allocating space for image");
      exit(1);
   }
   std::cout << "trying to load " << image_file << std::endl;
   if (!ImageLoad(image_file, TextureImage)) {
      exit(1);
   }
   /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
   /*  x size from image, y size from image,                                              */
   /*  border 0 (normal), rgb color data, unsigned byte data, data  */
   glBindTexture(GL_TEXTURE_2D, texID);
   err = glGetError();
   assert(err == 0);
   glTexImage2D(GL_TEXTURE_2D, 0, 3,
                TextureImage->sizeX, TextureImage->sizeY,
                0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    err = glGetError();
   assert(err == 0);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); /*  cheap scaling when image bigger than texture */
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); /*  cheap scaling when image smalled than texture*/
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
      return loadPNG("textures/sky.png", image);
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