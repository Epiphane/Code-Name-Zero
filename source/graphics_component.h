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
   Renderer *getRenderer(int i) { return renderers[i]; }
   virtual void render(GameObject *obj);
};

class ModelRenderer : public GraphicsComponent {
public:
   ModelRenderer(const char *filename, const char *baseDir);
   ModelRenderer(const char *filename);
};

class GroundRenderer : public GraphicsComponent {
public:
   GroundRenderer(float size);
};

#endif /* defined(__Project__graphics_component__) */
