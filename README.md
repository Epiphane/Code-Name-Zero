CPE 476 - RGB Zero

-------------

Pieces of this Readme:
- [Debugging Info](#debugging)

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
