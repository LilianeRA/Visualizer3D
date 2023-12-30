#include "DrawableSpheres.h"
#include "DirUtils.h"
//#include <GL/glu.h>

#include <iostream>


GLenum glCheckError_2(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error{ "None" };
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError2() glCheckError_2(__FILE__, __LINE__) 


DrawableSpheres::DrawableSpheres(int slices, int stacks)
{
	mSlices = slices;
	mStacks = stacks;

	// Create and compile our GLSL program from the shaders
	mSphereShader = new Shader(Shader::mObjectToDraw::mSphere);
	std::cout << "mSphereShader\n";
	mSphereShader->LoadShaders(DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/sphere.vs").c_str(), 
							   DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/sphere.fs").c_str() );


}

DrawableSpheres::~DrawableSpheres()
{
	//mSphereData.clear();
	mPosition.clear();
	mColor.clear();
	mRadius.clear();
}

void DrawableSpheres::Draw()
{
	glCheckError2();
	mSphereShader->DrawShader(&mPosition, &mRadius, &mColor);
	//mSphereShader->DrawShader();
}

void DrawableSpheres::PushSphere(glm::vec3 position, glm::vec3 color, float radius)
{
	/*mSphere *sphere = new mSphere();
	sphere->mPosition = position;
	sphere->mColor = color;
	sphere->mRadius = radius;

	mSphereData.push_back(sphere);*/
	mPosition.push_back(position);
	mColor.push_back(color);
	mRadius.push_back(radius);
}
/*
void DrawableSpheres::PopSphere()
{
	mSphereData.pop_back();
	Upload();
}*/

void DrawableSpheres::Update()
{
	int samples = mStacks;
	int definition = mSlices;
	float radius = 1.0;

	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

	float dArc = glm::pi<float>() / samples;
	for (int i = 0; i < samples + 1; i++)
	{
		float phi = -glm::pi<float>() + i * dArc;
		float h = cos(phi);
		float r = radius * sin(phi);
		for (int j = 0; j < definition + 1; j++)
		{
			float theta = -j * (2.0f * glm::pi<float>() / definition);
			glm::vec3 n(r*std::cos(theta), h, r*std::sin(theta));

			vertices.push_back(n);
			//colors.push_back(glm::dvec3(1.0));
			if (i > 0 && j > 0)
			{
				indices.push_back((i - 1)*(definition + 1) + j - 1);
				indices.push_back((i - 1)*(definition + 1) + j);
				indices.push_back(i*(definition + 1) + j);

				indices.push_back((i - 1)*(definition + 1) + j - 1);
				indices.push_back(i*(definition + 1) + j);
				indices.push_back(i*(definition + 1) + j - 1);
			}
		}
	}

	mNumberOfVerticesPerSphere = vertices.size();

	mSphereShader->SetBuffers(vertices, indices, colors);
	glCheckError2();

    /*std::vector<GLfloat> radius;
    std::vector<glm::vec3> offset;
    std::vector<glm::vec3> colors;

    for(unsigned int i = 0; i < mSphereData.size(); i++)
    {
    	offset.push_back(mSphereData.at(i)->mPosition);
    	radius.push_back(mSphereData.at(i)->mRadius);
		colors.push_back(mSphereData.at(i)->mColor);
    }

	mSphereShader->SetRadius(radius);
	mSphereShader->SetOffset(offset);
	//mSphereShader->SetColors(colors);
	radius.clear();
	colors.clear();
	offset.clear();*/
}
/*
int DrawableSpheres::GetTotalSpheres()
{
	return mSphereData.size();
}

void DrawableSpheres::UpdateSpherePosition(int index, glm::vec3 position)
{
	if(index < 0 || index >= mSphereData.size()) return;
	mSphereData.at(index)->mPosition = position;
	Upload();
}

void DrawableSpheres::RotateSpherePosition(int index, const glm::dmat3 &rotation, const glm::dvec3 &rot_pt)
{
	if(index < 0 || index >= mSphereData.size()) return;
	glm::dvec3 p1 = mSphereData.at(index)->mPosition;
	p1 = rotation*(p1 - rot_pt) + rot_pt;
	
	mSphereData.at(index)->mPosition = p1;
	
	Upload();
}


void DrawableSpheres::TranslateSpherePosition(int index, glm::vec3 position)
{
	if(index < 0 || index >= mSphereData.size()) return;
	mSphereData.at(index)->mPosition += position;
	Upload();
}

void DrawableSpheres::UpdateSphereColor(int index, glm::vec3 color)
{
	if(index < 0 || index >= mSphereData.size()) return;
	mSphereData.at(index)->mColor = color;
	Upload();
}

void DrawableSpheres::DeleteSphereAt(int index)
{
	if(index < 0 || index >= mSphereData.size()) return;
	std::vector<mSphere*>::iterator it = mSphereData.begin()+index;
	mSphereData.erase(it);
	Upload();
}

*/