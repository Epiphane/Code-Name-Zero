#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <glm/ext.hpp>
#include <typeinfo>
using namespace std;

#include "camera.h"
#include "bounds.h"
#include "shader.hpp"
#include "renderer.h"
#include "texture.h"
#include "main.h"

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
Renderer::Renderer() : elements(0) {};
Renderer::~Renderer() {};

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

Renderer *Renderer::clone() {
   std::cerr << "Clone function not implemented for this kind of renderer" << std::endl;
   // Clone function not implemented for this kind of renderer
   assert(0);
   return NULL;
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

	// ---------------- POST-PROCESSOR SHADER ---------------------------

	ProgramPostProc = (Program *)malloc(sizeof(Program));

	ProgramPostProc->programID = LoadShaders("./shaders/PostProcVertex.glsl", "./shaders/PostProcFragment.glsl");
	ProgramPostProc_v_coord = glGetAttribLocation(ProgramPostProc->programID, "v_coord");
	ProgramPostProc_fbo_texture = glGetUniformLocation(ProgramPostProc->programID, "fbo_texture");
	ProgramPostProc_blur = glGetUniformLocation(ProgramPostProc->programID, "blur");


	ProgramPostProc->create = &ProgramPostProccreate;
	ProgramPostProc->bufferData = &ProgramPostProcbufferData;
//	ProgramPostProc->render = &ProgramPostProcrender;
	ProgramPostProccreate();
	ProgramPostProcbufferData(NULL, 0, 0, NULL);
}

GLuint vbo_fbo_vertices;
GLfloat fbo_vertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f,
};

Renderer *ProgramPostProccreate() {
	Renderer *prog = new Renderer();
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
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


void ProgramPostProcrender(int blur) {
//	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ProgramPostProc->programID);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glUniform1i(ProgramPostProc_fbo_texture, /*GL_TEXTURE*/0);
	glUniform1i(ProgramPostProc_blur, blur);
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