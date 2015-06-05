#ifndef ____RenderPostProcess__
#define ____RenderPostProcess__

#include <string>
#include <glm/glm.hpp>

class RendererPostProcess {
private:

    
public:
   RendererPostProcess();
   ~RendererPostProcess();
   static void setupBufferData(int screenWidth, int screenHeight);
   static void render(int blur);
   static void capture();
   static void endCapture();
   static void shaders_init();
   static unsigned int get_fbo();
        
};



#endif
