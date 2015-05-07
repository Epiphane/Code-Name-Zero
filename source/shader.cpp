#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <GL/glew.h>

#include "main.h"
#include "shader.hpp"

#define DEBUG_SHADER false

// ----------------- LOAD SHADERS -----------------------------

GLuint compileShader(const char *filePath, GLenum shaderType) {
   GLuint shaderID = glCreateShader(shaderType);
   
   // Read the Shader code from the file
   string shaderCode;
   ifstream shaderStream(filePath, ios::in);
   if(shaderStream.is_open()){
      string line = "";
      while(getline(shaderStream, line))
         shaderCode += "\n" + line;
      shaderStream.close();
   }
   
   GLint result = GL_FALSE;
   int infoLogLength;
   
   // Compile Shader
   if (DEBUG_SHADER)
      DEBUG_LOG("Compiling shader: " + string(filePath))
   const char *sourcePointer = shaderCode.c_str();
   glShaderSource(shaderID, 1, &sourcePointer , NULL);
   glCompileShader(shaderID);
   
   // Check Shader
   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
   glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
   if(infoLogLength > 0){
      vector<char> errorMessage(infoLogLength+1);
      glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
	  if (errorMessage[0] != '\0') {
		  DEBUG_LOG(string(&errorMessage[0]));
	  }
   }
   
   return shaderID;
}



GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
   return LoadShaders(vertex_file_path, NULL, fragment_file_path);
}

GLuint LoadShaders(const char *vertFilePath, const char *geomFilePath, const char *fragFilePath) {
   GLint result = GL_FALSE;
   int infoLogLength;
   
   // Create shaders
   GLuint vertexShader = compileShader(vertFilePath, GL_VERTEX_SHADER);
   GLuint geomShader;
   if (geomFilePath != NULL)
       geomShader = compileShader(geomFilePath, GL_GEOMETRY_SHADER);
   GLuint fragShader = compileShader(fragFilePath, GL_FRAGMENT_SHADER);
   
   // Link the program
   if (DEBUG_SHADER)
      DEBUG_LOG("Linking Program")
   GLuint programID = glCreateProgram();
   glAttachShader(programID, vertexShader);
   if (geomFilePath != NULL) {
       glAttachShader(programID, geomShader);
   }
   glAttachShader(programID, fragShader);
   glLinkProgram(programID);
   
   // Check the program
   glGetProgramiv(programID, GL_LINK_STATUS, &result);
   glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
   if (infoLogLength > 0){
      vector<char> errorMessage(infoLogLength + 1);
      glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
      
      // Windows returns empty error messages, check if error message is null first before quitting
      if (errorMessage[0] != '\0') {
         printf("%s\n", &errorMessage[0]);
//         exit(1);
      }
   }
   
   glDeleteShader(vertexShader);
   if (geomFilePath != NULL)
       glDeleteShader(geomShader);
   glDeleteShader(fragShader);
   
   return programID;
}


