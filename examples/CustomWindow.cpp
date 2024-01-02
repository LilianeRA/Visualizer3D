#include "CustomWindow.h"
#include "imgui.h"

CustomWindow::CustomWindow(bool bidimensional, const std::string& title, int width, int height) 
	: WindowGLFW(bidimensional, title, width, height)
{
	mEnvelopeTransparency = 1.0f;

	mEnvelopeWireframe = false;
	mGridLines = false;
}

CustomWindow::~CustomWindow()
{
	
}

void CustomWindow::SetCustomWindow()
{
	ImGui::Begin("Custom Configuration");
    // custom checkboxes
	ImGui::Checkbox("Envelope Lines", &mEnvelopeWireframe);
	ImGui::Checkbox("Grid lines", &mGridLines);
	ImGui::SliderFloat("Envelope Transparency", &mEnvelopeTransparency, 0.0f, 1.0f);
	ImGui::End();
}

void CustomWindow::CustomDraw()
{
	// for the virtual function
	for (const auto dl : mOtherLines)
	{
		if (dl->GetName().find("Grid") == std::string::npos)
			dl->Draw();
		else
			if (mGridLines)
				dl->Draw();
		//dl->Draw();
	}
	for (const auto ds : mOtherSpheres)
	{
		ds->Draw(mLightPos, mLightColor);
	}
	for (const auto dt : mOtherTriangles)
	{
		if (dt->GetName().find("Envelope") != std::string::npos)
		{
			dt->Draw(mLightPos, mLightColor, mEnvelopeTransparency, mEnvelopeWireframe);
		}
		//else if(dt->GetName().find("Grid") != std::string::npos)
		//    dt->Draw(mLightPos, mLightColor, mGridTransparency, mGridLines);
	}
}

void CustomWindow::CustomShutdown()
{
	for (auto dl : mOtherLines)
		delete dl;
	mOtherLines.clear();
	for (auto ds : mOtherSpheres)
		delete ds;
	mOtherSpheres.clear();
	for (auto dt : mOtherTriangles)
		delete dt;
	mOtherTriangles.clear();
}

void CustomWindow::AppendDrawableLine(DrawableLines* dl)
{
	if (dl)
	{
		mOtherLines.push_back(dl);
		std::cout << "mOtherLines " << mOtherLines.size() << std::endl;
	}
}

void CustomWindow::AppendDrawableSphere(DrawableSpheres* ds)
{
	if (ds)
	{
		mOtherSpheres.push_back(ds);
		std::cout << "mOtherSpheres " << mOtherSpheres.size() << std::endl;
	}

}

void CustomWindow::AppendDrawableTriangle(DrawableTriangles* dt)
{
	if (dt)
	{
		mOtherTriangles.push_back(dt);
	}
	std::cout << "mOtherTriangles " << mOtherTriangles.size() << std::endl;

}