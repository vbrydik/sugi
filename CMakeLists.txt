cmake_minimum_required(VERSION 3.10)
project(sugi C)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Libraries
#find_package(Lua 5.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Include headers
include_directories(
    src
#    ${LUA_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    )

# Add source files
file(GLOB SOURCES src/*.c)
file(GLOB SOURCES_NESTED src/*/*.c)

# Create exececutable
add_executable(sugi ${SOURCES} ${SOURCES_NESTED})
set_property(TARGET sugi PROPERTY C_STANDARD 99)

# Link libraries
target_link_libraries(
    sugi glfw m
#    ${LUA_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )
target_compile_options(sugi PRIVATE -w)

# add_library(Lua STATIC IMPORTED)
# add_dependencies(Lua lua_lib)
