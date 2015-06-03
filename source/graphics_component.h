//
//  graphics_component.h
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#ifndef __Project__graphics_component__
#define __Project__graphics_component__

#include <vector>

#include "track_enum.h"
#include "bounds.h"
#include "renderer.h"

class GameObject;

class GraphicsComponent {
protected:
   Bounds bounds;
   std::vector<Renderer *> renderers;
   
   glm::vec3 current_tint;
   
public:
   GraphicsComponent();
   
   Bounds *getBounds() { return &bounds; }
   virtual void render(glm::mat4 Model);
   virtual void renderOutline(glm::mat4 Model);
   
   GraphicsComponent *clone();
   void clone(GraphicsComponent *other);
   
   void tint(Track lane);
   void setPowerup(float powerup);
};

class ModelRenderer : public GraphicsComponent {
private:
   ModelRenderer(std::string filename, std::string baseDir);

public:
   static ModelRenderer *load(std::string filename, std::string baseDir);
};

class GroundRenderer : public GraphicsComponent {
public:
   GroundRenderer(float size);
};

#endif /* defined(__Project__graphics_component__) */
