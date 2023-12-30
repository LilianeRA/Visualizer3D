#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

//GLM Math Headers
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

/*#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>*/

class Shader
{
public:
	enum class mObjectToDraw { mLine, mSphere, mTriangle };

	Shader(mObjectToDraw mode);
	virtual ~Shader();

	bool LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	void SetBuffers(const std::vector<glm::vec3> &vertices, const std::vector<GLuint> &indices, const std::vector<glm::vec3> &colors);


	void DrawShader(const std::vector<glm::vec3> *offset = nullptr, 
					const std::vector<GLfloat> *radius = nullptr, 
					const std::vector<glm::vec3> *colors = nullptr);
	/*void SetRadius(std::vector<GLfloat> &radius);
	void SetOffset(std::vector<glm::vec3> &offset);
	void UseShader();
	void EnableVertexAttribArrayVertex();
	void EnableVertexAttribArrayColor();
	void DisableVertexAttribArrayVertex();
	void DisableVertexAttribArrayColor();
	*/

protected:

private:
	/// Buffers
	GLuint mProgramID;
	GLuint mVertexArray;
	GLuint mVertexBuffer;
	GLuint mColorBuffer;
	GLuint mIndexBuffer;
	GLuint mPerFrameDataBuffer;
	// Shader variables
	GLuint mMatrixID;
	GLuint mVertexPositionID;
	GLuint mVertexColorID;

	mObjectToDraw mMode;
	unsigned int mNumberOfVertices;
	unsigned int mNumberOfIndices;
	/*std::vector<GLfloat> mRadius;
	std::vector<glm::vec3> mOffset;
	std::vector<glm::vec3> mColors;*/

	mutable std::unordered_map<std::string, GLint> mUniformCache;

	void mSetUniform1f(const std::string &name, float value);
	void mSetUniformVec3(const std::string &name, const glm::vec3 &value);
	GLint mGetUniformLocation(const std::string &name) const;

	void SetModelviewMatrix();
	void CleanUp();
};

#endif // SHADER_H
