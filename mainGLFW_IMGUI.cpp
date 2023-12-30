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