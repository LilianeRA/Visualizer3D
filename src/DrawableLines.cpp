#include "DrawableLines.h"
#include "DirUtils.h"
//#include <GL/glu.h>

#include <iostream>

DrawableLines::DrawableLines(const std::string &name)
{
	// Create and compile our GLSL program from the shaders
	mLineShader = new Shader(Shader::mObjectToDraw::mLine);
	std::cout << "mLineShader\n";
	mLineShader->LoadShaders(DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/line.vs").c_str(), 
							 DirUtils::m_JoinPaths(DirUtils::m_GetCurrentDir(), "../../shaders/line.fs").c_str());
	mName = name;
}

DrawableLines::~DrawableLines()
{
	mLineData.clear();
}

void DrawableLines::Draw()
{
	mLineShader->DrawShader();
}


void DrawableLines::PushLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 color)
{
	mLine *line = new mLine();
	line->mPosition1 = pos1;
	line->mPosition2 = pos2;
	line->mColor = color;

	mLineData.push_back(line);
}

/*void DrawableLines::PopLine()
{
	mLineData.pop_back();
	//Upload();
}*/

void DrawableLines::Update()
{
    std::vector<GLuint> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;

    for(unsigned int i = 0; i < mLineData.size(); i++)
    {
		vertices.push_back(mLineData.at(i)->mPosition1);
		vertices.push_back(mLineData.at(i)->mPosition2);
		indices.push_back(2*i);
		indices.push_back(2*i+1);
		//std::cout << i << " " << 2 * i << " " << 2 * i + 1 << std::endl;
    	// One color for each vertex
		colors.push_back(mLineData.at(i)->mColor);
		colors.push_back(mLineData.at(i)->mColor);
    }

	mLineShader->SetBuffers(vertices, indices, &colors);

	vertices.clear();
	indices.clear();
	colors.clear();
}

int DrawableLines::GetTotalLines()
{
	return mLineData.size();
}

std::string DrawableLines::GetName()
{
	return mName;
}
void DrawableLines::UpdateLinePosition(int index, glm::vec3 pos1, glm::vec3 pos2)
{
	if(index < 0 || index >= mLineData.size()) return;
	mLineData.at(index)->mPosition1 = pos1;
	mLineData.at(index)->mPosition2 = pos2;
}


void DrawableLines::RotateLinePosition(int index, const glm::dmat3 &rotation, const glm::dvec3 &rot_pt)
{
	if(index < 0 || index >= mLineData.size()) return;
	glm::dvec3 p1 = mLineData.at(index)->mPosition1;
	glm::dvec3 p2 = mLineData.at(index)->mPosition2;
	p1 = rotation*(p1 - rot_pt) + rot_pt;
	p2 = rotation*(p2 - rot_pt) + rot_pt;
	
	mLineData.at(index)->mPosition1 = p1;
	mLineData.at(index)->mPosition2 = p2;
}

void DrawableLines::TranslateLinePosition(int index, const glm::vec3 &translation)
{
	if(index < 0 || index >= mLineData.size()) return;
	mLineData.at(index)->mPosition1 += translation;
	mLineData.at(index)->mPosition2 += translation;
}
/*
void DrawableLines::UpdateLineColor(int index, glm::vec3 color)
{
	if(index < 0 || index >= mLineData.size()) return;
	mLineData.at(index)->mColor = color;
	//Upload();
}

void DrawableLines::DeleteLineAt(int index)
{
	if(index < 0 || index >= mLineData.size()) return;
	std::vector<mLine*>::iterator it = mLineData.begin()+index;
	mLineData.erase(it);
	//Upload();
}*/

