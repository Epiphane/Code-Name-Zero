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
}

