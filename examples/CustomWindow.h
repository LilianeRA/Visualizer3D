#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include "WindowGLFW.h"

class CustomWindow : public WindowGLFW
{
	public:
		CustomWindow(bool bidimensional, const std::string& title, int width = 1080, int height = 720);
		virtual ~CustomWindow();

		void AppendDrawableLine(DrawableLines* dl);
		void AppendDrawableSphere(DrawableSpheres* ds);
		void AppendDrawableTriangle(DrawableTriangles* dt);

	protected:
		virtual void SetCustomWindow() override;
		virtual void CustomDraw() override;
		virtual void CustomShutdown() override;

	private:
		float mEnvelopeTransparency;
		bool mEnvelopeWireframe;
		bool mGridLines;

		std::vector<DrawableLines*> mOtherLines;
		std::vector<DrawableSpheres*> mOtherSpheres;
		std::vector<DrawableTriangles*> mOtherTriangles;
};
#endif // CUSTOMWINDOW_H
