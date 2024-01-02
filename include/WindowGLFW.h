#ifndef WINDOWGLFW_H
#define WINDOWGLFW_H

#include "DrawableSpheres.h"
#include "DrawableLines.h"
#include "DrawableTriangles.h"
#include "Camera3D.h"
//#include "Camera2D.h"
#include <string>
#include <chrono>


typedef std::chrono::high_resolution_clock::time_point timePoint;

class WindowGLFW
{
    public:
        WindowGLFW(bool bidimensional, const std::string &title, int width = 1080, int height = 720);
        virtual ~WindowGLFW();

        void InitializeWindow();
		//void AddCheckbox(const std::string& title, bool value);

        void Run();


    protected:
        virtual void SetCustomWindow();
        virtual void CustomDraw();
        virtual void CustomShutdown();

        // std::vector<std::pair<const std::string, bool>> mCheckboxList;
		glm::vec3 mLightPos;
		glm::vec3 mLightColor;
    private:
        bool mWindowInitialized;
        bool mBidimensional; // 2D or 3D visualization
		bool mMoveCamera;
        Camera3D *mCamera3D;
        //Camera2D *mCamera2D;

        bool nada;

        /// Windows attributes
        GLFWwindow *mWindow;
        std::string mWindowTitle;
        int mScreenWidth;
        int mScreenHeight;

        Mouse *mMouse;

        /// Time attributes
		timePoint mStartTime;
		timePoint mCurrentTime;
		timePoint mLastTime;
		double mTotalTime;
        double mElapsedTime;

        /// Shader attributes
		DrawableLines *mAxis;
        DrawableSpheres *mLightSphere;

        

        void Draw();
        void Update();
        void Shutdown();
        void Exit(); 

        void ResetTime();
        void GetTime(timePoint& time) const;

        void Resize(int width, int height);
        void Input(int key, int scanCode, int action, int mod);
        bool IsKeyDown(int key) const;
        void MouseUpdate();
        static void KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mod);
        static void ResizeCallback(GLFWwindow* window, int width, int height);
        static void MouseCallback(GLFWwindow* window, double x, double y);

        void InitializeAxisShaders();
        void InitializeSpheresShaders();
};

#endif // WINDOWGLFW_H
