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
#include "texture.h"
#include "main.h"

#include "renderer3D.h"

Program *ProgramText = NULL;
GLuint ProgramText_uTexUnit, ProgramText_aPosition, ProgramText_aTexCoord;
Renderer *ProgramTextcreate();
void ProgramTextbufferData(Renderer *p, int type, long num, void *data);
void ProgramTextrender(Renderer *p, glm::mat4 Model);

Program *ProgramPostProc = NULL;
GLuint ProgramPostProc_v_coord, ProgramPostProc_fbo_texture, ProgramPostProc_blur;
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

glm::mat4 renderer_getCurrentModel() { return currentMVP; }
glm::mat4 renderer_getProjection() { return Projection; }

void Renderer::pushMatrix(glm::mat4 matrix) {
    MatrixStack.push(currentMVP);
    
    currentMVP *= matrix;
}

void Renderer::popMatrix() {
    currentMVP = MatrixStack.top();
    
    MatrixStack.pop();
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

   Renderer3D_init();
   
    // ---------------- TEXT SHADER ---------------------------
    ProgramText = (Program *) malloc(sizeof(Program));

    ProgramText->programID = LoadShaders("./shaders/TextVertex.glsl", "./shaders/TextFragment.glsl");
    ProgramText_aPosition = glGetAttribLocation(ProgramText->programID, "aPosition");
    ProgramText_aTexCoord = glGetAttribLocation(ProgramText->programID, "aUV");
    ProgramText_uTexUnit = glGetAttribLocation(ProgramText->programID, "uTexUnit");
    
    ProgramText->create = &ProgramTextcreate;
    ProgramText->bufferData = &ProgramTextbufferData;
    ProgramText->render = &ProgramTextrender;

	// ---------------- POST-PROCESSOR SHADER ---------------------------

	ProgramPostProc = (Program *)malloc(sizeof(Program));

	ProgramPostProc->programID = LoadShaders("./shaders/PostProcVertex.glsl", "./shaders/PostProcFragment.glsl");
	ProgramPostProc_v_coord = glGetAttribLocation(ProgramPostProc->programID, "v_coord");
	ProgramPostProc_fbo_texture = glGetUniformLocation(ProgramPostProc->programID, "fbo_texture");
	ProgramPostProc_blur = glGetUniformLocation(ProgramPostProc->programID, "blur");


	ProgramPostProc->create = &ProgramPostProccreate;
	ProgramPostProc->bufferData = &ProgramPostProcbufferData;
	ProgramPostProc->render = &ProgramPostProcrender;
	ProgramPostProccreate();
	ProgramPostProcbufferData(NULL, 0, 0, NULL);
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
    texture_load(filename, texID);
}

GLuint vbo_fbo_vertices;
GLfloat fbo_vertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f,
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

unsigned int get_fbo() {
	return fbo;
}


void ProgramPostProcrender(Renderer *p, glm::mat4 Model) {
//	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ProgramPostProc->programID);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(ProgramPostProc_fbo_texture, /*GL_TEXTURE*/0);
	glUniform1i(ProgramPostProc_blur, (int)p);
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