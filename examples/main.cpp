#include <iostream>
#include "DirUtils.h"
#include "CustomWindow.h"
#include "GocadReader.h"


int main(int argc, char** args)
{
	std::cout << DirUtils::GetCurrentDir() << std::endl;
	std::cout << DirUtils::GetMainDir() << std::endl;

	CustomWindow *glfw = new CustomWindow(false, "gocad_demo", 1080, 720);
	glfw->InitializeWindow(); // will init GLEW and GLFW

	std::cout << "GocadReader\n";
	GocadReader *gr = new GocadReader();
	gr->ReadDebugLogger(DirUtils::JoinPaths(DirUtils::GetMainDir(), "examples/DebugLogger.txt"));
	glfw->AppendDrawableLine(gr->GetGridLines());
	glfw->AppendDrawableLine(gr->GetSkeletonLines());
	glfw->AppendDrawableLine(gr->GetCellLines());
	glfw->AppendDrawableSphere(gr->GetCellSpheres());
	//glfw->AppendDrawableTriangle(gr->GetGridTriangles());
	glfw->AppendDrawableTriangle(gr->GetEnvelopeTriangles());
	glfw->Run();

	return 0;
}













