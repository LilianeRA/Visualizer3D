#include "DrawableTriangles.h"
#include "DirUtils.h"

#include <iostream>

DrawableTriangles::DrawableTriangles()
{
	mTriangleShader = nullptr;

	// Create and compile our GLSL program from the shaders
	mTriangleShader = new Shader(Shader::mObjectToDraw::mTriangle);
	std::cout << "mTriangleShader\n";
	mTriangleShader->LoadShaders(DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/triangle.vs").c_str(), 
								 DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/triangle.fs").c_str()); 
}

DrawableTriangles::~DrawableTriangles()
{
	//mTriangleData.clear();
	mVertices.clear();
	mNormals.clear();
	mIndices.clear();
}

void DrawableTriangles::Draw(const glm::vec3 &lightPos, const glm::vec3 &lightColor)
{
	mTriangleShader->DrawShader(nullptr, nullptr, nullptr, &lightPos, &lightColor);
}


/*void DrawableTriangles::PushTriangle(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3, const glm::vec3 &normal, const glm::vec3 &color)
{
	mTriangle *triangle = new mTriangle();
	triangle->mPosition1 = pos1;
	triangle->mPosition2 = pos2;
	triangle->mPosition3 = pos3;
	triangle->mNormal = normal;
	triangle->mColor = color;

	mTriangleData.push_back(triangle);
}
void DrawableTriangles::PopTriangle()
{
	mTriangleData.pop_back();
	Upload();
}*/

void DrawableTriangles::Update()
{
	std::vector<glm::vec3> colors(mVertices.size(), mColor);
	mTriangleShader->SetBuffers(mVertices, mIndices, &colors, &mNormals);
	colors.clear();
    /*std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<GLuint> indices;

    for(unsigned int i = 0; i < mTriangleData.size(); i++)
    {
    	vertices.push_back(mTriangleData.at(i)->mPosition1);
    	vertices.push_back(mTriangleData.at(i)->mPosition2);
		vertices.push_back(mTriangleData.at(i)->mPosition3);

		normals.push_back(mTriangleData.at(i)->mNormal);
		normals.push_back(mTriangleData.at(i)->mNormal);
		normals.push_back(mTriangleData.at(i)->mNormal);
    	// One color for each vertex
		colors.push_back(mTriangleData.at(i)->mColor);
		colors.push_back(mTriangleData.at(i)->mColor);
		colors.push_back(mTriangleData.at(i)->mColor);

		indices.push_back(i * 3);
		indices.push_back(i * 3+1);
		indices.push_back(i * 3+2);
    }

	mTriangleShader->SetBuffers(vertices, indices, &colors, &normals);

	normals.clear();
	vertices.clear();
	colors.clear();
	indices.clear();*/
}

void DrawableTriangles::SetBuffers(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const glm::vec3 &color, const std::vector<GLuint> &indices)
{
	mVertices.clear();
	mNormals.clear();
	mIndices.clear();
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		mVertices.push_back(vertices.at(i));
		mNormals.push_back(normals.at(i));
	}
	for (const GLuint ind : indices)
	{
		mIndices.push_back(ind);
	}
	mColor = color;

	//Update();
}

int DrawableTriangles::GetTotalTriangles()
{
	return int(mIndices.size()/3);
}

/*
void DrawableTriangles::UpdateTrianglePosition(int index, const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3)
{
	if(index < 0 || index >= mTriangleData.size()) return;
	mTriangleData.at(index)->mPosition1 = pos1;
	mTriangleData.at(index)->mPosition2 = pos2;
	mTriangleData.at(index)->mPosition3 = pos3;
	Upload();
}*/


void DrawableTriangles::RotateTrianglePosition(int index, const glm::mat3 &rotation, const glm::vec3 &rot_pt)
{
	if(index < 0 || index >= GetTotalTriangles()) return;
	int ind = index * 3;
	glm::vec3 p1 = mVertices.at(mIndices.at(ind  ));
	glm::vec3 p2 = mVertices.at(mIndices.at(ind+1));
	glm::vec3 p3 = mVertices.at(mIndices.at(ind+2));
	p1 = rotation*(p1 - rot_pt) + rot_pt;
	p2 = rotation*(p2 - rot_pt) + rot_pt;
	p3 = rotation*(p3 - rot_pt) + rot_pt;
	
	mVertices.at(mIndices.at(ind  )) = p1;
	mVertices.at(mIndices.at(ind+1)) = p2;
	mVertices.at(mIndices.at(ind+2)) = p3;
}

void DrawableTriangles::RotateTriangles(const glm::mat3 &rotation, const glm::vec3 &rot_pt)
{
	for (unsigned int i = 0; i < mVertices.size(); i++)
	{
		mVertices.at(i) = rotation * (mVertices.at(i) - rot_pt) + rot_pt;
	}
	/*glm::dvec3 p1 = mTriangleData.at(index)->mPosition1;
	glm::dvec3 p2 = mTriangleData.at(index)->mPosition2;
	glm::dvec3 p3 = mTriangleData.at(index)->mPosition3;
	p1 = rotation * (p1 - rot_pt) + rot_pt;
	p2 = rotation * (p2 - rot_pt) + rot_pt;
	p3 = rotation * (p3 - rot_pt) + rot_pt;

	mTriangleData.at(index)->mPosition1 = p1;
	mTriangleData.at(index)->mPosition2 = p2;
	mTriangleData.at(index)->mPosition3 = p3;*/

	//Upload();
}

void DrawableTriangles::TranslateTriangles(const glm::vec3 &translation)
{
	for (unsigned int i = 0; i < mVertices.size(); i++)
	{
		mVertices.at(i) += translation;
	}
}

void DrawableTriangles::TranslateTrianglePosition(int index, const glm::vec3 &translation)
{
	if (index < 0 || index >= GetTotalTriangles()) return;
	int ind = index * 3;
	mVertices.at(mIndices.at(ind  )) += translation;
	mVertices.at(mIndices.at(ind+1)) += translation;
	mVertices.at(mIndices.at(ind+2)) += translation;
}
/*void DrawableTriangles::UpdateTriangleColor(int index, const glm::vec3 &color)
{
	if(index < 0 || index >= mTriangleData.size()) return;
	mTriangleData.at(index)->mColor = color;
	Upload();
}

void DrawableTriangles::DeleteTriangleAt(int index)
{
	if(index < 0 || index >= mTriangleData.size()) return;
	std::vector<mTriangle*>::iterator it = mTriangleData.begin()+index;
	mTriangleData.erase(it);
	Upload();
}*/

