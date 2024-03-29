#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>
#include "Mouse.h"
#include "WindowGLFW.h"
//#include "Shader.h"
#include <iostream>
#include <sstream>
#include <vector>


static WindowGLFW *mStaticWindow = nullptr;

WindowGLFW::WindowGLFW(bool bidimensional, const std::string &title, int width, int height)
{
    //ctor
    mWindowInitialized = false;
    mBidimensional = bidimensional;
	mMoveCamera = true;
    mScreenWidth = width;
    mScreenHeight = height;
    mWindowTitle = title;
    mWindow = nullptr;
    mStaticWindow = this;
    mTotalTime = 0.0;
    mElapsedTime = 0.0;
    mCamera3D = nullptr;
    /*mCamera2D = nullptr;
    mMouse = nullptr;*/
	mLightSphere = nullptr;
	mAxis = nullptr;

	mLightPos = glm::vec3{ 102.0f, 100.0f, 200.0f };
	mLightColor = glm::vec3{ 1.0f, 1.0f, 1.0f };

    nada = true;

    if(!mBidimensional)
    {
        mCamera3D = new Camera3D(mScreenWidth, mScreenHeight);
        //mCamera3D->SetPosition(10,15,20);
        mCamera3D->LookAt(0, 0, 0);
    }
    else
    {
        /////mCamera2D = new Camera2D(mScreenWidth, mScreenHeight);
        //mCamera2D->SetPosition(-10.0f, mScreenHeight/2 -10.0f);
        //mCamera2D->LookAt(10, 10);
    }
	
}

WindowGLFW::~WindowGLFW()
{
    //dtor
	//Shutdown();
    Exit();
}

/// TIME
void WindowGLFW::ResetTime()
{
    GetTime(mStartTime);
    mCurrentTime = mStartTime;
    mLastTime = mCurrentTime;

    mTotalTime = 0.0;
    mElapsedTime = 0.0;
}

void WindowGLFW::GetTime(timePoint& time) const
{
    time = std::chrono::high_resolution_clock::now();
}

void WindowGLFW::InitializeWindow()
{
    if(mWindowInitialized) return;
    if (!glfwInit())
    {
        std::cout<<"glfwInit() failed.\n";
        return;
    }
    else
    {
        std::cout<<"GLFW3 initialied.\n";
    }

	// Decide GL+GLSL versions
/*#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);          // 3.0+ only
#endif*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // GLFW_OPENGL_CORE_PROFILE thinks glPushMatrix is deprecated.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // to use legacy functions of OpenGL
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mWindowTitle.c_str(), nullptr, nullptr);
    if (mWindow == nullptr)
    {
        glfwTerminate();
        std::cout<<"glfwCreateWindow() failed.\n";
        return;
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

    /*glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        glfwDestroyWindow(mWindow);
        cout<<"glewInit() failed.\n";
        return;
    }
    else
    {
        cout<<"GLEW initialized.\n";
    }*/

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glClearColor(0.95, 0.95, 0.95, 1.0f); // will possibly be changed by ImGUI

    //Callback de teclas
    glfwSetCursorPosCallback(mWindow, MouseCallback);
    glfwSetWindowSizeCallback(mWindow, ResizeCallback);
    glfwSetKeyCallback(mWindow, KeyboardCallback);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	const char* glsl_version = "#version 460";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();


    //mMouse = new Mouse(mWindow);
    mWindowInitialized = true;
}

void WindowGLFW::InitializeAxisShaders()
{
	float length = 100.0f;
	std::cout << "mDrawableLines (axis)\n";
	mAxis = new DrawableLines("Axis");

	mAxis->PushLine(glm::vec3(0.0, 0.0, 0.0), glm::vec3(length, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0));
	mAxis->PushLine(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, length, 0.0), glm::vec3(0.0, 1.0, 0.0));
	mAxis->PushLine(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, length), glm::vec3(0.0, 0.0, 1.0));
	mAxis->Update();
	// Create and compile our GLSL program from the shaders
	/*mAxisShader = new Shader(Shader::mObjectToDraw::mLine);
	mAxisShader->LoadShaders("../shaders/axis.vertexshader", "../shaders/axis.fragmentshader" );

	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
	vertices.push_back(glm::vec3(length, 0.0, 0.0));
	vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
	vertices.push_back(glm::vec3(0.0, length, 0.0));
	vertices.push_back(glm::vec3(0.0, 0.0, 0.0));
	vertices.push_back(glm::vec3(0.0, 0.0, length));

	std::vector<glm::vec3> colors;
	colors.push_back(glm::vec3(1.0, 0.0, 0.0));
	colors.push_back(glm::vec3(1.0, 0.0, 0.0));
	colors.push_back(glm::vec3(0.0, 1.0, 0.0));
	colors.push_back(glm::vec3(0.0, 1.0, 0.0));
	colors.push_back(glm::vec3(0.0, 0.0, 1.0));
	colors.push_back(glm::vec3(0.0, 0.0, 1.0));

	mAxisShader->SetVertices(vertices);
	mAxisShader->SetColors(colors);*/
}

void WindowGLFW::InitializeSpheresShaders()
{
	mLightSphere = new DrawableSpheres("Light Spheres");
	mLightSphere->PushSphere(mLightPos, mLightColor, 2.0);
}

/*void WindowGLFW::AddCheckbox(const std::string &title, bool value)
{
    if (mCheckboxList.size() == 0)
    {
        mCheckboxList.push_back(std::pair<const std::string, bool*>(title, &value) );
    }
    else
    {
        auto checkbox = std::find_if(mCheckboxList.begin(), mCheckboxList.end(),
            [&title](const std::pair<const std::string, bool> &p)
            {
                if (title.compare(p.first) == 0) return true;
                return false;
            });
        if (checkbox == std::end(mCheckboxList)) // not found
        {
            mCheckboxList.push_back(std::pair<const std::string, bool>(title, value));
        }
    }
}*/


void WindowGLFW::SetCustomWindow()
{
}
void WindowGLFW::CustomDraw()
{
}
void WindowGLFW::CustomShutdown()
{
}

/*void WindowGLFW::ShowBasicInfo(ImVec4 &clear_color, bool &ortho_proj)
{
    
    static int counter = 0;
    ImGui::Text("Light configurations");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

    //ImGui::SliderFloat("Grid Transparency", &mGridTransparency, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat("Envelope Transparency", &mEnvelopeTransparency, 0.0f, 1.0f);
    //ImGui::Checkbox("Envelope Lines", &mEnvelopeWireframe);
    // custom checkboxes
    for (auto& c : mCheckboxList)
    {
        ImGui::Checkbox(c.first.c_str(), &c.second);
    }

    if (ImGui::Button("Reset"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);


}*/


/// EXECUTION
void WindowGLFW::Run()
{
    if(!mWindowInitialized) 
    {
        std::cout<<"Error: window not initialized. Call InitializeWindow() before calling Run().\n";
        return;
    }
    //InitializeWindow();
    ResetTime();
    InitializeAxisShaders();
    InitializeSpheresShaders();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool show_demo_window = true;
	bool ortho_proj = false;
	mMoveCamera = false;
    while (!glfwWindowShouldClose(mWindow))
    {
		//glCheckError(); // INVALID_ENUM ???
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        mMoveCamera = !ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
            ImGui::SetNextWindowPos(ImVec2(0, 0));

            ImGui::Begin("Basic Configuration");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Checkbox("Orthographic Projection", &ortho_proj);
            ImGui::ColorEdit3("Clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Text("Light configurations");               // Display some text (you can use a format strings too)
            ImGui::SliderFloat3("Light Pos", &mLightPos.x, -200.0f, 200.0f);
            ImGui::ColorEdit3("Light Color", (float*)&mLightColor); // Edit 3 floats representing a color

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
		}
		{
            SetCustomWindow();
		}
		mCamera3D->SetOrthographic(ortho_proj);

		ImGui::Render();
		glfwMakeContextCurrent(mWindow);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		Update(); 

        Draw();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // to draw the ImGUI windows

		glfwMakeContextCurrent(mWindow);
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
    }

    Shutdown();
}

void WindowGLFW::Draw()
{
    //glm::vec3 mCameraScale(0.1f, 0.1f, 0.1f);
    glm::vec3 mCameraScale(1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    if(!mBidimensional)
    {
        //glLoadMatrixf(&mCamera3D->ViewProjectionMatrix()[0][0]);
        glLoadMatrixf(glm::value_ptr(mCamera3D->ViewProjectionMatrix()));
        glScalef(mCameraScale[0], mCameraScale[1], mCameraScale[2]);
    }
    else
    {
        //glLoadMatrixf(&mCamera2D->ViewProjectionMatrix()[0][0]);
    }

    /*glBegin(GL_LINES);
    /// X axis
    glColor3f(1.0f, 0.0f, 0.0f);
   	glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(100, 0.0, 0.0);
    /// Y axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 100, 0.0);
    /// Z axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 100);
    glEnd();*/


    /*mAxisShader->UseShader();
    mAxisShader->SetModelviewMatrix();
	mAxisShader->EnableVertexAttribArrayVertex();
	mAxisShader->EnableVertexAttribArrayColor();
	mAxisShader->DrawShader();
	mAxisShader->DisableVertexAttribArrayVertex();
	mAxisShader->DisableVertexAttribArrayColor();*/

	mAxis->Draw();
	mLightSphere->Draw(mLightPos, mLightColor);
	
    CustomDraw();


    if(nada)
    {
        std::cout<<"GL Modelview"<<std::endl;
        GLfloat viewmat[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, viewmat);
        for(int i = 0; i < 4; i++)
            std::cout<<viewmat[i*4]<<" "<<viewmat[4*i+1]<<" "<<viewmat[4*i+2]<<" "<<viewmat[4*i+3]<<std::endl;
        std::cout<<std::endl;
        std::cout<<"GL Projection"<<std::endl;
        glGetFloatv(GL_PROJECTION_MATRIX, viewmat);
        for(int i = 0; i < 4; i++)
            std::cout<<viewmat[i*4]<<" "<<viewmat[4*i+1]<<" "<<viewmat[4*i+2]<<" "<<viewmat[4*i+3]<<std::endl;
        std::cout<<std::endl;
        std::cout<<"GL Modelview Projection"<<std::endl;
        glGetFloatv(GL_MODELVIEW_PROJECTION_NV, viewmat);
        for(int i = 0; i < 4; i++)
            std::cout<<viewmat[i*4]<<" "<<viewmat[4*i+1]<<" "<<viewmat[4*i+2]<<" "<<viewmat[4*i+3]<<std::endl;
        std::cout<<std::endl;
        nada = false;
    }
    glPopMatrix();

    glLineWidth(1.0f);
    //glEnable(GL_DEPTH_TEST);

}

void WindowGLFW::Update()
{
    GetTime(mCurrentTime);
    std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>> (mCurrentTime - mLastTime);
	std::chrono::duration<double> total = std::chrono::duration_cast<std::chrono::duration<double>> (mCurrentTime - mStartTime);

    mTotalTime = total.count();
    mElapsedTime = elapsed.count();
    mLastTime = mCurrentTime;

    static double time = 0.0;
    static int frames = 0;

    frames++;
    time += mElapsedTime;
    if (time > 1)
    {
	    std::stringstream ss;
	    ss << mWindowTitle << " - fps:" << frames << " - " << (1000.0 / (double)frames) << " ms";
        glfwSetWindowTitle(mWindow, ss.str().c_str());

        frames = 0;
        time = 0;
    }

	if(!mBidimensional)
	{
		mCamera3D->Update();
	}
	/*else
	{
		mCamera2D->Update();
	}*/
	mLightSphere->UpdateSpherePosition(0, mLightPos);
}

/// CALLBACK
bool WindowGLFW::IsKeyDown(int key) const
{
	return glfwGetKey(mWindow, key) == GLFW_PRESS;
}

void WindowGLFW::Input(int key, int scanCode, int action, int mod)
{
    if (IsKeyDown(GLFW_KEY_ESCAPE))
    {
        Exit();
    }
}

void WindowGLFW::MouseUpdate()
{
    if(!mBidimensional)
    {
        mCamera3D->MouseUpdate(mMoveCamera);
    }
    /*else
    {
        mCamera2D->MouseUpdate(mMouse);
    }*/
}

void WindowGLFW::Resize(int width, int height)
{
    glViewport(0, 0, width, height);
    if(!mBidimensional)
    {
        mCamera3D->UpdateScreenSize(width, height);
    }
    else
    {
        //mCamera2D->UpdateScreenSize(mScreenWidth, mScreenHeight);
    }
}

void WindowGLFW::ResizeCallback(GLFWwindow* window, int width, int height)
{
    mStaticWindow->mScreenWidth = width;
    mStaticWindow->mScreenHeight = height;
    mStaticWindow->Resize(width, height);
}

void WindowGLFW::KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mod)
{
    mStaticWindow->Input(key, scanCode, action, mod);
}

void WindowGLFW::MouseCallback(GLFWwindow * window, double x, double y)
{
    mStaticWindow->MouseUpdate();
}



/// TERMINATE
void WindowGLFW::Shutdown()
{
    if (mWindow != nullptr)
    {
        glfwDestroyWindow(mWindow);
    }
    glfwTerminate();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Cleanup VBO and shader
	delete mLightSphere;
	delete mAxis;

    CustomShutdown();
}

void WindowGLFW::Exit()
{
    glfwSetWindowShouldClose(mWindow, 1);
}
