#add_custom_target(gocad_demo)

include_directories(../include)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/examples/*.h)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/examples/*.cpp)
add_executable(gocad_demo ${HEADERS} ${SOURCES})
target_link_libraries(gocad_demo ${LIBRARIES} ${PROJECT_NAME}_s)









