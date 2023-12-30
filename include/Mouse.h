#ifndef MOUSE_H
#define MOUSE_H
#include "GLFW/glfw3.h"


class Mouse
{
    public:
        enum MouseButtons
        {
            MouseButtonsLeft = 0,
            MouseButtonsRight = 1,
            MouseButtonsMiddle = 2,
            MouseButtonsX1 = 3
        };
        typedef struct _MouseState
        {
            _MouseState() : x(0), y(0), scroll(0)
            { for(int i = 0; i < 4; i++) buttons[i] = 0; }
            double x, y;
            long scroll;
            unsigned char buttons[4];
        } MouseState;

        Mouse(GLFWwindow *window);
        virtual ~Mouse();

        void Update();

        /// Getters
        double MouseDeltaX() { return mMouseDeltaX; };
        double MouseDeltaY() { return mMouseDeltaY; };
        double MouseDeltaWheel() { return mMouseDeltaWheel; };

        /// State
        bool IsButtonUp(MouseButtons button) const;
        bool IsButtonDown(MouseButtons button) const;
        bool WasButtonUp(MouseButtons button) const;
        bool WasButtonDown(MouseButtons button) const;
        bool WasButtonPressedThisFrame(MouseButtons button) const;
        bool WasButtonReleasedThisFrame(MouseButtons button) const;
        bool IsButtonHeldDown(MouseButtons button) const;


    protected:

    private:
        GLFWwindow *mWindow;

		MouseState mCurrentState;
		MouseState mLastState;
		double mMouseDeltaX;
		double mMouseDeltaY;
		double mMouseDeltaWheel;
};

#endif // MOUSE_H
