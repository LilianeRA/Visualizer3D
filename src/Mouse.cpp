#include "Mouse.h"
#include <string.h> // memcpy

Mouse::Mouse(GLFWwindow *window) : mWindow(window)
{
    //ctor
}

Mouse::~Mouse()
{
    //dtor
}

void Mouse::Update()
{
    memcpy(&mLastState, &mCurrentState, sizeof(mCurrentState));

    /*for (int i = 0; i < 4; i++)
    {
        mCurrentState.buttons[i] = glfwGetMouseButton(mWindow, i);
    }
    glfwGetCursorPos(mWindow, &mCurrentState.x, &mCurrentState.y);

    mMouseDeltaX = mCurrentState.x - mLastState.x;
    mMouseDeltaY = mCurrentState.y - mLastState.y;
    mMouseDeltaWheel = mCurrentState.scroll - mLastState.scroll;*/
}

bool Mouse::IsButtonUp(MouseButtons button) const
{
    return mCurrentState.buttons[button] == GLFW_RELEASE;
}

bool Mouse::IsButtonDown(MouseButtons button) const
{
    return mCurrentState.buttons[button] != GLFW_RELEASE;
}

bool Mouse::WasButtonUp(MouseButtons button) const
{
    return mLastState.buttons[button] == GLFW_RELEASE;
}

bool Mouse::WasButtonDown(MouseButtons button) const
{
    return mLastState.buttons[button] != GLFW_RELEASE;
}

bool Mouse::WasButtonPressedThisFrame(MouseButtons button) const
{
    return WasButtonUp(button) && IsButtonDown(button);
}

bool Mouse::WasButtonReleasedThisFrame(MouseButtons button) const
{
    return WasButtonDown(button) && IsButtonUp(button);
}

bool Mouse::IsButtonHeldDown(MouseButtons button) const
{
    return WasButtonDown(button) && IsButtonDown(button);
}


