#include <iostream>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>

#include "main.h"
#include "obj_parser.h"

#define WHITESPACE " \t\n\r"

// Cache of OBJ data
std::unordered_map<std::string, Obj::ObjData *> objects;

glm::vec3 Obj::ParseVec3(std::string line) {
   glm::vec3 result;
   
   sscanf(line.c_str(), "%f %f %f", &result.x, &result.y, &result.z);
   
   return result;
}

std::unique_ptr<Obj::Face> Obj::ParseFace(std::string line, int mat) {
   std::unique_ptr<Face> face((Face *)malloc(sizeof(Face)));
   face->vertex_count = 0;
   face->material_index = mat;
   
   do {
      assert(face->vertex_count < MAX_VERTEX_COUNT);
      
      face->texture_index[face->vertex_count] = 0;
      face->normal_index [face->vertex_count] = 0;
      
      if (line.find("/") != std::string::npos) {
         face->vertex_index[face->vertex_count] = atoi(line.substr(0, line.find("/")).c_str());
         assert(face->vertex_index[face->vertex_count] > 0);
         
         // Load the texture info
         line = line.substr(line.find("/") + 1);
         if (line.find("/") != std::string::npos) {
            // Only load in texture if it's not empty
            if (line[0] != '/')
               face->texture_index[face->vertex_count] = atoi(line.substr(0, line.find("/")).c_str());
            
            // Load the normal info
            line = line.substr(line.find("/") + 1);
            face->normal_index[face->vertex_count] = atoi(line.c_str());
         }
         else {
            face->texture_index[face->vertex_count] = atoi(line.c_str());
         }
      }
      else {
         // Untested
         face->vertex_index[face->vertex_count] = atoi(line.c_str());
      }
      
      face->vertex_count ++;
      if (line.find(" ") == std::string::npos)
         break;
      else
         line = line.substr(line.find(" ") + 1);
   } while(line.length() > 0);
   
   // Convert to 0-based arrays
   for(int i = 0; i < face->vertex_count; i ++) {
      face->vertex_index [i] --;
      face->normal_index [i] --;
      face->texture_index[i] --;
   }
   
   return std::move(face);
}

int Obj::ParseMtlFile(std::vector<std::unique_ptr<Material>> *materials, std::string filename) {
   std::ifstream mtl_stream(filename);
   if (mtl_stream.is_open()) {
      
      std::unique_ptr<Material> current_mat = nullptr;
      bool material_open = false;
      
      std::string line, word;
      while (getline(mtl_stream, line)) {
         // Ignore comments
         if (line.length() == 0 || line[0] == '#' || line.find("//") == 0)
            continue;
         
         int token = line.find(" ");
         word = line.substr(0, token);
         line = line.substr(token + 1);
         
         if (word == "newmtl") {
            material_open = true;
            current_mat   = std::unique_ptr<Material>((Material *)malloc(sizeof(Material)));
            
            // Set material defaults
            current_mat->amb  = glm::vec3(0.2);
            current_mat->diff = glm::vec3(0.8);
            current_mat->spec = glm::vec3(1.0);
            current_mat->trans_filter = glm::vec3(1.0);
            current_mat->reflect = 0.0;
            current_mat->trans = 1;
            current_mat->glossy = 98;
            current_mat->shiny = 0;
            current_mat->refract_index = 1;
            current_mat->texture_filename[0] = '\0';
            
            current_mat->name = line;
            materials->push_back(std::move(current_mat));
         }
         else {
            assert(material_open);
            
            if (word == "Ka") {
               current_mat->amb = ParseVec3(line);
            }
            else if (word == "Kd") {
               current_mat->diff = ParseVec3(line);
            }
            else if (word == "Ks") {
               current_mat->spec = ParseVec3(line);
            }
            else if (word == "Ns") {
               current_mat->shiny = atof(line.c_str());
            }
            else if (word == "d") {
               current_mat->trans = atof(line.c_str());
            }
            else if (word == "r") {
               current_mat->reflect = atof(line.c_str());
            }
            else if (word == "sharpness") {
               current_mat->glossy = atof(line.c_str());
            }
            else if (word == "Ni") {
               current_mat->refract_index = atof(line.c_str());
            }
            else if (word == "Tf") {
               current_mat->trans_filter = ParseVec3(line);
            }
            else if (word == "map_Ka") {
               current_mat->texture_filename = line;
            }
         }
      }
   }
   
	return 1;
}

int c = 0;
Obj::ObjData *Obj::ParseObjFile(std::string filename) {
   std::unordered_map<std::string, ObjData *>::iterator cachedObj = objects.find(filename);
   if (cachedObj != objects.end())
      return cachedObj->second;
   
   std::cout << "Loading " << filename << "..." << std::endl;
   
   Obj::ObjData *data = new ObjData;
   
   std::ifstream obj_stream(filename);
   int current_mat = -1;
   
   std::string line, word;
   if (obj_stream.is_open()) {
      while (getline(obj_stream, line)) {
         // Skip comments
         if (line.length() == 0 || line[0] == '#') {
            continue;
         }
         
         int token = line.find(" ");
         word = line.substr(0, token);
         line = line.substr(token + 1);
         
         if (word == "v") {
            glm::vec3 vector = ParseVec3(line);
            data->vertices.push_back(vector);
         }
         else if (word == "vn") {
            data->normals.push_back(ParseVec3(line));
         }
         else if (word == "vt") {
            data->textures.push_back(ParseVec3(line));
         }
         else if (word == "f") {
            std::unique_ptr<Face> face = ParseFace(line, current_mat);
            
            std::cout << line << ": ";
            
            for (int i = 0; i < 3; i ++) {
               data->indices.push_back(face->vertex_index[i]);
               std::cout << data->indices[data->indices.size() - 1] << " ";
            }
            if (face->vertex_count == 4) {
               data->indices.push_back(face->vertex_index[2]);
               std::cout << data->indices[data->indices.size() - 1] << " ";
               data->indices.push_back(face->vertex_index[1]);
               std::cout << data->indices[data->indices.size() - 1] << " ";
               data->indices.push_back(face->vertex_index[3]);
               std::cout << data->indices[data->indices.size() - 1] << " ";
            }
            
            std::cout << std::endl;
         }
         else if (word == "usemtl") {
            current_mat = 0;
            for (std::unique_ptr<Material> &mat : data->materials) {
               if (mat->name == line.substr(line.find(" ") + 1)) {
                  break;
               }
               // Otherwise, keep searching
               current_mat ++;
            }
         }
         else if (word == "mtllib") {
            ParseMtlFile(&data->materials, line);
         }
         else {
            std::cerr << "Unknown command '" << word << "' in scene: " << line << std::endl;
         }
      }
   }
   
   objects[filename] = data;
   obj_stream.close();
   
   return data;
}