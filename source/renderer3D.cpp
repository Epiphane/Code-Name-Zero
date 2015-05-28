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
#include "in_game_state.h"

#include <glm/gtc/type_ptr.hpp>

GLuint Renderer3D::program, Renderer3D::uWinScale, Renderer3D::uProj;
GLuint Renderer3D::uModel, Renderer3D::uView, Renderer3D::uLightPos, Renderer3D::uCameraPos;
GLuint Renderer3D::uAColor, Renderer3D::uDColor, Renderer3D::uSColor;
GLuint Renderer3D::uShine, Renderer3D::aNormal, Renderer3D::aPosition;
GLuint Renderer3D::uTexScale, Renderer3D::uTexUnits, Renderer3D::uHasTextures;
// TODO Make Shadows have its own Renderer so these aren't necessary
GLuint Renderer3D::uShadowView, Renderer3D::uShadowProj, Renderer3D::uShadowMap;
// Color GLuint for ship tint.
GLuint Renderer3D::uShipTint;

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
   uShadowProj = glGetUniformLocation(program, "uShadowProj");
   uShadowView = glGetUniformLocation(program, "uShadowView");
   uShadowMap = glGetUniformLocation(program, "uShadowMap");
   uShipTint = glGetUniformLocation(program, "uShipTint");

   initialized = true;
}

Renderer3D::Renderer3D(bool isClone) : Renderer(), b_vertex(Vertices), b_uv(UVs), b_normal(Normals), b_material(Materials), b_index(Indices), numMaterials(0), hasTextures(false), tint(0) {
   if (!initialized)
      init();
   
   // Initialize any new buffers
   if (!isClone) {
      b_vertex  .init();
      b_uv      .init();
      b_normal  .init();
      b_material.init();
      b_index   .init();
   }
}

Renderer3D::Renderer3D(Renderer3D *clone) : Renderer3D(true) {
   b_vertex = clone->b_vertex;
   b_uv = clone->b_uv;
   b_normal = clone->b_normal;
   b_material = clone->b_material;
   b_index = clone->b_index;
};

void Renderer3D::render(glm::mat4 Model) {
   
   glUseProgram(program);
   
   // Send window scale
   if (w_width > w_height)
      glUniform2f(uWinScale, (float)w_height / w_width, 1);
   else
      glUniform2f(uWinScale, 1, (float)w_width / w_height);
   
   // LIGHT POSITION. HARDCODED. YEE BREH
   glm::vec3 camPos = camera_getPosition();
   // TODO: Add a way to get the sun vector into this render function for sweet lighting trix
   glm::vec3 shadowLightPos = glm::vec3(-5.0f, 10.0f, 5.0f);
   glm::vec3 lightPos = glm::vec3(0.0f,10.0f,0.0f);
   
   glm::mat4 shadowView = glm::lookAt(shadowLightPos, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
   /*(left, right, bottom, top, zNear, zFar) changes 'dimensions' of shadow map*/
   glm::mat4 shadowProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, 120.0f);
   // Bias matrix to make window coordinates and texture coordinates cooperate.
   glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
                        0.0, 0.5, 0.0, 0.0,
                        0.0, 0.0, 0.5, 0.0,
                        0.5, 0.5, 0.5, 1.0);
   
   glm::mat4 View = camera_getMatrix();
   glm::mat4 Proj = renderer_getProjection();
   
   if (isShadowMapRender) {
      View = shadowView;
      Proj = shadowProj;
   } else {
      shadowProj = biasMatrix * shadowProj;
   }
   
   // Learn matrix pointers from the pro, scrub
   glUniformMatrix4fv(uShadowView, 1, GL_FALSE, glm::value_ptr(shadowView));
   glUniformMatrix4fv(uShadowProj, 1, GL_FALSE, glm::value_ptr(shadowProj));
   // Shadow Texture, dedicated to 1
   glUniform1i(uShadowMap, 1);

   // Send Ship tint to shaders
   glUniform3f(uShipTint, tint.x, tint.y, tint.z);
   
   glUniformMatrix4fv(uView, 1, GL_FALSE, &View[0][0]);
   glUniformMatrix4fv(uProj, 1, GL_FALSE, &Proj[0][0]);
   glUniformMatrix4fv(uModel, 1, GL_FALSE, &Model[0][0]);
   
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
   
   glUniform3f(uLightPos, lightPos.x, lightPos.y, lightPos.z);
   glUniform3f(uCameraPos, camPos.x, camPos.y, camPos.z);
   
   // Bind attributes...
   b_vertex  .attribPointer(LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
   b_normal  .attribPointer(LOCATION_NORMAL,   3, GL_FLOAT, GL_FALSE, 0, 0);
   b_uv      .attribPointer(LOCATION_UV,       2, GL_FLOAT, GL_FALSE, 0, 0);
   b_material.attribPointer(LOCATION_MATERIAL, 1, GL_FLOAT, GL_FALSE, 0, 0);
   
   if (elements == 0)
      DEBUG_LOG("WARNING: Rendering an object with 0 elements");
   
   b_index.bind();
   glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
   // Cleanup
   glDisableVertexAttribArray(LOCATION_POSITION);
   glDisableVertexAttribArray(LOCATION_NORMAL);
   glDisableVertexAttribArray(LOCATION_UV);
   glDisableVertexAttribArray(LOCATION_MATERIAL);
   
   glUseProgram(0);
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
   VBO *buf;
   
   if(type == Vertices) {
      buf = &b_vertex;
   }
   else if(type == Normals) {
      buf = &b_normal;
   }
   else if(type == UVs) {
      buf = &b_uv;
   }
   else if(type == Materials) {
      buf = &b_material;
   }
   else if(type == Indices) {
      buf = &b_index;
   }
   else {
      std::cerr << "Buffer type " << type << " not recognized" << std::endl;
      assert(0);
   }
   
   buf->bufferData(size, data, GL_STATIC_DRAW);
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