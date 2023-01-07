cmake_minimum_required(VERSION 3.14)

message("CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")

# Define project name and detail
project("glut_example"
    VERSION 0.1.0
    LANGUAGES C CXX
    DESCRIPTION "GLUT program example"
)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Define the executable name of this project, and create a CMake binary target
add_executable(CarExe "car.c")
add_executable(CubeExe "cubes.c")
add_executable(TemplateExe "template.c")
add_executable(WindMillExe "wind_mill.c")

# Assign source files to the executable.
target_link_libraries(CarExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(CubeExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(TemplateExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)
target_link_libraries(WindMillExe PRIVATE GLUT::GLUT OpenGL::GL OpenGL::GLU)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(CarExe PRIVATE X11)
    target_link_libraries(CubeExe PRIVATE X11)
    target_link_libraries(TemplateExe PRIVATE X11)
    target_link_libraries(WindMillExe PRIVATE X11)
endif()