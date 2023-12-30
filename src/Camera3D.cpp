#include "Camera3D.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>

Camera3D::Camera3D(float width, float height, float fieldOfView,
                   float nearPlaneDistance, float farPlaneDistance) :
    mScreenWidth(width), mScreenHeight(height),
    mFieldOfView(fieldOfView), mAspectRatio(0.0),
    mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance),
    mZoomSensibility(5.0f), mPanSensibility(0.1f), mOrbitSensibility(10.0f),
    mCameraHasChanged(false)
{
    //ctor
    Reset();
}

Camera3D::~Camera3D()
{
    //dtor
}

void Camera3D::Reset()
{
    mPosition = glm::vec3(0.0f, 0.0f, 10.0f);
    mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mFocus = glm::vec3(0.0f, 0.0f, 0.0f);
    mRight = glm::vec3(1.0f, 0.0f, 0.0f);
    mUp = glm::vec3(0.0f, 1.0f, 0.0f);

    mCameraHasChanged = true;
}

void Camera3D::SetPosition(float x, float y, float z)
{
    mPosition = glm::vec3(x, y, z);
    mCameraHasChanged = true;
}

void Camera3D::LookAt(float x, float y, float z)
{
    mFocus = glm::vec3(x, y, z);
    mCameraHasChanged = true;
}

void Camera3D::Pan(glm::vec2 pan)
{
    glm::vec3 distance = (mPosition - mFocus);
    glm::vec3 translation = (pan.x * mRight) + (-pan.y * mUp);
    translation *= sqrt(distance.x*distance.x + distance.y*distance.y + distance.z*distance.z) * 10.0f * mPanSensibility + 0.001f;
    mPosition += translation;
    mFocus += translation;
    mCameraHasChanged = true;
}

void Camera3D::Orbit(glm::vec2 delta)
{
    glm::vec3 distance = mPosition - mFocus;

    float theta = atan2( distance.x, distance.z );
    float phi = atan2( sqrt( distance.x * distance.x + distance.z * distance.z ), distance.y );

    delta *= mOrbitSensibility;
    theta += delta.x;
    phi += delta.y;

    float EPS = 0.000001f;

    phi = glm::max( EPS, glm::min( glm::pi<float>() - EPS, phi ) );

    float radius = sqrt(distance.x*distance.x + distance.y*distance.y + distance.z*distance.z);

    distance.x = radius * sin( phi ) * sin( theta );
    distance.y = radius * cos( phi );
    distance.z = radius * sin( phi ) * cos( theta );

    mPosition = mFocus + distance;

    mDirection = normalize(-distance);
    mRight = normalize(cross(mDirection, glm::vec3(0, 1, 0)));
    mUp = cross(mRight, mDirection);

    mCameraHasChanged = true;
}

void Camera3D::Zoom(float times)
{
    glm::vec3 distance = mPosition - mFocus;
    distance *= (1.0f + times * mZoomSensibility);
    mPosition = mFocus + distance;

    mCameraHasChanged = true;
}


/// Getters
glm::mat4 Camera3D::ViewProjectionMatrix() const
{
    return mViewProjectionMatrix;
}

/// UPDATES
void Camera3D::UpdateScreenSize(float width, float height)
{
    mScreenWidth = width;
    mScreenHeight = height;
    mAspectRatio = mScreenWidth/mScreenHeight;
    mCameraHasChanged = true;
}

void Camera3D::MouseUpdate(bool update)
{
	if (!update) return;
    //mMouse->Update();

	ImGuiKey mouseLeft{ 641 };
	ImGuiKey mouseRight{ 642 };
	ImGuiKey mouseMiddle{ 643 };
	ImGuiKey mouseWheel{ 647 };


	ImGuiIO& io = ImGui::GetIO(); 
	double deltaX = io.MouseDelta.x; //mMouse->MouseDeltaX();
	double deltaY = io.MouseDelta.y; //mMouse->MouseDeltaY();

    //if(mMouse->IsButtonHeldDown(Mouse::MouseButtonsRight) &&
    //   mMouse->IsButtonHeldDown(Mouse::MouseButtonsLeft))
	if (ImGui::IsKeyDown(mouseRight))
    {
        Pan(glm::vec2(-deltaX / mScreenWidth, -deltaY / mScreenWidth));
    }
    else if(ImGui::IsKeyDown(mouseLeft))
    {
        Orbit(glm::vec2(-deltaX / mScreenWidth, -deltaY / mScreenWidth));
	std::cout << deltaX << " " << deltaY << std::endl;
    }
    else if(ImGui::IsKeyDown(mouseMiddle))
    {
        Zoom(-deltaX / mScreenWidth);
    }
}

void Camera3D::Update()
{
    if(mCameraHasChanged)
    {
        UpdateViewMatrix();
        UpdateProjectionMatrix();
        UpdateViewProjectionMatrix();
        mCameraHasChanged = false;
    }
}

void Camera3D::UpdateViewMatrix()
{
    mViewMatrix = lookAt(mPosition, mFocus, mUp);
}

void Camera3D::UpdateProjectionMatrix()
{
    /// perspectiveFov-> glm/gtc/matrix_transform.hpp
    mProjectionMatrix = glm::perspectiveFov(mFieldOfView, mScreenWidth, mScreenHeight, mNearPlaneDistance, mFarPlaneDistance);
}

void Camera3D::UpdateViewProjectionMatrix()
{
    glm::mat4 viewMat(mViewMatrix);
    glm::mat4 projectionMat(mProjectionMatrix);

    mViewProjectionMatrix = (projectionMat * viewMat);
}




