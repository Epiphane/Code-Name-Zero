#ifndef ____shader__
#define ____shader__

#include <string>
#include <glm/glm.hpp>

const int VERTEX_BUFFER = 0;
const int COLOR_BUFFER = 1;
const int UV_BUFFER = 1;
const int NORMAL_BUFFER = 2;
const int INDICES_BUFFER = 3;
const int MATERIAL_BUFFER = 4;

typedef enum {
   Vertices, Colors, UVs, Normals, Indices, Materials
} DataType;

// Location for shader attributes
const int LOCATION_POSITION = 0;
const int LOCATION_NORMAL = 1;
const int LOCATION_UV = 2;
const int LOCATION_COLOR = 2;
const int LOCATION_MATERIAL = 3;

const unsigned int MATERIAL_RUBBER = 0;
const unsigned int MATERIAL_METAL = 1;
const unsigned int MATERIAL_GRASS = 2;
const unsigned int MATERIAL_RED_METAL = 3;
const unsigned int MATERIAL_CHAIR = 4;
const unsigned int MATERIAL_RED = 5;
const unsigned int MATERIAL_BLUE = 6;
const unsigned int MATERIAL_YELLOW = 7;
const unsigned int MATERIAL_GREEN = 8;

void shaders_init();
void renderText(const char *text, float x, float y);
glm::mat4 renderer_getCurrentModel();
glm::mat4 renderer_getProjection();

class Renderer;

// Shader programs

typedef struct Program {
    int programID;
    Renderer *(* create)();
    void(* bufferData)(Renderer *p, int type, long elements, void *data);
    void(* render)(Renderer *p, glm::mat4 Model);
} Program;

extern Program *ProgramText;
extern Program *ProgramPostProc;

// Renderers

class Renderer {
private:
    unsigned int *buffers;
    unsigned long elements;
    
    int numBuffers;
    
public:
    Renderer(int numBuffers);
    ~Renderer();
    
    unsigned int getNumElements() { return elements; }
    void setNumElements(unsigned long num) { elements = num; }
    
    unsigned int getBuffer(int num) { return buffers[num]; }
    
    /* Rendering functions */
    Program *program;
    void bufferData(int type, long num, void *data) { program->bufferData(this, type, num, data); }
    virtual void render(glm::mat4 Model) { program->render(this, Model); }
    
    static void pushMatrix(glm::mat4 matrix);
    static void popMatrix();
    
    unsigned int mat;
    glm::vec3 bend;
};

class TexRenderer : public Renderer {
public:
    TexRenderer() : Renderer(2) {};
    unsigned int texID;
    
    void loadTexture(char *filename);
};

void ProgramPostProcrender(Renderer *p, glm::mat4 Model);
unsigned int get_fbo();

#endif
