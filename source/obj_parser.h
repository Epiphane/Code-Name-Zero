#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>
#include <glm/glm.hpp>

#define OBJ_FILENAME_LENGTH 500
#define MATERIAL_NAME_SIZE 255
#define OBJ_LINE_SIZE 500
#define MAX_VERTEX_COUNT 4 //can only handle quads & triangles

namespace Obj {
   typedef struct Face {
      int vertex_index[MAX_VERTEX_COUNT];
      int normal_index[MAX_VERTEX_COUNT];
      int texture_index[MAX_VERTEX_COUNT];
      int vertex_count;
      int material_index;
   } Face;
   
   typedef struct Material {
      std::string name;
      std::string texture_filename;
      glm::vec3 amb;
      glm::vec3 diff;
      glm::vec3 spec;
      glm::vec3 trans_filter;
      double reflect;
      double refract;
      double trans;
      double shiny;
      double glossy;
      double refract_index;
   } Material;
   
   class ObjData {
   public:
      ObjData() {
         vertices .clear();
         normals  .clear();
         textures .clear();
         indices  .clear();
         materials.clear();
      }
      
      std::vector<glm::vec3> vertices;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec3> textures;
      std::vector<unsigned int> indices;
      
      std::vector<std::unique_ptr<Obj::Face>> faces;
      std::vector<std::unique_ptr<Obj::Material>> materials;
   };
   
   glm::vec3             ParseVec3(std::string line);
   std::unique_ptr<Face> ParseFace(std::string line, int material_index);
   
   int ParseMtlFile(std::vector<std::unique_ptr<Material>> *materials, std::string filename);
   Obj::ObjData *ParseObjFile(std::string filename);
};

#endif
