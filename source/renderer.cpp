#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <glm/ext.hpp>
using namespace std;

#include "camera.h"
#include "bounds.h"
#include "shader.hpp"
#include "renderer.h"
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

Image *TextureImage;

typedef struct RGB {
    GLubyte r;
    GLubyte g;
    GLubyte b;
} RGB;

RGB myimage[64][64];
RGB* g_pixel;

//forward declaration of image loading and texture set-up code
int ImageLoad(char *filename, Image *image);
GLvoid LoadTexture(char* image_file, int tex_id);

// Shader loader used by programs
GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath);

Program *Program3D = NULL;
GLuint Program3D_uWinScale, Program3D_uProj, Program3D_uModel, Program3D_uView;
GLuint Program3D_uLightPos, Program3D_uAColor, Program3D_uDColor;
GLuint Program3D_uSColor, Program3D_uShine, Program3D_uBend;
GLuint Program3D_aPosition, Program3D_aNormal;
Renderer *Program3Dcreate();
void Program3DbufferData(Renderer *p, int type, long num, void *data);
void Program3Drender(Renderer *p, glm::mat4 Model);

Program *ProgramText = NULL;
GLuint ProgramText_uTexUnit, ProgramText_aPosition, ProgramText_aTexCoord;
Renderer *ProgramTextcreate();
void ProgramTextbufferData(Renderer *p, int type, long num, void *data);
void ProgramTextrender(Renderer *p, glm::mat4 Model);

Program *ProgramPostProc = NULL;
GLuint ProgramPostProc_v_coord, ProgramPostProc_fbo_texture;
Renderer *ProgramPostProccreate();
void ProgramPostProcbufferData(Renderer *p, int type, long num, void *data);
GLuint fbo, fbo_texture, rbo_depth;


GLuint *createBuffers(int num) {
    GLuint *buffers = (GLuint *) malloc(sizeof(GLuint) * num);
    
    glGenBuffers(num, buffers);
    
    return buffers;
}

// Program constructor/destructor
Renderer::Renderer(int numBuffers) : numBuffers(numBuffers), buffers(createBuffers(numBuffers)), elements(0) {}
Renderer::~Renderer() {
    glDeleteBuffers(numBuffers, buffers);
}

glm::mat4 Projection;
glm::mat4 currentMVP;
std::stack<glm::mat4> MatrixStack;

void Renderer::pushMatrix(glm::mat4 matrix) {
    MatrixStack.push(currentMVP);
    
    currentMVP *= matrix;
}

void Renderer::popMatrix() {
    currentMVP = MatrixStack.top();
    
    MatrixStack.pop();
}

void setMaterial(Material mat, GLuint uDColor, GLuint uSColor, GLuint uAColor, GLuint uShine) {
    switch(mat) {
        case MATERIAL_METAL:
            glUniform3f(Program3D_uAColor, 0.15, 0.15, 0.15);
            glUniform3f(Program3D_uDColor, 0.4, 0.4, 0.4);
            glUniform3f(Program3D_uSColor, 0.14, 0.14, 0.14);
            glUniform1f(Program3D_uShine, 76.8);
            break;
        case MATERIAL_RED_METAL:
            glUniform3f(Program3D_uAColor, 0.05, 0.05, 0.05);
            glUniform3f(Program3D_uDColor, 0.4, 0.05, 0.05);
            glUniform3f(Program3D_uSColor, 0.4, 0.05, 0.05);
            glUniform1f(Program3D_uShine, 25.0);
            break;
        case MATERIAL_RUBBER:
            glUniform3f(Program3D_uAColor, 0, 0, 0);//2, 0.02, 0.02);
            glUniform3f(Program3D_uDColor, 0, 0, 0);//.01, 0.01, 0.01);
            glUniform3f(Program3D_uSColor, 0.1, 0.1, 0.1);
            glUniform1f(Program3D_uShine, -100.0);
            break;
        case MATERIAL_CHAIR:
            glUniform3f(Program3D_uAColor, 0.02, 0.02, 0.01);
            glUniform3f(Program3D_uDColor, 0.2, 0.2, 0.2);
            glUniform3f(Program3D_uSColor, 0.1, 0.1, 0.1);
            glUniform1f(Program3D_uShine, 10.0);
            break;
        case MATERIAL_GRASS:
            glUniform3f(Program3D_uAColor, 0.15, 0.4, 0.15);
            glUniform3f(Program3D_uDColor, 0.4, 0.7, 0.4);
            glUniform3f(Program3D_uSColor, 0, 0, 0);
            glUniform1f(Program3D_uShine, -100.0);
            break;
       case MATERIAL_GREEN:
          glUniform3f(Program3D_uAColor, 0.316,0.925, 0.148);
          glUniform3f(Program3D_uDColor, 0, 1.0, 0);
          glUniform3f(Program3D_uSColor, 0.2,0.9,0.2);
          glUniform1f(Program3D_uShine, 60);
          break;
       case MATERIAL_RED: // Lane 2 (Red)
          glUniform3f(Program3D_uAColor, 0.925, 0.148, 0.316);
          glUniform3f(Program3D_uDColor, 1.0, 0, 0);
          glUniform3f(Program3D_uSColor, 0.9,0.2,0.2);
          glUniform1f(Program3D_uShine, 60);
          break;
       case MATERIAL_YELLOW: // Lane 3 (Yellow)
          glUniform3f(Program3D_uAColor, 0.925, 0.925, 0.02);
          glUniform3f(Program3D_uDColor, 0.9, 0.9, 0);
          glUniform3f(Program3D_uSColor, 0.8,0.8,0.2);
          glUniform1f(Program3D_uShine, 60);
          break;
       case MATERIAL_BLUE: // Lane 3 (Blue)
          glUniform3f(Program3D_uAColor, 0.148, 0.316, 0.925);
          glUniform3f(Program3D_uDColor, 0.2, 0.2, 0.8);
          glUniform3f(Program3D_uSColor, 0.2,0.2,0.9);
          glUniform1f(Program3D_uShine, 60);
          break;
        default:
            std::cerr << "Error: Material " << mat << " not found" << std::endl;
    }
}

void setUniforms(GLuint uWinScale, GLuint uPerspective, GLuint uView, GLuint uModel, glm::mat4 MVP) {
    if(w_width > w_height)
        glUniform2f(uWinScale, (float) w_height / w_width, 1);
    else
        glUniform2f(uWinScale, 1, (float) w_width / w_height);
    
    // Send camera projection
    MVP = currentMVP * MVP;
    
    glm::mat4 View = camera_getMatrix();
    
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(uView, 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(uPerspective, 1, GL_FALSE, &Projection[0][0]);
}

void shaders_init() {
   GLuint VertexArrayID;
   glGenVertexArrays(1, &VertexArrayID);
   glBindVertexArray(VertexArrayID);
   
    Projection = glm::perspective(45.0f, (float) w_width / w_height, 0.01f, 400.0f);
    currentMVP = glm::mat4(1.0f);
    MatrixStack.empty();
   
//    glMatrixMode(GL_PROJECTION);
//    glLoadMatrixf(&Projection[0][0]);
//   
//   error = glGetError();
//   std::cout << error << std::endl;
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
   
   // ----------------- 3D MODEL SHADER -------------------------
   Program3D = (Program *) malloc(sizeof(Program));
   
   Program3D->programID = LoadShaders("./shaders/3DVertex.glsl", "", "./shaders/3DFragment.glsl");
   Program3D_uWinScale = glGetUniformLocation(Program3D->programID, "windowScale");
   Program3D_uProj = glGetUniformLocation(Program3D->programID, "uProjMatrix");
   Program3D_uModel = glGetUniformLocation(Program3D->programID, "uModelMatrix");
   Program3D_uView = glGetUniformLocation(Program3D->programID, "uViewMatrix");
   Program3D_uAColor = glGetUniformLocation(Program3D->programID, "UaColor");
   Program3D_uDColor = glGetUniformLocation(Program3D->programID, "UdColor");
   Program3D_uSColor = glGetUniformLocation(Program3D->programID, "UsColor");
   Program3D_uLightPos = glGetUniformLocation(Program3D->programID, "uLightPos");
   Program3D_uShine = glGetUniformLocation(Program3D->programID, "uShine");
   Program3D_uBend = glGetUniformLocation(Program3D->programID, "uBend");
   Program3D_aNormal = glGetAttribLocation(Program3D->programID, "aNormal");
   Program3D_aPosition = glGetAttribLocation(Program3D->programID, "aPosition");

   Program3D->create = &Program3Dcreate;
   Program3D->bufferData = &Program3DbufferData;
   Program3D->render = &Program3Drender;

    // ---------------- TEXT SHADER ---------------------------
    ProgramText = (Program *) malloc(sizeof(Program));

    ProgramText->programID = LoadShaders("./shaders/TextVertex.glsl", "", "./shaders/TextFragment.glsl");
    ProgramText_aPosition = glGetAttribLocation(ProgramText->programID, "aPosition");
    ProgramText_aTexCoord = glGetAttribLocation(ProgramText->programID, "aUV");
    ProgramText_uTexUnit = glGetAttribLocation(ProgramText->programID, "uTexUnit");
    
    ProgramText->create = &ProgramTextcreate;
    ProgramText->bufferData = &ProgramTextbufferData;
    ProgramText->render = &ProgramTextrender;

	// ---------------- POST-PROCESSOR SHADER ---------------------------

	ProgramPostProc = (Program *)malloc(sizeof(Program));

	ProgramPostProc->programID = LoadShaders("./shaders/PostProcVertex.glsl", "", "./shaders/PostProcFragment.glsl");
	ProgramPostProc_v_coord = glGetAttribLocation(ProgramPostProc->programID, "v_coord");
	ProgramPostProc_fbo_texture = glGetUniformLocation(ProgramPostProc->programID, "fbo_texture");

	ProgramPostProc->create = &ProgramPostProccreate;
	ProgramPostProc->bufferData = &ProgramPostProcbufferData;
	ProgramPostProc->render = &ProgramPostProcrender;
	ProgramPostProccreate();
	ProgramPostProcbufferData(NULL, 0, 0, NULL);
}

// ----------------- PROGRAM 3D -------------------------------
Renderer *Program3Dcreate() {
    Renderer *prog = new Renderer(3);
    prog->program = Program3D;
    prog->mat = MATERIAL_METAL;
    
    return (Renderer *)prog;
}

void Program3DbufferData(Renderer *p, int type, long num, void *data) {
    size_t scalar;
    GLuint bufType;
    
    if(type == VERTEX_BUFFER) {
        bufType = GL_ARRAY_BUFFER;
        scalar = sizeof(float);
        
        glBindBuffer(bufType, p->getBuffer(0));
    }
    else if(type == NORMAL_BUFFER) {
        bufType = GL_ARRAY_BUFFER;
        scalar = sizeof(float);
        
        glBindBuffer(bufType, p->getBuffer(1));
    }
    else if(type == INDICES_BUFFER) {
        bufType = GL_ELEMENT_ARRAY_BUFFER;
        scalar = sizeof(unsigned int);
        
        glBindBuffer(bufType, p->getBuffer(2));
    }
    else {
        std::cerr << "Buffer type " << type << " not recognized" << std::endl;
        exit(1);
    }
    
    glBufferData(bufType, scalar * num, data, GL_STATIC_DRAW);
}

void Program3Drender(Renderer *p, glm::mat4 Model) {
    glUseProgram(Program3D->programID);
    
    // Send window scale
    setUniforms(Program3D_uWinScale, Program3D_uProj, Program3D_uView, Program3D_uModel, Model);
    glUniform3f(Program3D_uBend, p->bend.x, p->bend.y, p->bend.z);
    
    setMaterial(p->mat, Program3D_uDColor, Program3D_uSColor, Program3D_uAColor, Program3D_uShine);
    
    glUniform3f(Program3D_uLightPos, 100, 20, 33);
    
    // Bind attributes...
    // XYZ Position
    glEnableVertexAttribArray(Program3D_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, p->getBuffer(0));
    glVertexAttribPointer(Program3D_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // integer color
    glEnableVertexAttribArray(Program3D_aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, p->getBuffer(1));
    glVertexAttribPointer(Program3D_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p->getBuffer(2));
    glDrawElements(GL_TRIANGLES, p->getNumElements(), GL_UNSIGNED_INT, 0);
    
    if(p->getNumElements() == 0)
        std::cout << "WARNING: Rendering a sprite with 0 elements" << std::endl;
    // Cleanup
    glDisableVertexAttribArray(Program3D_aPosition);
    glDisableVertexAttribArray(Program3D_aNormal);

    glUseProgram(0);
}

// ----------------- PROGRAM TEXT -----------------------------
TexRenderer *TextRenderer = NULL;
void renderText(const char *text, float x, float y) {
    return;
   if (TextRenderer == NULL) {
      TextRenderer = (TexRenderer *) ProgramTextcreate();
      TextRenderer->loadTexture("text.bmp");
   }

   std::vector<glm::vec2> vertices;
   std::vector<glm::vec2> UVs;

   int length = strlen(text);
   int size = 32;
   for ( unsigned int i=0 ; i<length ; i++ ){
      glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
      glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
      glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
      glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

      vertices.push_back(vertex_up_left   );
      vertices.push_back(vertex_down_left );
      vertices.push_back(vertex_up_right  );

      vertices.push_back(vertex_down_right);
      vertices.push_back(vertex_up_right);
      vertices.push_back(vertex_down_left);

      char character = text[i];
      float uv_x = (character%16)/16.0f;
      float uv_y = (character/16)/16.0f;

      glm::vec2 uv_up_left    = glm::vec2( uv_x           , 1.0f - uv_y );
      glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, 1.0f - uv_y );
      glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, 1.0f - (uv_y + 1.0f/16.0f) );
      glm::vec2 uv_down_left  = glm::vec2( uv_x           , 1.0f - (uv_y + 1.0f/16.0f) );

      UVs.push_back(uv_up_left   );
      UVs.push_back(uv_down_left );
      UVs.push_back(uv_up_right  );

      UVs.push_back(uv_down_right);
      UVs.push_back(uv_up_right);
      UVs.push_back(uv_down_left);
   }

   ProgramTextbufferData(TextRenderer, VERTEX_BUFFER, vertices.size(), &vertices[0]);
   ProgramTextbufferData(TextRenderer, UV_BUFFER, UVs.size(), &UVs[0]);
   TextRenderer->setNumElements(vertices.size());

   ProgramTextrender(TextRenderer, glm::mat4(1));
}

Renderer *ProgramTextcreate() {
    TexRenderer *prog = new TexRenderer();
    prog->program = ProgramText;
    
    return (Renderer *)prog;
}

void ProgramTextbufferData(Renderer *p, int type, long num, void *data) {
    size_t scalar;
    GLuint bufType;
    
    if(type == VERTEX_BUFFER) {
        bufType = GL_ARRAY_BUFFER;
        scalar = sizeof(glm::vec2);
        
        glBindBuffer(bufType, p->getBuffer(0));
    }
    else if(type == UV_BUFFER) {
        bufType = GL_ARRAY_BUFFER;
        scalar = sizeof(glm::vec2);
        
        glBindBuffer(bufType, p->getBuffer(1));
    }
    else {
        std::cerr << "Buffer type " << type << " not recognized" << std::endl;
        assert(0);
    }
    
    glBufferData(bufType, scalar * num, data, GL_STATIC_DRAW);
}

void ProgramTextrender(Renderer *p, glm::mat4 Model) {
    glUseProgram(ProgramText->programID);
    
    glBindTexture(GL_TEXTURE_2D, ((TexRenderer *)p)->texID);
    
    // Bind attributes...
    // XYZ Position
    glEnableVertexAttribArray(ProgramText_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, p->getBuffer(0));
    glVertexAttribPointer(ProgramText_aPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // integer color
    glEnableVertexAttribArray(ProgramText_aTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, p->getBuffer(1));
    glVertexAttribPointer(ProgramText_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, p->getNumElements());
    
    if(p->getNumElements() == 0)
        std::cout << "WARNING: Rendering a sprite with 0 elements" << std::endl;
    // Cleanup
    glDisableVertexAttribArray(ProgramText_aPosition);
    glDisableVertexAttribArray(ProgramText_aTexCoord);

    glUseProgram(0);
}

void TexRenderer::loadTexture(char *filename) {
    LoadTexture(filename, texID);
}

// ------------------- TEXTURE LOADING ------------------------
//routines to load in a bmp files - must be 2^nx2^m and a 24bit bmp
GLvoid LoadTexture(char* image_file, int texID) {
    
    TextureImage = (Image *) malloc(sizeof(Image));
    if (TextureImage == NULL) {
        printf("Error allocating space for image");
        exit(1);
    }
    cout << "trying to load " << image_file << endl;
    if (!ImageLoad(image_file, TextureImage)) {
        exit(1);
    }
    /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
    /*  x size from image, y size from image,                                              */
    /*  border 0 (normal), rgb color data, unsigned byte data, data  */
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 TextureImage->sizeX, TextureImage->sizeY,
                 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
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

int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 /*  size of the image in bytes. */
    unsigned long i;                    /*  standard counter. */
    unsigned short int planes;          /*  number of planes in image (must be 1)  */
    unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
    char temp;                          /*  used to convert bgr to rgb color. */
    
    /*  make sure the file is there. */
    if ((file = fopen(filename, "rb"))==NULL) {
        printf("File Not Found : %s\n",filename);
        return 0;
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
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }
    
    /*  read the bpp */
    bpp = getshort(file);
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
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
        printf("Error reading image data from %s.\n", filename);
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

GLuint vbo_fbo_vertices;
GLfloat fbo_vertices[] = {
	-1, -1,
	1, -1,
	-1, 1,
	1, 1,
};

Renderer *ProgramPostProccreate() {
	TexRenderer *prog = new TexRenderer();
	prog->program = ProgramPostProc;

	glGenBuffers(1, &vbo_fbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return (Renderer *)prog;
}

void ProgramPostProcbufferData(Renderer *p, int type, long num, void *data) {
	printf("Nothing done in PostProcBufferData\n");

	/* init_resources */
	/* Create back-buffer, used for post-processing */

	/* Texture */
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Depth buffer */
	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1024, 768);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Framebuffer to link everything together */
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: error %p", status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint get_fbo() {
	return fbo;
}


void ProgramPostProcrender(Renderer *p, glm::mat4 Model) {
//	glClearColor(0.0, 0.0, 0.0, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ProgramPostProc->programID);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(ProgramPostProc_fbo_texture, /*GL_TEXTURE*/0);
	glEnableVertexAttribArray(ProgramPostProc_v_coord);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glVertexAttribPointer(
		ProgramPostProc_v_coord,  // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(ProgramPostProc_v_coord);

	glUseProgram(0);

//	glfwSwapBuffers((GLFWwindow *)p);
}