cmake_minimum_required(VERSION 3.10)
project(sugi_engine C)

# Libraries
find_package(Lua 5.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Include headers
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(sugi/src)

list(APPEND SUGI_LIBRARIES
    array
    hash
    ecs
    sugi
)

list(APPEND EXAMPLES
    01_hello_world
)

add_subdirectory(sugi)
add_subdirectory(examples)