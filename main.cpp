/*#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

using glm::mat4;

int main()
{
	glfwSetErrorCallback(
		[](int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}
	);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Simple example", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(
		window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	);

	glfwSetCursorPosCallback(
		window,
		[](auto* window, double x, double y)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
	}
	);

	glfwSetMouseButtonCallback(
		window,
		[](auto* window, int button, int action, int mods)
	{
		auto& io = ImGui::GetIO();
		const int idx = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : 1;
		io.MouseDown[idx] = action == GLFW_PRESS;
	}
	);

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return 0;
	glfwSwapInterval(1);

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	GLuint handleVBO;
	glCreateBuffers(1, &handleVBO);
	glNamedBufferStorage(handleVBO, 128 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);

	GLuint handleElements;
	glCreateBuffers(1, &handleElements);
	glNamedBufferStorage(handleElements, 256 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);

	glVertexArrayElementBuffer(vao, handleElements);
	glVertexArrayVertexBuffer(vao, 0, handleVBO, 0, sizeof(ImDrawVert));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);

	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, pos));
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, uv));
	glVertexArrayAttribFormat(vao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, IM_OFFSETOF(ImDrawVert, col));

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribBinding(vao, 2, 0);

	glBindVertexArray(vao);

	const GLchar* shaderCodeVertex = R"(
		#version 330 core
		layout (location = 0) in vec2 Position;
		layout (location = 1) in vec2 UV;
		layout (location = 2) in vec4 Color;
		layout(std140, binding = 0) uniform PerFrameData
		{
			uniform mat4 MVP;
		};
		out vec2 Frag_UV;
		out vec4 Frag_Color;
		void main()
		{
			Frag_UV = UV;
			Frag_Color = Color;
			gl_Position = MVP * vec4(Position.xy,0,1);
		}
	)";

	const GLchar* shaderCodeFragment = R"(
		#version 330 core
		in vec2 Frag_UV;
		in vec4 Frag_Color;
		layout (binding = 0) uniform sampler2D Texture;
		layout (location = 0) out vec4 Out_Color;
		void main()
		{
			Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
		}
	)";

	const GLuint handleVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(handleVertex, 1, &shaderCodeVertex, nullptr);
	glCompileShader(handleVertex);

	const GLuint handleFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(handleFragment, 1, &shaderCodeFragment, nullptr);
	glCompileShader(handleFragment);

	const GLuint program = glCreateProgram();
	glAttachShader(program, handleVertex);
	glAttachShader(program, handleFragment);
	glLinkProgram(program);
	glUseProgram(program);

	GLuint perFrameDataBuffer;
	glCreateBuffers(1, &perFrameDataBuffer);
	glNamedBufferStorage(perFrameDataBuffer, sizeof(mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer);

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	// Build texture atlas
	ImFontConfig cfg = ImFontConfig();
	cfg.FontDataOwnedByAtlas = false;
	cfg.RasterizerMultiply = 1.5f;
	cfg.SizePixels = 768.0f / 32.0f;
	cfg.PixelSnapH = true;
	cfg.OversampleH = 4;
	cfg.OversampleV = 4;
	ImFont* Font = io.Fonts->AddFontFromFileTTF("data/OpenSans-Light.ttf", cfg.SizePixels, &cfg);

	unsigned char* pixels = nullptr;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTextures(0, 1, &texture);

	io.Fonts->TexID = (ImTextureID)(intptr_t)texture;
	io.FontDefault = Font;
	io.DisplayFramebufferScale = ImVec2(1, 1);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)width, (float)height);
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		ImGui::Render();

		const ImDrawData* draw_data = ImGui::GetDrawData();

		const float L = draw_data->DisplayPos.x;
		const float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
		const float T = draw_data->DisplayPos.y;
		const float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
		const mat4 orthoProjection = glm::ortho(L, R, B, T);

		//glNamedBufferSubData(perFrameDataBuffer, 0, sizeof(mat4), glm::value_ptr(orthoProjection));

		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			//glNamedBufferSubData(handleVBO, 0, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), cmd_list->VtxBuffer.Data);
			//glNamedBufferSubData(handleElements, 0, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), cmd_list->IdxBuffer.Data);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				const ImVec4 cr = pcmd->ClipRect;
				glScissor((int)cr.x, (int)(height - cr.w), (int)(cr.z - cr.x), (int)(cr.w - cr.y));
				//glBindTextureUnit(0, (GLuint)(intptr_t)pcmd->TextureId);
				glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT,
					(void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)), (GLint)pcmd->VtxOffset);
			}
		}

		glScissor(0, 0, width, height);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}*/





/*
#include <glad/glad.h>
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera3D.h"

#include <stdlib.h>
#include <stdio.h>

using glm::mat4;
using glm::vec3;

static const struct
{
	float x, y;
	//float r, g, b;
} vertices[3] =
{
	{ -0.6f, -0.4f },
	{  0.6f, -0.4f },
	{   0.f,  0.6f }
	/*{ -0.6f, -0.4f,  1.f, 0.f, 0.f },
	{  0.6f, -0.4f,  0.f, 1.f, 0.f },
	{   0.f,  0.6f,  0.f, 0.f, 1.f }* /
};
static const struct
{
	float r, g, b;
} colors[3] =
{
	{ 0.f, 0.f, 1.f },
	{ 0.f, 1.f, 0.f },
	{ 1.f, 0.f, 0.f }
};
static const struct
{
	uint32_t i;
} indices[3] =
{
	{ 0 }, { 1 }, {2}
};// vertex order


static const char* vertex_shader_text = R"(
#version 460 core
layout(std140, binding = 0) uniform PerFrameData
{
	uniform mat4 MVP;
};
layout (location=0) in vec2 vPos;
layout (location=1) in vec3 vCol;
layout (location=0) out vec3 color;
void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
}
)";

static const char* fragment_shader_text = R"(
#version 460 core
layout (location=0) in vec3 color;
layout (location=0) out vec4 out_FragColor;
void main()
{
    out_FragColor = vec4(color, 1.0);
};
)";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool checkStatus(GLuint id)
{
	GLint   SuccessFlag = 0;
	GLsizei Length = 0;
	GLsizei MaxLength = 0;

	//
	if (glIsProgram(id))
	{
		glGetProgramiv(id, GL_LINK_STATUS, &SuccessFlag);
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &MaxLength);

		if (MaxLength)
		{
			char* Log = (char*)alloca(MaxLength);

			glGetProgramInfoLog(id, MaxLength, &Length, Log);

			if (Length) printf("Program info:\n%s", Log);
		}
	}
	else if (glIsShader(id))
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &SuccessFlag);
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &MaxLength);

		if (MaxLength)
		{
			char* Log = (char*)alloca(MaxLength);

			glGetShaderInfoLog(id, MaxLength, &Length, Log);

			if (Length) printf("Shader info:\n%s", Log);
		}
	}
	else
	{
		printf("Unknown object type");
	}

	if (SuccessFlag == 0)
	{
		return false;
	}

	return true;
}


Camera3D *camera = nullptr;

int main(void)
{
	GLFWwindow* window;
	GLuint vertex_buffer, color_buffer, element_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int screen_w = 1024;
	int screen_h = 768;
	window = glfwCreateWindow(screen_w, screen_h, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	camera = new Camera3D(screen_w, screen_h);

	glfwSetKeyCallback(
		window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	);

	glfwSetCursorPosCallback(
		window,
		[](auto* window, double x, double y)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
	}
	);

	glfwSetMouseButtonCallback(
		window,
		[](auto* window, int button, int action, int mods)
	{
		auto& io = ImGui::GetIO();
		const int idx = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : 1;
		io.MouseDown[idx] = action == GLFW_PRESS;
	}
	);

	glfwSetWindowSizeCallback(window,
		[](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		if (camera) camera->UpdateScreenSize(width, height);
	}
	);

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return 0;
	glfwSwapInterval(1);


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();



	// NOTE: OpenGL error checks have been omitted for brevity
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &color_buffer); 
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	
	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 


	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);
	checkStatus(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);
	checkStatus(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	checkStatus(program);

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = 0;
	vcol_location = 1;
	
	// if the colors are stored as an offset of the vertices array.
	/*
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);

	glEnableVertexAttribArray(vcol_location); // enable slot
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(sizeof(float) * 2));
	* /

	GLuint perFrameDataBuffer;
	glCreateBuffers(1, &perFrameDataBuffer);
	glNamedBufferStorage(perFrameDataBuffer, sizeof(mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, perFrameDataBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer);


	ImGuiKey mouseLeft{ 641 };
	ImGuiKey mouseRight{ 642 };

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool show_demo_window = true;
	bool move_camera = false;
	while (!glfwWindowShouldClose(window))
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			move_camera = !ImGui::IsWindowHovered();
			ImGui::End();
		}
		//float ratio;
		int width, height;
		mat4 m, p, mvp;

		glfwGetFramebufferSize(window, &width, &height);
		//ratio = width / (float)height;

		ImGui::Render();
		glfwMakeContextCurrent(window);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		camera->Update();


		//m = glm::rotate(mat4(1.0f), (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
		//p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		//mvp = p * m;

		//ImGui::IsItemHovered()

		if (move_camera)
		{
			//camera setting   
			if (ImGui::IsKeyDown(mouseLeft) && !ImGui::IsAnyItemHovered())
			{
				// pan camera
				glm::dvec2 pan(-io.MouseDelta.x / width, -io.MouseDelta.y / width);
				camera->Pan(pan);
				camera->Update();
			}
			if (ImGui::IsKeyDown(mouseRight))
			{
				// pan camera
				glm::dvec2 orbit(-io.MouseDelta.x / width, -io.MouseDelta.y / width);
				camera->Orbit(orbit);
				camera->Update();
			}
		}
		mvp = camera->ViewProjectionMatrix();

		glUseProgram(program);
		//glUniformMatrix4fv( mvp_location, 1, GL_FALSE, glm::value_ptr( mvp ) );
		glNamedBufferSubData(perFrameDataBuffer, 0, sizeof(mat4), glm::value_ptr(mvp));

		// if the colors are in another vector:
		glEnableVertexAttribArray(vpos_location);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);

		glEnableVertexAttribArray(vcol_location);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(colors[0]), (void*)0); 

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // to draw the ImGUI windows

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}

*/




#include <iostream>
#include "WindowGLFW.h"
#include "DirUtils.h"
#include "GocadReader.h"

int main(int argc, char** args)
{
	std::cout << DirUtils::m_GetCurrentDir() << std::endl;

	WindowGLFW *glfw = new WindowGLFW(false, "teste", 1080, 720);
	glfw->InitializeWindow(); // will init GLEW and GLFW

	/*std::cout << "GocadReader\n";
	GocadReader *gr = new GocadReader();
	gr->ReadDebugLogger(DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../DebugLogger.txt"));
	//glfw->AppendDrawableLine(gr->GetGridLines());
	glfw->AppendDrawableLine(gr->GetSkeletonLines());
	glfw->AppendDrawableLine(gr->GetCellLines());
	glfw->AppendDrawableSphere(gr->GetCellSpheres());
	glfw->AppendDrawableTriangle(gr->GetEnvelopetriangles());*/
	glfw->Run();

	return 0;
}




// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
/*
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "glad/glad.h"
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "Camera3D.h"
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool compile_shaders(unsigned int &shaderProgram, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return success;
}

void set_buffers(const std::vector<float> &vertices, const std::vector<int> &indices, unsigned int &VAO, unsigned int &VBO, unsigned int &EBO)
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

int mScreenWidth = 1280;
int mScreenHeight = 720;
Camera3D *mCamera3D = new Camera3D(mScreenWidth, mScreenHeight);
Mouse *mMouse = nullptr;
void MouseCallback(GLFWwindow * window, double x, double y)
{
	if(mMouse)
		mCamera3D->MouseUpdate(mMouse);
}
void Resize(GLFWwindow* window, int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
	glViewport(0, 0, mScreenWidth, mScreenHeight);
	mCamera3D->UpdateScreenSize(mScreenWidth, mScreenHeight);
	
}

const char *vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.7f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	//mMouse = new Mouse(window);
	//glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetWindowSizeCallback(window, Resize);

	unsigned int shaderProgram1, shaderProgram2;
	if (!compile_shaders(shaderProgram1, vertexShaderSource1, fragmentShaderSource1))
		return -1;
	if (!compile_shaders(shaderProgram2, vertexShaderSource1, fragmentShaderSource2))
		return -1;

	std::vector<float> vertices1, vertices2;
	std::vector<int> indices1, indices2;
	unsigned int VAO1, VAO2;
	unsigned int VBO1, VBO2;
	unsigned int EBO1, EBO2;
	vertices1.push_back(-0.1f); vertices1.push_back(0.5f); vertices1.push_back(0.0f);  // top right
	vertices1.push_back(-0.1f); vertices1.push_back(-0.5f); vertices1.push_back(0.0f);  // bottom right
	vertices1.push_back(-0.5f); vertices1.push_back(-0.5f); vertices1.push_back(0.0f);  // bottom left
	vertices1.push_back(-0.5f); vertices1.push_back(0.5f); vertices1.push_back(0.0f);  // top left
	// note that we start from 0!
	indices1.push_back(0); indices1.push_back(1); indices1.push_back(3); // first Triangle
	indices1.push_back(1); indices1.push_back(2); indices1.push_back(3); // second Triangle


	vertices2.push_back(0.1f); vertices2.push_back(0.5f); vertices2.push_back(0.0f);  // top right
	vertices2.push_back(0.1f); vertices2.push_back(-0.5f); vertices2.push_back(0.0f);  // bottom right
	vertices2.push_back(0.5f); vertices2.push_back(-0.5f); vertices2.push_back(0.0f);  // bottom left
	vertices2.push_back(0.5f); vertices2.push_back(0.5f); vertices2.push_back(0.0f);  // top left
	// note that we start from 0!
	indices2.push_back(0); indices2.push_back(1); indices2.push_back(3); // first Triangle
	indices2.push_back(1); indices2.push_back(2); indices2.push_back(3); // second Triangle

	set_buffers(vertices1, indices1, VAO1, VBO1, EBO1);
	set_buffers(vertices2, indices2, VAO2, VBO2, EBO2);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		/// ***********************
		// draw our first triangle
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time 
		// draw our first triangle
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time
		/// ***********************
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}*/


/*
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <chrono>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>
#include <thread>

const char *vertexShaderSrc = R"(
#version 450

layout (location = 0) in vec2 aPos;

void main() {
    gl_Position = vec4(aPos, 1, 1);
}

)";

const char *fragmentShaderSrc = R"(
#version 450

layout (location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1, 0, 0, 1);
}

)";

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, GLchar const *message,
	void const *user_param) 
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	auto const src_str = [source]() {
		switch (source) {
		case GL_DEBUG_SOURCE_API:
			return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:
			return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER:
			return "OTHER";
		default:
			return "UNKNOWN SOURCE";
		}
	}();

	auto const type_str = [type]() {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY:
			return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:
			return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER:
			return "MARKER";
		case GL_DEBUG_TYPE_OTHER:
			return "OTHER";
		default:
			return "UNKNOWN TYPE";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW:
			return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM:
			return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH:
			return "HIGH";
		default:
			return "UNKNOWN SEVERITY";
		}
	}();
	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id
		<< ": " << message << '\n';
}

int main(int argc, char *argv[]) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 600, "Example", nullptr, nullptr);
	if (!window)
		throw std::runtime_error("Error creating glfw window");
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Error initializing glad");
		


	// Initialize ImGui
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450 core");
	ImGui::StyleColorsClassic();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(message_callback, nullptr);

	//Compile shader
	int success;
	char infoLog[512];
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, 0);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "Vertex shader compilation failed:" << std::endl;
		std::cerr << infoLog << std::endl;
		return 0;
	}

	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "Fragment shader compilation failed:" << std::endl;
		std::cerr << infoLog << std::endl;
		return 0;
	}

	auto program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "Shader linking failed:" << std::endl;
		std::cerr << infoLog << std::endl;
		return 0;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	 //Create vertex array and buffers
	GLuint vao;
	glCreateVertexArrays(1, &vao);

	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE,
		offsetof(glm::vec2, x));

	glVertexArrayAttribBinding(vao, 0, 0);

	glm::vec2 vertices[] = { {-0.2, -0.2}, {-0.2, 0.2}, {0.2, 0.2}, {0.2, -0.2} };

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(glm::vec2) * 4, vertices,
		GL_DYNAMIC_STORAGE_BIT);

	std::uint32_t indices[] = { 0, 2, 1, 2, 0, 3 };

	GLuint ibo;
	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, sizeof(std::uint32_t) * 6, indices,
		GL_DYNAMIC_STORAGE_BIT);

	glBindVertexArray(vao);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(glm::vec2));
	glVertexArrayElementBuffer(vao, ibo);
	glUseProgram(program);
	glClearColor(1, 1, 1, 1);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool showDemo = false;
		ImGui::Begin("Example");
		if (ImGui::Button("Show/Hide ImGui demo"))
			showDemo = !showDemo;
		ImGui::End();
		if (showDemo)
			ImGui::ShowDemoWindow(&showDemo);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}*/













