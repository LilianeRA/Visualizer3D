include(FetchContent)
cmake_minimum_required(VERSION 3.23)

project(Visualizer3D VERSION 0.0.1 DESCRIPTION "A 3D visualizer using ImGUI, GLFW, GLAD and OpenGL.")
#--- CMake configuration
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#--- Compiler configuration
set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

#FetchContent_Declare(
#        imgui
#        GIT_REPOSITORY https://github.com/ocornut/imgui.git
#        GIT_TAG master
#)

option(BUILD_EXAMPLES "Build the example programs" ON)

#FetchContent_MakeAvailable(imgui)
#add_subdirectory(extern/imgui)
#include_directories(extern/imgui)


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
#--- ImGUI
find_package(IMGUI REQUIRED)
include_directories(${IMGUI_INCLUDE_DIR})

#set(LIBRARIES imgui ${OPENGL_LIBRARIES} )
set(LIBRARIES ${IMGUI_LIBRARY} ${GLFW_LIBRARY} ${GLAD_LIBRARY})

#--- Build example
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()



include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/shaders)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/*.h ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/src/*.cpp)

# setting the install path inside the current directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# build the library
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
add_library(${PROJECT_NAME}_s STATIC ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})

# setting all the headers as available to be read by the client
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS})
target_link_directories(${PROJECT_NAME} PRIVATE ${SOURCES})

# IMPORTED_LINK_DEPENDENT_LIBRARIES brings the libs inported here to the code that will use this lib.
set_target_properties(${PROJECT_NAME} 
    PROPERTIES IMPORTED_LINK_DEPENDENT_LIBRARIES ${LIBRARIES}
    #PUBLIC_HEADER "${HEADERS}"
    PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/Visualizer3D.h"
)

install (TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_s
    #FILES "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION lib/cmake/${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)


