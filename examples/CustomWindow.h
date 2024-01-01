#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include "WindowGLFW.h"

class CustomWindow : public WindowGLFW
{
	public:
		CustomWindow(bool bidimensional, const std::string& title, int width = 1080, int height = 720);
		virtual ~CustomWindow();
	protected:
		virtual void SetCustomWindow() override;
};
#endif // CUSTOMWINDOW_H
