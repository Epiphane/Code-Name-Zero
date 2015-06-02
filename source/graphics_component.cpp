//
//  graphics_component.cpp
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#include <iostream>
#include <unordered_map>

#include "main.h"
#include "tiny_obj_loader.h"
#include "renderer3D.h"
#include "graphics_component.h"
#include "game_object.h"

GraphicsComponent::GraphicsComponent() : current_tint(glm::vec3(0)) {
   renderers.clear();
   bounds.min_x = bounds.max_x = 0;
   bounds.min_y = bounds.max_y = 0;
   bounds.min_z = bounds.max_z = 0;
}

GraphicsComponent *GraphicsComponent::clone() {
   GraphicsComponent *copy = new GraphicsComponent;
   
   clone(copy);
   
   return copy;
}

void GraphicsComponent::clone(GraphicsComponent *copy) {
   // Copy bounds
   memcpy((void *)&copy->bounds, (void *)&bounds, sizeof(Bounds));
   
   // Copy renderers
   std::vector<Renderer *>::iterator renderer;
   for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
      copy->renderers.push_back((*renderer)->clone());
}

void GraphicsComponent::render(glm::mat4 Model) {
   std::vector<Renderer *>::iterator renderer;
   for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
      (*renderer)->render(Model);
}

void GraphicsComponent::renderOutline(glm::mat4 Model) {
   Renderer3D *renderer3D;
   std::vector<Renderer *>::iterator renderer;
   for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++) {
      renderer3D = dynamic_cast<Renderer3D *>(*renderer);
      if (renderer3D != nullptr) {
         renderer3D->renderOutline(Model);
      }
   }
}

void GraphicsComponent::tint(Track lane) {
   glm::vec3 tint(0);
   const float color_intensity = 0.6f;
   switch(lane) {
      case GREEN:
         tint.g = color_intensity;
         break;
      case BLUE:
         tint.b = color_intensity;
         break;
      case RED:
         tint.r = color_intensity;
         break;
   }
   
   if (tint == current_tint) {
      return;
   }
   
   Renderer3D *renderer3D;
   std::vector<Renderer *>::iterator renderer;
   for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++) {
      renderer3D = dynamic_cast<Renderer3D *>(*renderer);
      if (renderer3D != nullptr) {
         renderer3D->setTint(tint);
      }
   }
   
   current_tint = tint;
}

GroundRenderer::GroundRenderer(float size) {
   GraphicsComponent::GraphicsComponent();
   
   bounds.min_x = bounds.min_z = -size;
   bounds.max_x = bounds.max_z =  size;
   bounds.min_y = -1;
   bounds.max_y = 0;
   
   const std::vector<float> posBuf = {
      -size, 0,  size,
      size, 0,  size,
      -size, 0, -size,
      size, 0, -size
   };
   
   const std::vector<unsigned int> indBuf = {
      0, 1, 2, 1, 2, 3
   };
   
   const std::vector<float> norBuf = {
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0
   };
   
   Renderer3D *renderer = new Renderer3D();
   
   renderer->setNumElements(6);
   renderer->bufferData(Indices, indBuf);
   
   renderer->bufferData(Vertices, posBuf);
   renderer->bufferData(Normals, norBuf);
   
   renderers.push_back(renderer);
}

std::unordered_map<std::string, std::string> baseDirs;
std::unordered_map<std::string, ModelRenderer *> modelRenderers;

ModelRenderer *ModelRenderer::load(std::string filename, std::string baseDir) {
   if (baseDirs[filename] == baseDir) {
      if (modelRenderers[filename]) {
         return modelRenderers[filename];
      }
   }

   return new ModelRenderer(filename, baseDir);
}

ModelRenderer::ModelRenderer(std::string filename, std::string baseDir) {
   GraphicsComponent::GraphicsComponent();
   
   if (baseDirs[filename] == baseDir) {
      if (modelRenderers[filename]) {
         modelRenderers[filename]->clone(this);
      
         return;
      }
   }
   
   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;
   
   std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), baseDir.c_str());
   if(!err.empty()) {
      std::cerr << err << std::endl;
   }
   
   // resize_obj(shapes);
   for(int s = 0; s < shapes.size(); s ++) {
      const std::vector<float> &posBuf = shapes[s].mesh.positions;
      const std::vector<float> &norBuf = shapes[s].mesh.normals;
      const std::vector<float> &uvBuf = shapes[s].mesh.texcoords;
      const std::vector<unsigned int> &indBuf = shapes[s].mesh.indices;
      
      std::vector<float> matBuf(posBuf.size() / 3);
      // Apply each material to every vertex
      for(int i = 0; i < shapes[s].mesh.material_ids.size(); i ++) {
         int mat = shapes[s].mesh.material_ids[i];
         matBuf[indBuf[3 * i    ]] = mat;
         matBuf[indBuf[3 * i + 1]] = mat;
         matBuf[indBuf[3 * i + 2]] = mat;
      }
      
      Renderer3D *renderer = new Renderer3D();
      
      renderer->setNumElements(indBuf.size());
      renderer->bufferData(Indices,   indBuf);
      renderer->bufferData(Vertices,  posBuf);
      renderer->bufferData(Normals,   norBuf);
      renderer->bufferData(UVs,       uvBuf);
      renderer->bufferData(Materials, matBuf);
      renderer->setMaterials(baseDir, materials);
      
      renderers.push_back(renderer);
      
      baseDirs[filename] = baseDir;
      modelRenderers.emplace(filename, this);
   }
   
   // Compute bounds
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         if(shapes[i].mesh.positions[3*v+0] < bounds.min_x) bounds.min_x = shapes[i].mesh.positions[3*v+0];
         if(shapes[i].mesh.positions[3*v+0] > bounds.max_x) bounds.max_x = shapes[i].mesh.positions[3*v+0];
         
         if(shapes[i].mesh.positions[3*v+1] < bounds.min_y) bounds.min_y = shapes[i].mesh.positions[3*v+1];
         if(shapes[i].mesh.positions[3*v+1] > bounds.max_y) bounds.max_y = shapes[i].mesh.positions[3*v+1];
         
         if(shapes[i].mesh.positions[3*v+2] < bounds.min_z) bounds.min_z = shapes[i].mesh.positions[3*v+2];
         if(shapes[i].mesh.positions[3*v+2] > bounds.max_z) bounds.max_z = shapes[i].mesh.positions[3*v+2];
      }
   }
}
