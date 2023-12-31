set(IMGUI_HEADERS
	imconfig.h
	imgui.h
	imgui_internal.h
	imstb_rectpack.h
	imstb_textedit.h
	imstb_truetype.h
	
	backends/imgui_impl_glfw.h
	backends/imgui_impl_opengl3.h
	backends/imgui_impl_opengl3_loader.h
)

set(IMGUI_SOURCES
	imgui.cpp
	imgui_demo.cpp # useful examples of how to use imgui
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	
	backends/imgui_impl_glfw.cpp
	backends/imgui_impl_opengl3.cpp
)

add_library(imgui
	${IMGUI_HEADERS}
	${IMGUI_SOURCES}
)

target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLAD)

#--- GLFW
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

find_package(GLAD REQUIRED)
include_directories(${GLAD_INCLUDE_DIR})
add_library(
        glad
        ${GLAD_INCLUDE_DIR}/../src/glad.c
        ${GLAD_INCLUDE_DIR}/glad/glad.h
        ${GLAD_INCLUDE_DIR}/KHR/khrplatform.h
)
target_include_directories(glad PUBLIC include)

#add_dependencies(imgui GLFW)
target_link_libraries(imgui PUBLIC ${GLFW_LIBRARIES} glad)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(imgui PROPERTIES FOLDER "External Dependencies")
set_target_properties(imgui PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
set_target_properties(imgui PROPERTIES RELWITHDEBINFO_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
set_target_properties(imgui PROPERTIES MINSIZEREL_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")

set(${IMGUI_INCLUDE_DIR} "${CMAKE_CURRENT_LIST_DIR}")
set(${IMGUI_LIBRARY} "imgui")