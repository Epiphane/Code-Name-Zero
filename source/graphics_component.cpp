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

GraphicsComponent::GraphicsComponent() {
   renderers.clear();
   bounds.min_x = bounds.max_x = 0;
   bounds.min_y = bounds.max_y = 0;
   bounds.min_z = bounds.max_z = 0;
}

void GraphicsComponent::render(GameObject *obj) {
   glm::mat4 Model = obj->getModel();
   
   std::vector<Renderer *>::iterator renderer;
   for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
      (*renderer)->render(Model);
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

std::unordered_map<std::string, ModelRenderer> modelRenderers;

ModelRenderer::ModelRenderer(std::string filename) : ModelRenderer(filename, "") {};

ModelRenderer::ModelRenderer(std::string filename, std::string baseDir) {
   GraphicsComponent::GraphicsComponent();
   
   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;
   
   std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), baseDir.c_str());
   if(!err.empty()) {
      std::cerr << err << std::endl;
   }
   
   // resize_obj(shapes);
   for(int s = 0; s < shapes.size(); s ++) {
      const std::vector<float> &posBuf = shapes[s].mesh.positions;
      std::vector<float> &norBuf = shapes[s].mesh.normals;
      const std::vector<float> &uvBuf = shapes[s].mesh.texcoords;
      const std::vector<unsigned int> &indBuf = shapes[s].mesh.indices;
      
      if (norBuf.size() == 0) {
         norBuf.clear();
         int idx1, idx2, idx3;
         glm::vec3 v1, v2, v3;
         // For every vertex initialize a normal to 0
         for (int j = 0; j < shapes[s].mesh.positions.size()/3; j++) {
            norBuf.push_back(0);
            norBuf.push_back(0);
            norBuf.push_back(0);
         }
         // Compute the normals for every face
         // then add its normal to its associated vertex
         for (int i = 0; i < shapes[s].mesh.indices.size()/3; i++) {
            idx1 = shapes[s].mesh.indices[3*i+0];
            idx2 = shapes[s].mesh.indices[3*i+1];
            idx3 = shapes[s].mesh.indices[3*i+2];
            v1 = glm::vec3(shapes[s].mesh.positions[3*idx1 +0],shapes[s].mesh.positions[3*idx1 +1], shapes[s].mesh.positions[3*idx1 +2]);
            v2 = glm::vec3(shapes[s].mesh.positions[3*idx2 +0],shapes[s].mesh.positions[3*idx2 +1], shapes[s].mesh.positions[3*idx2 +2]);
            v3 = glm::vec3(shapes[s].mesh.positions[3*idx3 +0],shapes[s].mesh.positions[3*idx3 +1], shapes[s].mesh.positions[3*idx3 +2]);
            
            glm::vec3 u, v;
            
            u = v2 - v1;
            v = v3 - v1;
            norBuf[3*idx1+0] += u.y * v.z - u.z * v.y;
            norBuf[3*idx1+1] += u.z * v.x - u.x * v.z;
            norBuf[3*idx1+2] += u.x * v.y - u.y * v.x;
            
            u = v3 - v2;
            v = v1 - v2;
            norBuf[3*idx2+0] += u.y * v.z - u.z * v.y;
            norBuf[3*idx2+1] += u.z * v.x - u.x * v.z;
            norBuf[3*idx2+2] += u.x * v.y - u.y * v.x;
            
            u = v1 - v3;
            v = v2 - v3;
            norBuf[3*idx3+0] += u.y * v.z - u.z * v.y;
            norBuf[3*idx3+1] += u.z * v.x - u.x * v.z;
            norBuf[3*idx3+2] += u.x * v.y - u.y * v.x;
         }
      }
      
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
