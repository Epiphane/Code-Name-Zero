//
//  static_state.cpp
//  RGBZero
//
//  Created by Thomas on 6/1/15.
//
//

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "tutorial_state.h"
#include "static_state.h"

StaticState::StaticState(std::string background) {
   renderer = new Renderer2D(background, true, 0);
   
   std::vector<glm::vec2> positions, uvs;
   positions.push_back(glm::vec2(-1, 1));
   positions.push_back(glm::vec2(1, -1));
   uvs.push_back(glm::vec2(0));
   uvs.push_back(glm::vec2(1));
   
   std::vector<float> opacities;
   opacities.push_back(1);
   opacities.push_back(1);
   
   renderer->bufferData(Opacities, opacities);
   renderer->bufferData(Vertices, positions);
   renderer->bufferData(UVs, uvs);
}

void StaticState::render(float dt) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   renderer->render(glm::mat4(1));
   GLenum error = glGetError();
   assert(error == 0);
}

LoadingScreen::LoadingScreen() : StaticState("./textures/loading_screen.png"), fading_time(0), progress(0) {
};


void loadObstacleModel(std::string obstacle, std::string extension) {
   std::string baseDir = "models/obstacles/" + obstacle + "_" + extension + "/";
   ModelRenderer::load(baseDir + obstacle + "_" + extension + ".obj", baseDir);
}

void sendProgress(int file, float num_loaded, float total_to_load) {
   float progress = num_loaded / total_to_load;
   write(file, &progress, sizeof(float));
}

void LoadingScreen::start() {
   pipe(loading_pipe);
   // Set reading end of the pipe to nonblocking
   if (fcntl(loading_pipe[0], F_SETFL, O_NONBLOCK) == -1) {
      fprintf(stderr, "Call to fcntl failed.\n");
      exit(1);
   }
   
   if (!fork()) {
      // Loader writes to parent
      close(loading_pipe[0]);
      
      int num_loaded = 0;
      int total_to_load = 9;
      
      // Child loads all models and then exits
      loadObstacleModel("wall", "blue");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("wall", "green");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("wall", "red");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("meteor", "blue");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("meteor", "green");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("meteor", "red");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("obstacle2", "blue");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("obstacle2", "green");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      loadObstacleModel("obstacle2", "red");
      sendProgress(loading_pipe[1], ++num_loaded, total_to_load);
      
      // Done reading. exit process
      close(loading_pipe[1]);
      exit(0);
   }
   else {
      // Parent reads only
      close(loading_pipe[1]);
   }
}

void LoadingScreen::update(float dt) {
   if (progress == 1) {
      fading_time += dt;
      
      std::vector<float> opacities;
      opacities.push_back(1 - fading_time);
      opacities.push_back(1 - fading_time);
      
      renderer->bufferData(Opacities, opacities);
      
      if (fading_time >= 1) {
         setState(new TutorialState(0));
      }
   }
   else {
      float load_progress;
      int result = read(loading_pipe[0], &load_progress, sizeof(float));
      if (result == -1) {
         // EAGAIN means pipe is empty for now
         if (errno != EAGAIN) {
            std::cerr << "Error reading pipe: " << strerror(errno) << std::endl;
         }
      }
      else if (result == 0) {
         // Pipe closed!
         progress = 1;
      }
      else {
         progress = load_progress;
      }
   }
}