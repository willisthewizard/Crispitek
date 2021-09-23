# This version of CMake should work
cmake_minimum_required(VERSION 3.16)
# Name project
project(crispitek)
# Language and compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-c -Wall)
# Add source files
file(GLOB_RECURSE SOURCE_FILES
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/*.cpp)
# Add header files
file(GLOB_RECURSE HEADER_FILES
	${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/src/*.hpp)
# Copy resouces
file(GLOB RESOURCES ${PROJECT_SOURCE_DIR}/resources/*)
file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
#STB
include_directories(${PROJECT_SOURCE_DIR}/lib/stb)
#GLW
include_directories(${PROJECT_SOURCE_DIR}/lib/GLM)
# GLFW
# Don't build docs, tests, or examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# Add GLFW to build
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/lib/glfw)
add_subdirectory(${GLFW_DIR} ${CMAKE_CURRENT_BINARY_DIR}/glfw)
include_directories(${GLFW_DIR}/include)
# Glad
set(GLAD_DIR ${PROJECT_SOURCE_DIR}/lib/glad)
include_directories(${GLAD_DIR}/include)
# Prevent GLFW from defining gl symbols and conflicting with Glad if
# GLFW was included first
add_definitions(-DGLFW_INCLUDE_NONE)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${GLAD_DIR}/src/glad.c)
# Link to libraries
target_link_libraries(${PROJECT_NAME} glfw -static-libstdc++ -static-libgcc ${GLFW_LIBRARIES})
