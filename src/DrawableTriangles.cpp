#include "DrawableTriangles.h"
#include "DirUtils.h"

#include <iostream>

DrawableTriangles::DrawableTriangles()
{
	mTriangleShader = nullptr;

	// Create and compile our GLSL program from the shaders
	mTriangleShader = new Shader(Shader::mObjectToDraw::mTriangle);
	std::cout << "mTriangleShader\n";
	mTriangleShader->LoadShaders(DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/triangle.vertexshader").c_str(), 
								 DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/triangle.fragmentshader").c_str()); 
}

DrawableTriangles::~DrawableTriangles()
{
	mTriangleData.clear();
}

/*void DrawableTriangles::Draw()
{
    mTriangleShader->UseShader();
    mTriangleShader->SetModelviewMatrix();
	mTriangleShader->EnableVertexAttribArrayVertex();
	mTriangleShader->EnableVertexAttribArrayColor();
	mTriangleShader->DrawShader();
	mTriangleShader->DisableVertexAttribArrayVertex();
	mTriangleShader->DisableVertexAttribArrayColor();
	glBindVertexArray(0);
	glUseProgram(0);
}

void DrawableTriangles::PushTriangle(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3, const glm::vec3 &color)
{
	mTriangle *triangle = new mTriangle();
	triangle->mPosition1 = pos1;
	triangle->mPosition2 = pos2;
	triangle->mPosition3 = pos3;
	triangle->mColor = color;

	mTriangleData.push_back(triangle);
	Upload();
}

void DrawableTriangles::PopTriangle()
{
	mTriangleData.pop_back();
	Upload();
}

void DrawableTriangles::Upload()
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
	std::vector<GLushort> indices;

    for(unsigned int i = 0; i < mTriangleData.size(); i++)
    {
    	vertices.push_back(mTriangleData.at(i)->mPosition1);
    	vertices.push_back(mTriangleData.at(i)->mPosition2);
    	vertices.push_back(mTriangleData.at(i)->mPosition3);
    	// One color for each vertex
		colors.push_back(mTriangleData.at(i)->mColor);
		colors.push_back(mTriangleData.at(i)->mColor);
		colors.push_back(mTriangleData.at(i)->mColor);

		indices.push_back(i * 3);
		indices.push_back(i * 3+1);
		indices.push_back(i * 3+2);
    }

	mTriangleShader->SetBuffers(vertices, indices, colors);

	vertices.clear();
	colors.clear();
	indices.clear();
}

int DrawableTriangles::GetTotalTriangles()
{
	return mTriangleData.size();
}

void DrawableTriangles::UpdateTrianglePosition(int index, const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3)
{
	if(index < 0 || index >= mTriangleData.size()) return;
	mTriangleData.at(index)->mPosition1 = pos1;
	mTriangleData.at(index)->mPosition2 = pos2;
	mTriangleData.at(index)->mPosition3 = pos3;
	Upload();
}


void DrawableTriangles::RotateTrianglePosition(int index, const glm::dmat3 &rotation, const glm::dvec3 &rot_pt)
{
	if(index < 0 || index >= mTriangleData.size()) return;
	glm::dvec3 p1 = mTriangleData.at(index)->mPosition1;
	glm::dvec3 p2 = mTriangleData.at(index)->mPosition2;
	glm::dvec3 p3 = mTriangleData.at(index)->mPosition3;
	p1 = rotation*(p1 - rot_pt) + rot_pt;
	p2 = rotation*(p2 - rot_pt) + rot_pt;
	p3 = rotation*(p3 - rot_pt) + rot_pt;
	
	mTriangleData.at(index)->mPosition1 = p1;
	mTriangleData.at(index)->mPosition2 = p2;
	mTriangleData.at(index)->mPosition3 = p3;
	
	//Upload();
}

void DrawableTriangles::TranslateTrianglePosition(int index, const glm::vec3 &translation)
{
	if(index < 0 || index >= mTriangleData.size()) return;
	mTriangleData.at(index)->mPosition1 += translation;
	mTriangleData.at(index)->mPosition2 += translation;
	mTriangleData.at(index)->mPosition3 += translation;
	//Upload();
}

void DrawableTriangles::UpdateTriangleColor(int index, const glm::vec3 &color)
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

