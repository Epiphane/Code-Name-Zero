//
//  shadowMap.cpp
//  RGBZero
//
//  Created by Max Linsenbard on 5/4/15.
//
//

#include "shadowMap.h"
#include "main.h"

ShadowMap::ShadowMap() {
   textureSize=0;
   
   glGenFramebuffers(1,&frameBufferID);
   glGenTextures(1, &textureID);
}

ShadowMap::~ShadowMap() {
   glDeleteFramebuffers(1,&frameBufferID);
   glDeleteTextures(1, &textureID);
}

void ShadowMap::init(int size) {
   textureSize = size;
   
   glBindTexture(GL_TEXTURE_2D, textureID);
   
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
   
   glBindTexture(GL_TEXTURE_2D, 0);
   
   glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
   
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
   
   // Disable writes and reads to / from the color buffer
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
   
   GLenum status;
   if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
      fprintf(stderr, "glCheckFramebufferStatus: error %u", status);
      return;
   }
   
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::enable() {
   glViewport(0, 0, textureSize, textureSize);
   glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
}

void ShadowMap::disable() {
   glViewport(0, 0, w_width, w_height);
   
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}