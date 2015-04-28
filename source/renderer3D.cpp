//
//  renderer_3D.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/26/15.
//
//

#include "main.h"
#include "shader.hpp"
#include "camera.h"
#include "texture.h"
#include "renderer.h"
#include "renderer3D.h"

GLuint Program3D;
GLuint Program3D_uWinScale, Program3D_uProj, Program3D_uModel, Program3D_uView;
GLuint Program3D_uLightPos, Program3D_uAColor, Program3D_uDColor;
GLuint Program3D_uSColor, Program3D_uShine, Program3D_uBend;
GLuint Program3D_aPosition, Program3D_aNormal;
GLuint Program3D_uTexScale, Program3D_uTexUnits, Program3D_uHasTextures;

void setMaterial(unsigned int mat, GLuint uDColor, GLuint uSColor, GLuint uAColor, GLuint uShine) {
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

void Renderer3D_init() {
   Program3D = LoadShaders("./shaders/3DVertex.glsl", "./shaders/3DFragment.glsl");
   Program3D_uWinScale = glGetUniformLocation(Program3D, "windowScale");
   Program3D_uProj = glGetUniformLocation(Program3D, "uProjMatrix");
   Program3D_uModel = glGetUniformLocation(Program3D, "uModelMatrix");
   Program3D_uView = glGetUniformLocation(Program3D, "uViewMatrix");
   Program3D_uAColor = glGetUniformLocation(Program3D, "UaColor");
   Program3D_uDColor = glGetUniformLocation(Program3D, "UdColor");
   Program3D_uSColor = glGetUniformLocation(Program3D, "UsColor");
   Program3D_uLightPos = glGetUniformLocation(Program3D, "uLightPos");
   Program3D_uShine = glGetUniformLocation(Program3D, "uShine");
   Program3D_uBend = glGetUniformLocation(Program3D, "uBend");
   Program3D_aNormal = glGetAttribLocation(Program3D, "aNormal");
   Program3D_aPosition = glGetAttribLocation(Program3D, "aPosition");
   Program3D_uTexUnits = glGetUniformLocation(Program3D, "uTexUnits");
   Program3D_uTexScale = glGetUniformLocation(Program3D, "uTexScale");
   Program3D_uHasTextures = glGetUniformLocation(Program3D, "uHasTextures");
}

Renderer3D::Renderer3D() : Renderer(0), elements(0), numMaterials(0), hasTextures(false) {
   glGenBuffers(NUM_BUFFERS, buffers);
   
   GLenum error = glGetError();
   assert(error == 0);
};

void Renderer3D::render(glm::mat4 Model) {
   GLenum error = glGetError();
   assert(error == 0);
   
   glUseProgram(Program3D);
   
   // Send window scale
   if(w_width > w_height)
      glUniform2f(Program3D_uWinScale, (float) w_height / w_width, 1);
   else
      glUniform2f(Program3D_uWinScale, 1, (float) w_width / w_height);
   
   // Send camera projection
   Model = renderer_getCurrentModel() * Model;
   
   glm::mat4 View = camera_getMatrix();
   glm::mat4 Proj = renderer_getProjection();
   
   glUniformMatrix4fv(Program3D_uModel, 1, GL_FALSE, &Model[0][0]);
   glUniformMatrix4fv(Program3D_uView,  1, GL_FALSE, &View [0][0]);
   glUniformMatrix4fv(Program3D_uProj,  1, GL_FALSE, &Proj [0][0]);
   
   glUniform3fv(Program3D_uAColor,  numMaterials, (float *)ambient);
   glUniform3fv(Program3D_uDColor,  numMaterials, (float *)diffuse);
   glUniform3fv(Program3D_uSColor,  numMaterials, (float *)specular);
   glUniform1fv(Program3D_uShine,   numMaterials, shine);
   glUniform1i(Program3D_uHasTextures, hasTextures);
   if (hasTextures) {
      glUniform2fv(Program3D_uTexScale,numMaterials, (float *)textureScale);
      
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
      glUniform1i(Program3D_uTexUnits, 0);
   }
   
   error = glGetError();
   assert(error == 0);
   
   glUniform3f(Program3D_uLightPos, 100, 20, 33);
   
   // Bind attributes...
   glEnableVertexAttribArray(LOCATION_POSITION);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_vertex]);
   glVertexAttribPointer(LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glEnableVertexAttribArray(LOCATION_NORMAL);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_normal]);
   glVertexAttribPointer(LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   glEnableVertexAttribArray(LOCATION_UV);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_uv]);
   glVertexAttribPointer(LOCATION_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
   glEnableVertexAttribArray(LOCATION_MATERIAL);
   glBindBuffer(GL_ARRAY_BUFFER, buffers[b_material]);
   glVertexAttribPointer(LOCATION_MATERIAL, 1, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Draw it!
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[b_index]);
   glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
   
   if(elements == 0)
      std::cerr << "WARNING: Rendering an object with 0 elements" << std::endl;
   
   // Cleanup
   glDisableVertexAttribArray(LOCATION_POSITION);
   glDisableVertexAttribArray(LOCATION_NORMAL);
   glDisableVertexAttribArray(LOCATION_UV);
   glDisableVertexAttribArray(LOCATION_MATERIAL);
   
   glUseProgram(0);
   
   error = glGetError();
   assert(error == 0);
}

void Renderer3D::setMaterials(std::string baseDir, const std::vector<tinyobj::material_t>& data) {
   numMaterials = data.size();
   std::cout << numMaterials << std::endl;
   assert(numMaterials < MAX_MATERIALS);
   if (numMaterials == 0)
      return;
   
   if (hasTextures) {
      glDeleteTextures(0, &texture);
      hasTextures = false;
   }
   
   GLenum error = glGetError();
   assert(error == 0);
   
   for(int i = 0; i < numMaterials; i ++) {
      ambient[i]  = glm::vec3(data[i].ambient[0],  data[i].ambient[1],  data[i].ambient[2]);
      diffuse[i]  = glm::vec3(data[i].diffuse[0],  data[i].diffuse[1],  data[i].diffuse[2]);
      specular[i] = glm::vec3(data[i].specular[0], data[i].specular[1], data[i].specular[2]);
      shine[i] = data[i].shininess;
      
      if (data[i].diffuse_texname.length() > 0) {
         if (!hasTextures) {
            glGenTextures(1,&texture);
            glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
            glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE, numMaterials);
            
            hasTextures = true;
         }
      
         int width, height;
         texture_loadToArray(baseDir + data[i].diffuse_texname, texture, i, &width, &height);
         textureScale[i] = glm::vec2((float) width / MAX_TEXTURE_SIZE, (float) height / MAX_TEXTURE_SIZE);
      }
   }
}

void Renderer3D::bufferData(DataType type, size_t size, void *data) {
   GLuint bufType;
   
   if(type == Vertices) {
      bufType = GL_ARRAY_BUFFER;
      
      glBindBuffer(bufType, buffers[b_vertex]);
   }
   else if(type == Normals) {
      bufType = GL_ARRAY_BUFFER;
      
      glBindBuffer(bufType, buffers[b_normal]);
   }
   else if(type == UVs) {
      bufType = GL_ARRAY_BUFFER;
      
      glBindBuffer(bufType, buffers[b_uv]);
   }
   else if(type == Materials) {
      bufType = GL_ARRAY_BUFFER;
      
      glBindBuffer(bufType, buffers[b_material]);
   }
   else if(type == Indices) {
      bufType = GL_ELEMENT_ARRAY_BUFFER;
      
      glBindBuffer(bufType, buffers[b_index]);
   }
   else {
      std::cerr << "Buffer type " << type << " not recognized" << std::endl;
      assert(0);
   }
   
   glBufferData(bufType, size, data, GL_STATIC_DRAW);
}

void Renderer3D::bufferData(DataType type, const std::vector<float> &data) {
   assert(type == Vertices || type == UVs || type == Normals || type == Materials);
   bufferData(type, sizeof(float) * data.size(), (void *)&data[0]);
}

void Renderer3D::bufferData(DataType type, const std::vector<glm::vec3> &data) {
   assert(type == Vertices || type == UVs || type == Normals);
   bufferData(type, sizeof(glm::vec3) * data.size(), (void *)&data[0]);
}

void Renderer3D::bufferData(DataType type, const std::vector<unsigned int> &d) {
   assert(type == Indices || type == Materials);
   bufferData(type, sizeof(unsigned int) * d.size(), (void *)&d[0]);
}

void Renderer3D::bufferData(DataType type, const std::vector<int> &data) {
   assert(type == Indices || type == Materials);
   bufferData(type, sizeof(int) * data.size(), (void *)&data[0]);
}