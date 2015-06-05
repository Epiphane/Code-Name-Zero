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
#include "rendererPostProcess.h"
#include "texture.h"
#include "main.h"

int programID;
GLuint PostProc_v_coord, PostProc_fbo_texture, PostProc_blur;
GLuint PostProc_fbo, Local_fbo_texture, PostProc_rbo_depth;
bool PostProc_initialized = false;

GLuint PostProc_vbo_fbo_vertices;
GLfloat PostProc_fbo_vertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f,
};

void RendererPostProcess::shaders_init() {
   if (!PostProc_initialized) {
      programID = LoadShaders("./shaders/PostProcVertex.glsl", "./shaders/PostProcFragment.glsl");
      PostProc_v_coord = glGetAttribLocation(programID, "v_coord");
      PostProc_fbo_texture = glGetUniformLocation(programID, "fbo_texture");
      PostProc_blur = glGetUniformLocation(programID, "blur");
   }

	setupBufferData(w_width, w_height);
}

void RendererPostProcess::setupBufferData(int screenWidth, int screenHeight) {
	/* init_resources */
	/* Create back-buffer, used for post-processing */
   if (PostProc_initialized) {
      // Remove old materials
      glDeleteTextures(1, &Local_fbo_texture);
      glDeleteRenderbuffers(1, &PostProc_rbo_depth);
      glDeleteFramebuffers(1, &PostProc_fbo);
      glDeleteBuffers(1, &PostProc_vbo_fbo_vertices);
   }

   PostProc_initialized = true;

   /* Vertices */
   glGenBuffers(1, &PostProc_vbo_fbo_vertices);
   glBindBuffer(GL_ARRAY_BUFFER, PostProc_vbo_fbo_vertices);
   glBufferData(GL_ARRAY_BUFFER, sizeof(PostProc_fbo_vertices), PostProc_fbo_vertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Texture */
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &Local_fbo_texture);
	glBindTexture(GL_TEXTURE_2D, Local_fbo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Depth buffer */
	glGenRenderbuffers(1, &PostProc_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, PostProc_rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Framebuffer to link everything together */
	glGenFramebuffers(1, &PostProc_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, PostProc_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Local_fbo_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, PostProc_rbo_depth);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: error %p", status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int RendererPostProcess::get_fbo() {
	return PostProc_fbo;
}

void RendererPostProcess::render(int blur) {
//	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	glBindTexture(GL_TEXTURE_2D, Local_fbo_texture);
	glUniform1i(PostProc_fbo_texture, /*GL_TEXTURE*/0);
	glUniform1i(PostProc_blur, blur);
	glEnableVertexAttribArray(PostProc_v_coord);

	glBindBuffer(GL_ARRAY_BUFFER, PostProc_vbo_fbo_vertices);
	glVertexAttribPointer(
		PostProc_v_coord,  // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(PostProc_v_coord);

	glUseProgram(0);
   
}

void RendererPostProcess::capture() {
	glBindFramebuffer(GL_FRAMEBUFFER, PostProc_fbo);
}

void RendererPostProcess::endCapture() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
