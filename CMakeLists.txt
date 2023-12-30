include(FetchContent)
cmake_minimum_required(VERSION 3.20)
project(OpenGLCamera)
#--- CMake configuration
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#--- Compiler configuration
set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
)

FetchContent_MakeAvailable(imgui)
add_subdirectory(extern/imgui)


#--- GLAD
find_package(GLAD REQUIRED)
include_directories(${GLAD_INCLUDE_DIR})
#--- GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIR})
#--- Eigen3
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})
#--- GL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})
#--- GLFW
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

include_directories(${IMGUI_DIRECTORY})

set(LIBRARIES ${OPENGL_LIBRARIES} imgui)

#add_subdirectory(extern/imgui)
#--- Build example
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/shaders)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/*.h ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

