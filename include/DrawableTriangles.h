#ifndef DRAWABLETRIANGLES_H
#define DRAWABLETRIANGLES_H

#include "Shader.h"
#include "DrawableLines.h"

//GLM Math Headers
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


class DrawableTriangles
{
    public:
        DrawableTriangles(const std::string &name);
        virtual ~DrawableTriangles();
		
        void Draw(const glm::vec3 &lightPos, const glm::vec3 &lightColor, const float transparency, bool wireframe=false);

		void SetBuffers(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const glm::vec3 &color, const std::vector<GLuint> &indices);
        //void PushTriangle(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3, const glm::vec3 &normal, const glm::vec3 &color);
		void Update();

        int GetTotalTriangles();
        std::string GetName();

		void RotateTriangles(const glm::mat3 &rotation, const glm::vec3 &rot_pt);
		void TranslateTriangles(const glm::vec3 &translation);

        /*void RotateTrianglePosition(int index, const glm::mat3 &rotation, const glm::vec3 &rot_pt);
        void TranslateTrianglePosition(int index, const glm::vec3 &translation);
        void PopTriangle();


        void UpdateTrianglePosition(int index, const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3);
        void UpdateTriangleColor(int index, const glm::vec3 &color);
        void DeleteTriangleAt(int index);*/
		
    private:
    
		/*using mTriangle = 
		struct
		{
			glm::vec3 mPosition1;
			glm::vec3 mPosition2;
			glm::vec3 mPosition3;
			glm::vec3 mNormal;
			glm::vec3 mColor;
		};
		
        std::vector<mTriangle*> mTriangleData;*/

		std::vector<glm::vec3> mVertices;
		std::vector<glm::vec3> mNormals;
		std::vector<GLuint> mIndices;
		glm::vec3 mColor;

		std::string mName;

		DrawableLines *mWireframe;
		Shader *mTriangleShader;
        
};

#endif // DRAWABLETRIANGLES_H