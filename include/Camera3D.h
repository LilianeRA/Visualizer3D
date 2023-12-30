#ifndef CAMERA3D_H
#define CAMERA3D_H
#include "Mouse.h"

//GLM Math Headers
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


class Camera3D
{
    public:

    public:
        Camera3D(float width, float height, float fieldOfView = glm::quarter_pi<float>(),
                 float nearPlaneDistance = 0.001f, float farPlaneDistance = 1000.0f);
        virtual ~Camera3D();

        void Reset();
        void Update();
        void UpdateScreenSize(float width, float height);
        void LookAt(float x, float y, float z);
        void SetPosition(float x, float y, float z);
        void MouseUpdate(bool update);

        glm::mat4 ViewProjectionMatrix() const;
		void Pan(glm::vec2 pan);
		void Orbit(glm::vec2 delta);

    protected:

    private:
        float mScreenWidth;
        float mScreenHeight;
		float mFieldOfView;
		float mAspectRatio;
		float mNearPlaneDistance;
		float mFarPlaneDistance;
        float mZoomSensibility;
        float mPanSensibility;
        float mOrbitSensibility;

		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec3 mRight;
		glm::vec3 mFocus;
		glm::vec3 mUp;

		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewProjectionMatrix;

		bool mCameraHasChanged;

        /// Movement
        void Zoom(float times);

		/// updates
        void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateViewProjectionMatrix();
};

#endif // CAMERA3D_H
