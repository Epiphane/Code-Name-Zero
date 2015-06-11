//
//  rendererSky.cpp
//  RGBZero
//
//  Created by Max Linsenbard on 5/25/15.
//
//

#include "rendererSky.h"
#include "camera.h"
#include "main.h"
#include "shader.hpp"
#include "renderer.h"
#include <glm/gtc/type_ptr.hpp>

static GLuint vbo_verticies;
static GLuint vbo_indices;
static float vertices[12] = { -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f,
   1.0f, 1.0f, 0.0f };
static unsigned int indices[6] = { 0, 3, 2,
   0, 1, 3 };

SkyRenderer::SkyRenderer() {
   // load shaders
   program = LoadShaders("./shaders/SkyShaderVertex.glsl", "./shaders/SkyShaderFragment.glsl");
   uInvProjMatrix = glGetUniformLocation(program, "uInvProjMatrix");
   uInvViewMatrix = glGetUniformLocation(program, "uInvViewMatrix");
   uLightDir = glGetUniformLocation(program, "uLightDir");
   uViewport = glGetUniformLocation(program, "uViewport");
   uFramebufferResolution = glGetUniformLocation(program, "uFramebufferResolution");
   uBrightness = glGetUniformLocation(program, "uBrightness");
   uMusicSamples = glGetUniformLocation(program, "uMusicSamples");
   
   glGenBuffers(1, &vbo_verticies);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_verticies);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
   
   glGenBuffers(1, &vbo_indices);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);
   
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SkyRenderer::~SkyRenderer() {
   glDeleteBuffers(1, &vbo_verticies);
   glDeleteBuffers(1, &vbo_indices);
}

void SkyRenderer::render(glm::vec3 lightDirection, float brightness, float *samples) {
   glUseProgram(program);
   
   glm::mat4 InvView = glm::inverse(camera_getMatrix());
   glm::mat4 InvProj = glm::inverse(renderer_getProjection());
   glm::vec4 ViewPort = glm::vec4(0,0,w_width,w_height);
   glm::vec2 FrameBufferResolution = glm::vec2(w_width, w_height);
   
   // Normalize light vector (we want direction from eye to light)
   glm::vec3 normLightDirection = glm::normalize(lightDirection);
   
   // Draw the quad using the skyrenderer program
   glDisable(GL_DEPTH_TEST);
   
   glUniformMatrix4fv(uInvProjMatrix, 1, GL_FALSE, glm::value_ptr(InvProj));
   glUniformMatrix4fv(uInvViewMatrix, 1, GL_FALSE, glm::value_ptr(InvView));
   glUniform2fv(uFramebufferResolution, 1, glm::value_ptr(FrameBufferResolution));
   glUniform4fv(uViewport, 1, glm::value_ptr(ViewPort));
   glUniform3fv(uLightDir, 1, glm::value_ptr(normLightDirection));
   glUniform1f(uBrightness, brightness);

   glUniform1fv(uMusicSamples, 64, samples);
   
   glBindBuffer(GL_ARRAY_BUFFER, vbo_verticies);
   glEnableVertexAttribArray(LOCATION_POSITION);
   glVertexAttribPointer(LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
   
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glDisableVertexAttribArray(LOCATION_POSITION);
   glUseProgram(0);
   glEnable(GL_DEPTH_TEST);
}