#include "Camera3D.h"
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
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

	mScale = 2.0f; // ortho

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

void Camera3D::Pan(const glm::vec2 &pan)
{
    glm::vec3 distance = (mPosition - mFocus);
    glm::vec3 translation = (pan.x * mRight) + (-pan.y * mUp);
    translation *= sqrt(distance.x*distance.x + distance.y*distance.y + distance.z*distance.z) * 10.0f * mPanSensibility + 0.001f;
    mPosition += translation;
    mFocus += translation;
    mCameraHasChanged = true;
}

void Camera3D::Orbit(const glm::vec2 &delta, const glm::vec2 &mousepos)
{
    /*glm::vec3 distance = mPosition - mFocus;

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
    mUp = cross(mRight, mDirection);*/

	glm::quat pitch_quat = glm::angleAxis(delta.y*mOrbitSensibility/3.0f, glm::vec3(1, 0, 0));
	glm::quat heading_quat = glm::angleAxis(delta.x*mOrbitSensibility/3.0f, glm::vec3(0, 1, 0));
	glm::quat qRoll;
	if(mousepos.x > mScreenWidth/2.0)
		qRoll = glm::angleAxis(-delta.y*mOrbitSensibility / 3.0f, glm::vec3(0, 0, 1));
	else
		qRoll = glm::angleAxis(delta.y*mOrbitSensibility / 3.0f, glm::vec3(0, 0, 1));
	//add the two quaternions
	glm::quat temp = glm::cross(pitch_quat, heading_quat);
	temp = glm::cross(qRoll, temp);
	temp = glm::normalize(temp);
	//update the direction from the quaternion
	mDirection = glm::rotate(temp, mDirection);
	mRight = glm::rotate(temp, mRight);
	mUp = glm::rotate(temp, mUp);
	mPosition = -glm::normalize(mDirection) * glm::length(mPosition - mFocus) + mFocus;

    mCameraHasChanged = true;
}

void Camera3D::SetOrthographic(bool use_ortho)
{
	if(use_ortho != mUseOrtho)
		mCameraHasChanged = true;
	mUseOrtho = use_ortho;

	if (mUseOrtho)
		mPanSensibility = 5.0f;
	else
		mPanSensibility = 0.1f;
}

void Camera3D::Zoom(float times)
{
	if (!mUseOrtho)
	{
		glm::vec3 distance = mPosition - mFocus;
		distance *= (1.0f + times * mZoomSensibility);
		mPosition = mFocus + distance;
	}
	else
	{
		mScale -= (times * mZoomSensibility);
	}

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
        Orbit(glm::vec2(-deltaX / mScreenWidth, -deltaY / mScreenWidth), glm::vec2(io.MousePos.x, io.MousePos.y));
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
	if (mUseOrtho)
	{
		/*glm::mat4 m(1.0f);
		glm::mat4 t = glm::translate(glm::mat4(1.0f), -mPosition);*/
		glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(mScale));

		mViewMatrix = glm::lookAt(mPosition, mFocus, mUp);
		mViewMatrix = s * mViewMatrix;
	}
	else
	    mViewMatrix = glm::lookAt(mPosition, mFocus, mUp);
}

void Camera3D::UpdateProjectionMatrix()
{
    /// perspectiveFov-> glm/gtc/matrix_transform.hpp
	if(mUseOrtho)
		mProjectionMatrix = glm::ortho(0.0f, mScreenWidth, 0.0f, mScreenHeight, -5000.0f, 5000.f);
	else
		if(mScreenWidth > 0 && mScreenHeight > 0) // Assertion failed: width > static_cast<T>(0), file d:\programmingprojects\glm-0.9.9.8\glm\ext\matrix_clip_space.inl, line 374
			mProjectionMatrix = glm::perspectiveFov(mFieldOfView, mScreenWidth, mScreenHeight, mNearPlaneDistance, mFarPlaneDistance);
}

void Camera3D::UpdateViewProjectionMatrix()
{
    glm::mat4 viewMat(mViewMatrix);
    glm::mat4 projectionMat(mProjectionMatrix);

    mViewProjectionMatrix = (projectionMat * viewMat);
}




