//
// seriously modified ZJ Wood January 2015 - conversion to glfw
// inclusion of matrix stack Feb. 2015
// original from Shinjiro Sueda
// October, 2014
//
// Base code for program 3 - does not compile as is, needs other files
//and shaders but has skeleton for much of the data transfer for shading
//and traversal of the mesh for computing normals - you must compute normals

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>
 
#include "main.h"
#include "audio_manager.h"
#include "input_manager.h"
#include "in_game_state.h"
#include "camera.h"
#include "tiny_obj_loader.h"
#include "rendererDebug.h"

using namespace std;

bool DEBUG = false;
void toggleDebug() {
   DEBUG = !DEBUG;
   
   audio_setPaused(DEBUG);
   camera_setDebug(DEBUG);
}

bool showDebugLog = true;
bool isShadowMapRender = false;
void setDebugLog(bool enabled) { showDebugLog = enabled; }

State *currentState = NULL;
State *getCurrentState() { return currentState; }
void setState(State *state) {
   if (currentState != NULL) {
      currentState->pause();
   }
   
   currentState = state;
   currentState->start();
}

GLFWwindow* window;

const int w_width = 1024;
const int w_height = 768;
const char *w_title = "Lab 1";

/*
 * Generates a pseudo random float
 *
 * float l : lower bound
 * float h : upper bound
 */
float randFloat(float l, float h) {
   float r = rand() / (float)RAND_MAX;
      return (1.0f - r) * l + r * h;
}

/*
 * Generates a random vec3 within a circle along the XZ plane 
 *
 * float r - radius of the circle
 */
glm::vec3 randPoint(float r) {
   float randA = randFloat(0, 1);
   float randB = randFloat(0, 1);

   if (randB < randA) {
      float temp = randA;
      randA = randB;
      randB = temp;
   }

   float angle = M_PI * 2 * randA / randB;
   return glm::vec3(randB * r * cos(angle), 0, randB * r * sin(angle));
}

int main(int argc, char **argv) {
   // Initialise GLFW
   if(!glfwInit()) {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return -1;
   }

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   // Open a window and create its OpenGL context
   window = glfwCreateWindow(w_width, w_height, w_title, NULL, NULL);
   if(window == NULL) {
      std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" << std::endl;
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(window);

   // Initialize GLEW
   glewExperimental = true;
   if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      return -1;
   }
   glGetError();

   // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPos(window, w_width / 2, w_height / 2);
   
   input_init(window);
   input_set_callback(GLFW_KEY_SPACE, toggleDebug);
   
   glEnable (GL_BLEND);
   glEnable(GL_DEPTH_TEST);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   glClearColor(0.10f, 0.05f, 0.3f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   shaders_init();
   audio_init();
   
   setState(new FIRST_STATE());
   RendererDebug::instance()->log("Hey there handsome \2", true);
   
   double clock = glfwGetTime();
   INIT_BENCHMARK
   do {
      assert(currentState != NULL);
      
      audio_update();
      
      double nextTime = glfwGetTime();
      if (nextTime - clock > SEC_PER_FRAME) {
         // Compute FPS
         const int fps_sample_rate = 100;
         static float samples[fps_sample_rate] = {1};
         static int pos = 0;
         samples[pos] = nextTime - clock;
         pos = (pos + 1) % fps_sample_rate;
         float elapsed = 0;
         for (int i = 0; i < fps_sample_rate; i ++)
            elapsed += samples[i];
         elapsed = elapsed / fps_sample_rate;
         
         float fps = 1 / elapsed;
         if (fps >= FRAMES_PER_SEC * 29.0f / 30.0f)
            RendererDebug::instance()->log("FPS: " + std::to_string((int) FRAMES_PER_SEC) + " \2", false);
         else {
            std::string msg = "FPS: " + std::to_string((int) fps);
            RendererDebug::instance()->log(msg, false);
            RendererDebug::instance()->log("Time since last frame: " + std::to_string(elapsed), false);
         }
         
         input_update();

         // Update and render the game
         // Use fixed time updating
         if (!DEBUG) {
            currentState->update(nextTime - clock);
         }
         else {
            currentState->update(0);
         }

         currentState->render(glfwGetTime() - clock);
         
         if (showDebugLog)
            RendererDebug::instance()->renderLog();
         else
            RendererDebug::instance()->clearLog();
         
         GLenum error = glGetError();
         assert(error == 0);
      
         // Swap buffers
         glfwSwapBuffers(window);
         glfwPollEvents();
         
         clock = nextTime;
      }
   } // Check if the ESC key was pressed or the window was closed
   while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

   // Close OpenGL window and terminate GLFW
   glfwTerminate();

   return 0;
}

std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
   out << "{" << vec.x << ", " << vec.y << "}";
   return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "}";
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
    out << "\n{\t";
    for(int i = 0; i < 4; i ++) {
        for(int j = 0; j < 4; j ++) {
            if (mat[j][i] == 0)
                out << "0\t";
            else
                out << mat[j][i] << "\t";
        }
        if(i < 3)
            out << std::endl << "\t";
    }
    out << "}";
    return out;
}

// Given a vector of shapes which has already been read from an obj file
// resize all vertices to the range [-1, 1]
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;
    float epsilon = 0.001;
    
    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;
    
    //Go through all vertices to determine min and max of each dimension
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
            if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];
            
            if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
            if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];
            
            if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
            if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
        }
    }
    //From min and max compute necessary scale and shift for each dimension
    float maxExtent, xExtent, yExtent, zExtent;
    xExtent = maxX-minX;
    yExtent = maxY-minY;
    zExtent = maxZ-minZ;
    if (xExtent >= yExtent && xExtent >= zExtent) {
        maxExtent = xExtent;
    }
    if (yExtent >= xExtent && yExtent >= zExtent) {
        maxExtent = yExtent;
    }
    if (zExtent >= xExtent && zExtent >= yExtent) {
        maxExtent = zExtent;
    }
    scaleX = 2.0 / maxExtent;
    shiftX = minX + (xExtent/ 2.0);
    scaleY = 2.0 / maxExtent;
    shiftY = minY + (yExtent / 2.0);
    scaleZ = 2.0/ maxExtent;
    shiftZ = minZ + (zExtent)/2.0;
    
    //Go through all verticies shift and scale them
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
            assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
            assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
            assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
        }
    }
}
