#include "Visualizer3D.h"
#include "WindowGLFW.h"

WindowGLFW *window = nullptr;

Visualizer3D::Visualizer3D()
{
	window = new WindowGLFW(false, "Visualizer3D", 1080, 720);
	window->InitializeWindow(); // will init GLEW and GLFW
}

Visualizer3D::~Visualizer3D()
{
}

void Visualizer3D::Run()
{
	if (window)
	{
		window->Run();
	}
}