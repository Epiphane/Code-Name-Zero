cmake_minimum_required(VERSION 3.1)
project(RGBZero)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(SRC_DIR "${PROJECT_SOURCE_DIR}/source")
set(DEP_DIR "${PROJECT_SOURCE_DIR}/dependencies")

# Source / header files
include("sources.txt")

# GLFW
set(GLFW_DIR "${DEP_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory(${GLFW_DIR})
list(APPEND INCLUDES
   ${GLFW_DIR}/include
)
list(APPEND LIBS
   glfw ${GLFW_LIBRARIES}
)

# glew
set(GLEW_DIR "${DEP_DIR}/glew")
list(APPEND LIB_SOURCES
   ${GLEW_DIR}/src/glew.c
)
list(APPEND INCLUDES
   ${GLEW_DIR}/include
)

#glm
list(APPEND INCLUDES
   ${DEP_DIR}
)

# FMOD
set(FMOD_DIR "${DEP_DIR}/FMOD")
set(FMOD_INCLUDE_DIR "${FMOD_DIR}/include")
list(APPEND INCLUDES
   ${FMOD_INCLUDE_DIR}
)

if (APPLE)
   set(FMOD_LIBRARY_RELEASE "${FMOD_DIR}/lib/osx/release/libfmod.dylib")
   set(FMOD_LIBRARY_DEBUG "${FMOD_DIR}/lib/osx/debug/libfmodL.dylib")
   list(APPEND LIBS
   ${FMOD_LIBRARY_RELEASE}
      ${FMOD_LIBRARY_DEBUG}
   )
endif (APPLE)

if (WIN32)
   if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(FMOD_WIN_ARCHITECTURE "x86_64")
   else (CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(FMOD_WIN_ARCHITECTURE "x86")
   endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

   set(FMOD_LIBRARY_RELEASE "${FMOD_DIR}/lib/win32/${FMOD_WIN_ARCHITECTURE}/release/fmod_vc.lib")
   set(FMOD_LIBRARY_DEBUG "${FMOD_DIR}/lib/win32/${FMOD_WIN_ARCHITECTURE}/debug/fmodL_vc.lib")
   list(APPEND LIBS
      ${FMOD_DIR}/lib/win32/${FMOD_WIN_ARCHITECTURE}/release/fmod_vc.lib
      ${FMOD_DIR}/lib/win32/${FMOD_WIN_ARCHITECTURE}/debug/fmodL_vc.lib
   )
      file(COPY ${FMOD_DIR}/lib/win32/${FMOD_WIN_ARCHITECTURE}/release/fmod.dll DESTINATION ${PROJECT_BINARY_DIR})
endif (WIN32)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(FMOD_LINUX_ARCHITECTURE "x86_64")
   else (CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(FMOD_LINUX_ARCHITECTURE "x86")
   endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

   set(FMOD_LIBRARY_RELEASE "${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/release/libfmod.so.5.10")
   set(FMOD_LIBRARY_DEBUG "${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/debug/libfmodL.so.5.10")
   list(APPEND LIBS
      ${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/release/libfmod.so.5.10
      ${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/release/libfmod.so.5
      ${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/release/libfmod.so
      ${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/debug/libfmodL.so.5.10
      ${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/debug/libfmodL.so.5
      ${FMOD_DIR}/lib/linux/${FMOD_LINUX_ARCHITECTURE}/debug/libfmodL.so
   )
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# dl libs
list(APPEND LIBS
   ${CMAKE_DL_LIBS}
)

foreach(HEADER ${HEADERS})
   list(APPEND ABS_HEADERS
      "${SRC_DIR}/${HEADER}"
   )
endforeach(HEADER)

foreach(SOURCE ${SOURCES})
   list(APPEND ABS_SOURCES
      "${SRC_DIR}/${SOURCE}"
   )
endforeach(SOURCE)

if (WIN32)
   foreach(RESOURCE ${RESOURCES})
      list(APPEND ABS_RESOURCES
         "${SRC_DIR}/${RESOURCE}"
      )
   endforeach(RESOURCE)

   file(COPY ${ABS_RESOURCES} DESTINATION ${PROJECT_BINARY_DIR})
endif (WIN32)

foreach(SHADER ${SHADERS})
   list(APPEND ABS_SHADERS
      "${SRC_DIR}/shaders/${SHADER}"
   )
endforeach(SHADER)

include_directories(${INCLUDES})
add_executable(${PROJECT_NAME} ${ABS_SOURCES} ${ABS_HEADERS} ${LIB_SOURCES} ${ABS_SHADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS})

set_target_properties(RGBZero PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/")
# create_target_launcher(RGBZero WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source/")
# create_default_target_launcher(RGBZero WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source/")

SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )