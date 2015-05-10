//
//  renderer_3D.cpp
//  RGBZero
//
//  Created by Thomas Steinke on 4/26/15.
//
//

#include <unordered_map>
#include <glm/ext.hpp>

#include "main.h"
#include "shader.hpp"
#include "camera.h"
#include "texture.h"
#include "renderer.h"
#include "renderer3D.h"

GLuint Renderer3D::program, Renderer3D::uWinScale, Renderer3D::uProj;
GLuint Renderer3D::uModel, Renderer3D::uView, Renderer3D::uLightPos, Renderer3D::uCameraPos;
GLuint Renderer3D::uAColor, Renderer3D::uDColor, Renderer3D::uSColor;
GLuint Renderer3D::uShine, Renderer3D::aNormal, Renderer3D::aPosition;
GLuint Renderer3D::uTexScale, Renderer3D::uTexUnits, Renderer3D::uHasTextures;
std::vector<Renderer3D *> Renderer3D::renderers;
VBO Renderer3D::rendererMatrices(ArrayBuffer);

bool Renderer3D::initialized = false;
void Renderer3D::init() {
   program = LoadShaders("./shaders/3DVertex.glsl", "./shaders/3DFragment.glsl");
   uWinScale = glGetUniformLocation(program, "windowScale");
   uProj = glGetUniformLocation(program, "uProjMatrix");
   uModel = glGetUniformLocation(program, "uModelMatrix");
   uView = glGetUniformLocation(program, "uViewMatrix");
   uAColor = glGetUniformLocation(program, "UaColor");
   uDColor = glGetUniformLocation(program, "UdColor");
   uSColor = glGetUniformLocation(program, "UsColor");
   uLightPos = glGetUniformLocation(program, "uLightPos");
   uShine = glGetUniformLocation(program, "uShine");
   uTexUnits = glGetUniformLocation(program, "uTexUnits");
   uTexScale = glGetUniformLocation(program, "uTexScale");
   uHasTextures = glGetUniformLocation(program, "uHasTextures");
   uCameraPos = glGetUniformLocation(program, "uCameraPos");

   rendererMatrices.init();

   renderers.clear();

   initialized = true;
}

Renderer3D::Renderer3D(bool isClone) : Renderer(0), elements(0), numMaterials(0), hasTextures(false) {
   if (!initialized)
      init();
   
   // Set the buffers to dirty, so we don't generate them until necessary
   for (int i = 0; i < NUM_BUFFERS; i ++) _d_buffers[i] = isClone;
   if (!isClone)
      glGenBuffers(NUM_BUFFERS, buffers);
   
   GLenum error = glGetError();
   assert(error == 0);

   renderers.push_back(this);
   batch.clear();
};

void Renderer3D::update() {
   std::vector<Renderer3D *>::iterator it;
   for (it = renderers.begin(); it != renderers.end(); it++) {
      (*it)->batchRender();
   }
}

void Renderer3D::render(glm::mat4 Model) {
   batch.push_back(renderer_getCurrentModel() * Model);
}

void Renderer3D::batchRender() {
   if (batch.size() == 0) {
      return;
   }

   glUseProgram(program);

   // Send window scale
   if (w_width > w_height)
      glUniform2f(uWinScale, (float)w_height / w_width, 1);
   else
      glUniform2f(uWinScale, 1, (float)w_width / w_height);

   glm::mat4 View = camera_getMatrix();
   glm::mat4 Proj = renderer_getProjection();
   glUniformMatrix4fv(uView, 1, GL_FALSE, &View[0][0]);
   glUniformMatrix4fv(uProj, 1, GL_FALSE, &Proj[0][0]);

   glUniform3fv(uAColor, numMaterials, (float *)ambient);
   glUniform3fv(uDColor, numMaterials, (float *)diffuse);
   glUniform3fv(uSColor, numMaterials, (float *)specular);
   glUniform1fv(uShine, numMaterials, shine);
   glUniform1i(uHasTextures, hasTextures);
   if (hasTextures) {
      glUniform2fv(uTexScale, numMaterials, (float *)textureScale);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
      glUniform1i(uTexUnits, 0);
   }

   glm::vec3 camPos = camera_getPosition();

//   glUniform3f(uLightPos, 50, 70, -533);
   glUniform3f(uLightPos, camPos.x + 10, camPos.y + 20, camPos.z + 100);
   glUniform3f(uCameraPos, camPos.x, camPos.y, camPos.z);

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

   if (elements == 0)
      DEBUG_LOG("WARNING: Rendering an object with 0 elements");
   
   // Send all matrices
   rendererMatrices.bufferData(sizeof(glm::mat4) * batch.size(), (void *)&batch[0], GL_STREAM_DRAW);
   rendererMatrices.attribPointer(LOCATION_MODEL_MATRIX    , 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)0);
   glVertexAttribDivisor(LOCATION_MODEL_MATRIX    , 1);
   rendererMatrices.attribPointer(LOCATION_MODEL_MATRIX + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(float) * 4));
   glVertexAttribDivisor(LOCATION_MODEL_MATRIX + 1, 1);
   rendererMatrices.attribPointer(LOCATION_MODEL_MATRIX + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(float) * 8));
   glVertexAttribDivisor(LOCATION_MODEL_MATRIX + 2, 1);
   rendererMatrices.attribPointer(LOCATION_MODEL_MATRIX + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(float) * 12));
   glVertexAttribDivisor(LOCATION_MODEL_MATRIX + 3, 1);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[b_index]);
   glDrawElementsInstanced(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0, batch.size());

   // Cleanup
   glDisableVertexAttribArray(LOCATION_POSITION);
   glDisableVertexAttribArray(LOCATION_NORMAL);
   glDisableVertexAttribArray(LOCATION_UV);
   glDisableVertexAttribArray(LOCATION_MATERIAL);
   glDisableVertexAttribArray(LOCATION_MODEL_MATRIX    );
   glDisableVertexAttribArray(LOCATION_MODEL_MATRIX + 1);
   glDisableVertexAttribArray(LOCATION_MODEL_MATRIX + 2);
   glDisableVertexAttribArray(LOCATION_MODEL_MATRIX + 3);

   
   glUseProgram(0);

   //GLenum error = glGetError();
   //assert(error == 0);

   batch.clear();
}

void Renderer3D::setMaterials(std::string baseDir, const std::vector<tinyobj::material_t>& data) {
   numMaterials = data.size();
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
   int bufIndex;
   
   if(type == Vertices) {
      bufType = GL_ARRAY_BUFFER;
      bufIndex = b_vertex;
   }
   else if(type == Normals) {
      bufType = GL_ARRAY_BUFFER;
      bufIndex = b_normal;
   }
   else if(type == UVs) {
      bufType = GL_ARRAY_BUFFER;
      bufIndex = b_uv;
   }
   else if(type == Materials) {
      bufType = GL_ARRAY_BUFFER;
      bufIndex = b_material;
   }
   else if(type == Indices) {
      bufType = GL_ELEMENT_ARRAY_BUFFER;
      bufIndex = b_index;
   }
   else {
      std::cerr << "Buffer type " << type << " not recognized" << std::endl;
      assert(0);
   }
   
   // Make sure we're not overwriting a dirty/cloned buffer
   if (_d_buffers[bufIndex]) {
      glGenBuffers(1, &buffers[bufIndex]);
      DEBUG_LOG("Buffer " + std::to_string(buffers[bufIndex]) + " created")
      _d_buffers[bufIndex] = false;
   }
   glBindBuffer(bufType, buffers[bufIndex]);
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