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

#include "bounds.h"
#include "renderer.h"

class GameObject;

class GraphicsComponent {
protected:
   Bounds bounds;
   std::vector<Renderer *> renderers;
   
public:
   GraphicsComponent();
   
   Bounds *getBounds() { return &bounds; }
   virtual void render(GameObject *obj);
   
   GraphicsComponent *clone();
   void clone(GraphicsComponent *other);
};

class ModelRenderer : public GraphicsComponent {
public:
   ModelRenderer(std::string filename, std::string baseDir);
   ModelRenderer(std::string filename);
};

class GroundRenderer : public GraphicsComponent {
public:
   GroundRenderer(float size);
};

#endif /* defined(__Project__graphics_component__) */
