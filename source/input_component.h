//
//  input_component.h
//  Project
//
//  Created by Thomas Steinke on 4/12/15.
//
//

#ifndef __Project__input_component__
#define __Project__input_component__

#include "input_manager.h"
class GameObject;

/* Input Components change pieces of the Game Object's state */
class InputComponent {
public:
   virtual void update(GameObject *obj) {};
};

class PlayerInputComponent : public InputComponent {
public:
   void update(GameObject *obj);
};

#endif /* defined(__Project__input_component__) */
