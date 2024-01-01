#ifndef DRAWABLELINES_H
#define DRAWABLELINES_H

#include "Shader.h"

//GLM Math Headers
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


class DrawableLines
{
    public:
        DrawableLines(const std::string &name);
        virtual ~DrawableLines();
        
        void PushLine(const glm::vec3 &pos1, const glm::vec3 &pos2, const  glm::vec3 &color);
		void Update();
        void Draw();

        int GetTotalLines();
        std::string GetName();

        void UpdateLinePosition(int index, const glm::vec3& pos1, const glm::vec3& pos2);
        void RotateLinePosition(int index, const glm::dmat3 &rotation, const glm::dvec3 &rot_pt);
        void TranslateLinePosition(int index, const glm::vec3 &translation);
        /*void PopLine();


        void UpdateLineColor(int index, glm::vec3 color);
        void DeleteLineAt(int index);
*/
    private:
    
		using mLine = 
		struct
		{
			glm::vec3 mPosition1;
			glm::vec3 mPosition2;
			glm::vec3 mColor;
		};
		
        std::vector<mLine*> mLineData;

		std::string mName;
		Shader *mLineShader;
        
};

#endif // DRAWABLELINES_H
