CPE 476 - RGB Zero

-------------

Pieces of this Readme:
- [Software Design Goals](#debugging)
   - [Style Guide](#code-style)
   - [Programming Patterns](#programming-patterns)
- [Our Game Engine](#engine)
   - [Components](#components)
      - [Graphics](#graphics-components)
      - [Input](#input-components)
      - [Physics](#physics-components)
      - [Collision](#collision-components)
- [Debugging Info](#debugging)

# Software Design

A lot of the ideas that I tried to make use of in this code base are from the book [Game Programming Patterns](http://gameprogrammingpatterns.com/contents.html) by Robert Nystrom. His opinion on software design is that "good design means that when I make a change, it’s as if the entire program was crafted in anticipation of it". Obviously that's not 100% plausible, but I think it'll help us a lot to keep these points in mind while we work:

1. Nobody should spend more time figuring out your code than you spent writing it.
2. Every file, class, etc. should have **one** purpose, and that purpose should be small.
3. Understanding one piece of the game should not require you to know more pieces (i.e. working on Audio shouldn't require you to understand how the Game Loop is managed).

> To me, this is a key goal of software architecture: minimize the amount of knowledge you need to have in-cranium before you can make progress.

This is a diagram of how Nystrom claims development works best. Make sure that you don't miss the **clean code** step!

![Game coding cycle](http://gameprogrammingpatterns.com/images/architecture-cycle.png)

This leads us to....

## Code Style

It's not very important how you write your code. **HOWEVER**, it can't hurt to keep everything relatively uniform.
- 3 space tabs?
- Curly brackets on the same line?
   ```
   while(true) {
      /* worst program ever */
   }
   ```
- `CONSTANTS_IN_CAPS`
- camelCaseVariables vs. underscore_variables?

## Programming Patterns

1. [Entity-Component System](http://gameprogrammingpatterns.com/component.html)

   Game Objects are simply a collection of "Components", which define the actual game logic.
   This makes creating and modifying game objects much simpler.
   
   [Components](#components) should:
   - Maintain any component-specific state (i.e. velocity, lives, ...)
   - Do all of their work in one function
   - Be simple and not reach past their intended purpose (read the [components](#components) section for more info)

2. [Prototyping](http://gameprogrammingpatterns.com/prototype.html)

   We don't have this yet, but we should consider making it possible to clone Game Objects 
   (for example, when it comes to spawning obstacles)

3. Services

   Stuff like Input, Camera Management, and Audio are referenced from many parts of the program.
   They should have an understandable interface and not be dependent on anything else.

--------------------------------

# Our core engine

## Components

Each GameObject updates its components in `input -> physics -> collision` order, and then all `graphics` components are called.

### Graphics Components



### Input Components

### Physics Components

### Collision Components

--------------------------------

# Debugging!

## Geometric Primitives

GL Primitive structures are super useful for debugging, especially when you need to visualize bounding boxes, important object shapes, etc. BUT the interface is *TOTALLY* different from the rest of your rendering pipeline (and inefficient as well). Here are your key pieces:

### Setting up models
```
glMatrixMode(/* GL_PROJECTION, GL_MODELVIEW */); // This sets up either the Projection or ModelView matrix to be edited

glPushMatrix();
glPopMatrix();
glLoadIdentity(); // Set the top of the matrix to the equivalent of glm::mat4(1)
glLoadMatrixf(float *M); // Load a matrix onto the top of the matrix stack
glMultMatrix(float *M); // Multiply the top of the matrix stack by M
```

### Rendering
```
glBegin(/* GL_TRIANGLES, GL_LINES, GL_QUADS, ... */);
/* Draw stuff here */
glEnd()
```

### Examples

Setting up a projection matrix

```
glm::mat4 Projection = glm::perspective(45.0f, (float) w_width / w_height, 0.01f, 400.0f);

glMatrixMode(GL_PROJECTION); // Set up Projection matrix for changing
glLoadMatrixf(&Projection[0][0]); // Load in the projection matrix from your game

glMatrixMode(GL_MODELVIEW); // Set up ModelView matrix for editing
glLoadIdentity(); // Blank slate
```

Drawing a line

```
glMultMatrix(&Model[0][0]); // Add Model's matrix in the stack
glBegin(GL_LINES);
   glVertex3f(-1, 0, 0);
   glVertex3f( 1, 0, 0);
glEnd();
```
That draws a line across the middle of the model.
