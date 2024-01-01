#include "CustomWindow.h"
#include "imgui.h"

CustomWindow::CustomWindow(bool bidimensional, const std::string& title, int width, int height) 
	: WindowGLFW(bidimensional, title, width, height)
{
}

CustomWindow::~CustomWindow()
{
	
}

void CustomWindow::SetCustomWindow()
{
	ImGui::Begin("Custom Information");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}