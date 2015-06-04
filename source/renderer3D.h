//
//  renderer_3D.h
//  RGBZero
//
//  Created by Thomas Steinke on 4/26/15.
//
//

#ifndef __RGBZero__renderer_3D__
#define __RGBZero__renderer_3D__

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "tiny_obj_loader.h"
#include "vertex_buffer_object.h"
#include "renderer.h"
#include "GLSL.h"

#define MAX_MATERIALS 50

class Renderer3D : public Renderer {
private:
   VBO b_vertex, b_uv, b_normal, b_material, b_index;
   
   GLuint numMaterials;
   glm::vec3 ambient[MAX_MATERIALS], diffuse[MAX_MATERIALS];
   glm::vec3 specular[MAX_MATERIALS];
   float shine[MAX_MATERIALS];
   glm::vec2 textureScale[MAX_MATERIALS];
   GLuint texture;
   
   bool hasTextures;
   
   glm::vec3 tint;
   float powerup;
   
   static bool initialized;
   void init();
   static GLuint program;
   static GLuint uProj, uModel, uView;
   static GLuint uWinScale, uLightPos, uCameraPos;
   static GLuint uAColor, uDColor;
   static GLuint uSColor, uShine;
   static GLuint uTexScale, uTexUnits, uHasTextures;
   static GLuint uShadowView, uShadowProj, uShadowMap;
   static GLuint uShipTint, uPowerup;
   
   static GLuint o_program;
   static GLuint o_uProj, o_uModel, o_uView;
   static GLuint o_uWinScale, o_uTint;
   
public:
   Renderer3D(bool isClone = false);
   Renderer3D(Renderer3D *clone);
   
   Renderer *clone();
   
   void setTint(glm::vec3 t) { tint = t; }
   void setPowerup(float p) { powerup = p; }
   
   // Send data to the GPU
   void bufferData(DataType type, size_t size, void *data);
   void bufferData(DataType type, const std::vector<float>& data);
   void bufferData(DataType type, const std::vector<glm::vec3>& data);
   // SPECIAL CASE: DataType `indices` also sets numElements!!
   void bufferData(DataType type, const std::vector<int>& data);
   void bufferData(DataType type, const std::vector<unsigned int>& data);
   
   // Set materials for an object
   void setMaterials(std::string baseDir, const std::vector<tinyobj::material_t>& data);
   
   // Render the data!
   void render(glm::mat4 Model);
   void renderOutline(glm::mat4 Model);
};

#endif /* defined(__RGBZero__renderer_3D__) */