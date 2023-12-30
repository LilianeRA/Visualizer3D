#ifndef DRAWABLESPHERES_H
#define DRAWABLESPHERES_H

#include "Shader.h"
//GLM Math Headers
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


class DrawableSpheres
{
    public:
        DrawableSpheres(int slices = 6, int stacks = 4);
        ~DrawableSpheres();
        
        void Draw(const glm::vec3 &lightPos, const glm::vec3 &lightColor);

        void PushSphere(glm::vec3 position, glm::vec3 color, float radius);
        /*void PopSphere();

        int GetTotalSpheres();

        void UpdateSpherePosition(int index, glm::vec3 position);
        void RotateSpherePosition(int index, const glm::dmat3 &rotation, const glm::dvec3 &rot_pt);
        void TranslateSpherePosition(int index, glm::vec3 position);
        void UpdateSphereColor(int index, glm::vec3 color);
        void DeleteSphereAt(int index);*/
       	void Update();

    private:
    
		/*using mSphere = 
		struct
		{
			glm::vec3 mPosition;
			glm::vec3 mColor;
			float mRadius;
		};
		
        std::vector<mSphere*> mSphereData;*/
        std::vector<glm::vec3> mColor;
        std::vector<glm::vec3> mPosition;
        std::vector<GLfloat> mRadius;

		Shader *mSphereShader;

        int mSlices;
        int mStacks;
		unsigned int mNumberOfVerticesPerSphere;

		//void mCreateSphere(float radius, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &colors, std::vector<GLuint> &indices);
        
};

#endif // DRAWABLESPHERES_H
