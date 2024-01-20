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


DrawableSpheres::DrawableSpheres(const std::string &name, int slices, int stacks) :
	mSlices(slices), mStacks(stacks), mNumberOfVerticesPerSphere(0)
{
	// Create and compile our GLSL program from the shaders
	mSphereShader = new Shader(Shader::mObjectToDraw::mSphere);
	std::cout << "mSphereShader\n";
	mSphereShader->LoadShaders(DirUtils::JoinPaths(DirUtils::GetMainDir(), "shaders/sphere.vs").c_str(),
							   DirUtils::JoinPaths(DirUtils::GetMainDir(), "shaders/sphere.fs").c_str() );
	mName = name;
	SetBuffers();
}

DrawableSpheres::~DrawableSpheres()
{
	//mSphereData.clear();
	mPosition.clear();
	mColor.clear();
	mRadius.clear();
}

void DrawableSpheres::Draw(const glm::vec3 &lightPos, const glm::vec3 &lightColor)
{
	glCheckError2();
	mSphereShader->DrawShader(&mPosition, &mRadius, &mColor, &lightPos, &lightColor);
	//mSphereShader->DrawShader();
}

void DrawableSpheres::PushSphere(const glm::vec3 &position, const glm::vec3 &color, float radius)
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

void DrawableSpheres::SetBuffers()
{
	int samples = mStacks;
	int definition = mSlices;
	float radius = 1.0;

	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
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
			normals.push_back(glm::normalize(n));
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

	mSphereShader->SetBuffers(vertices, indices, &colors, &normals);
	glCheckError2();

	colors.clear();
	vertices.clear();
	normals.clear();
	indices.clear();
}

/*void DrawableSpheres::Update()
{
    std::vector<GLfloat> radius;
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
	offset.clear();
}*/

int DrawableSpheres::GetTotalSpheres()
{
	return mPosition.size();
}

std::string DrawableSpheres::GetName()
{
	return mName;
}

/*void DrawableSpheres::ApplyTransformation(const glm::dmat3& transformation)
{
	glm::vec3 centroid = ComputeCentroid();
	for (const glm::dvec3 &pos : mPosition)
	{
		glm::dvec3 newpos = pos;
		newpos -= centroid;
		newpos = transformation * newpos;
		newpos += centroid;
	}
}*/

glm::vec3 DrawableSpheres::ComputeCentroid() const
{
	glm::vec3 centroid(0.0, 0.0, 0.0);

	for (const auto& p : mPosition)
	{
		centroid += p;
	}
	centroid /= (double)mPosition.size();

	return centroid;
}

DrawableSpheres* DrawableSpheres::Copy() const
{
	DrawableSpheres *copyDrawableSpheres = new DrawableSpheres(mName);

	for (int i = 0; i < mPosition.size(); ++i)
	{
		copyDrawableSpheres->PushSphere(mPosition.at(i), mColor.at(i), mRadius.at(i));
	}

	return copyDrawableSpheres;
}

void DrawableSpheres::UpdateSpherePosition(int index, const  glm::vec3 &position)
{
	if(index < 0 || index >= mPosition.size()) return;
	mPosition.at(index) = position;
}

void DrawableSpheres::SetSphereColor(int index, const glm::vec3& color)
{
	if (index < 0 || index >= mPosition.size()) return;
	mColor.at(index) = color;
}

void DrawableSpheres::RotateSpherePosition(int index, const glm::mat3 &rotation, const glm::vec3 &rot_pt)
{
	if(index < 0 || index >= mPosition.size()) return;
	glm::vec3 p1 = mPosition.at(index);
	p1 = rotation*(p1 - rot_pt) + rot_pt;
	
	mPosition.at(index) = p1;
}


void DrawableSpheres::TranslateSpherePosition(int index, const glm::vec3 &position)
{
	if(index < 0 || index >= mPosition.size()) return;
	mPosition.at(index) += position;
}
/*
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