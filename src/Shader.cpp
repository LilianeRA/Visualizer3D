#include "Shader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>



GLenum glCheckError_(const char *file, int line)
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
#define glCheckError() glCheckError_(__FILE__, __LINE__) 



Shader::Shader(mObjectToDraw mode) :
	mProgramID(INT_MAX), mVertexArray(INT_MAX), mVertexBuffer(INT_MAX), mColorBuffer(INT_MAX), mNormalBuffer(INT_MAX), mIndexBuffer(INT_MAX),
	mPerFrameDataBuffer(INT_MAX), mMatrixID(INT_MAX), mVertexPositionID(INT_MAX), mVertexColorID(INT_MAX), mVertexNormalID(INT_MAX),
	mMode(mode), mNumberOfVertices(0)
{
	//ctor
}

Shader::~Shader()
{
	//dtor
	CleanUp();
}


bool Shader::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	std::cout << "Loading shaders...\n";

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	auto read_shader_lines = [](const std::string &shader_path, bool &file_ok)  -> std::string 
	{
		std::string shaderCode;
		std::ifstream shaderStream(shader_path, std::ios::in);
		if (shaderStream.is_open()) {
			file_ok = true;
			std::string Line = "";
			while (getline(shaderStream, Line))
				shaderCode += "\n" + Line;
			shaderStream.close();
		}
		else {
			file_ok = false;
			std::cout << "Impossible to open " << shader_path << ". Are you in the right directory?\n";
			getchar();
			return "";
		}
		return shaderCode;
	};

	// Read the Vertex Shader code from the file
	bool file_ok = false;
	std::string VertexShaderCode = read_shader_lines(vertex_file_path, file_ok);
	if (!file_ok)
	{
		std::cout << "Vertex shader error\n";
		return false;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = read_shader_lines(fragment_file_path, file_ok);
	if (!file_ok)
	{
		std::cout << "Fragment shader error\n";
		return false;
	}


	auto compile_shader = [](const char *sourcePointer, GLuint shaderID) -> void
	{
		GLint Result = GL_FALSE;
		int InfoLogLength;

		glShaderSource(shaderID, 1, &sourcePointer, NULL);
		glCompileShader(shaderID);

		// Check Vertex Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> shaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
			std::cout << &shaderErrorMessage[0] <<std::endl;
			std::cout << Result <<std::endl;
		}
	};
	// Compile Vertex Shader
	std::cout << "Compiling shader : " << vertex_file_path << "\n";
	compile_shader(VertexShaderCode.c_str(), VertexShaderID);

	// Compile Fragment Shader
	std::cout << "Compiling shader : "<< fragment_file_path<<"\n";
	compile_shader(FragmentShaderCode.c_str(), FragmentShaderID);

	// Link the program
	std::cout << "Linking program\n";
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, VertexShaderID);
	glAttachShader(mProgramID, FragmentShaderID);
	glLinkProgram(mProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(mProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << "Program error: "<< ProgramErrorMessage[0] << std::endl;
	}


	glDetachShader(mProgramID, VertexShaderID);
	glDetachShader(mProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	std::cout << "mProgramID " << mProgramID << std::endl;
	glCheckError();
	//return ProgramID;
	return true;
}

void Shader::SetBuffers(const std::vector<glm::vec3> &vertices, const std::vector<GLuint> &indices, 
	const std::vector<glm::vec3> *colors, const std::vector<glm::vec3> *normals)
{
	glUseProgram(mProgramID);
	glCheckError();

	glCreateVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);
	glCheckError();

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glCheckError();

	if (colors)
	{
		glGenBuffers(1, &mColorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*colors->size(), colors->data(), GL_STATIC_DRAW);
		glCheckError();
	}
	if (normals)
	{
		glGenBuffers(1, &mNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals->size(), normals->data(), GL_STATIC_DRAW);
		glCheckError();
	}

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
	glCheckError();

	glCreateBuffers(1, &mPerFrameDataBuffer);
	glNamedBufferStorage(mPerFrameDataBuffer, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, mPerFrameDataBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mPerFrameDataBuffer);
	glCheckError();


	mMatrixID = mGetUniformLocation("MVP"); // glGetUniformLocation(mProgramID, "MVP");
	mVertexPositionID = glGetAttribLocation(mProgramID, "vPos"); // 0;  
	if (colors)
	{
		mVertexColorID = glGetAttribLocation(mProgramID, "vColor"); // 1; 
	}
	if (normals)
	{
		mVertexNormalID = glGetAttribLocation(mProgramID, "vNormal"); 
	}
	glCheckError();

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	glUseProgram(0);
	glCheckError();

	mNumberOfIndices = indices.size();
	mNumberOfVertices = vertices.size();
	/*std::cout << "mVertexPositionID " << mVertexPositionID << std::endl;
	std::cout << "mVertexColorID    " << mVertexColorID << std::endl;*/
	std::cout << "mNumberOfVertices " << mNumberOfVertices << std::endl;
	std::cout << "mNumberOfIndices  " << mNumberOfIndices << std::endl;

	/*//UseShader();
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	glGenVertexArrays(1, &mVertexArray); // VAO
	glGenBuffers(1, &mVertexBuffer);     // VBO
	//glGenBuffers(1, &mIndexBuffer);		 // EBO
	glGenBuffers(1, &mColorBuffer);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(mVertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	mVertexPosition_modelspaceID = glGetAttribLocation(mProgramID, "vertexPosition_modelspace");
	std::cout << "mVertexPosition_modelspaceID " << mVertexPosition_modelspaceID << std::endl;
	glVertexAttribPointer(
		mVertexPosition_modelspaceID,
		3,
		GL_FLOAT,
		GL_FALSE,
		(0),
		(void*)0
	);
	glEnableVertexAttribArray(mVertexPosition_modelspaceID);
	mVertexColorID = glGetAttribLocation(mProgramID, "vertexColor");
	std::cout << "mVertexColorID " << mVertexColorID << std::endl;
	glVertexAttribPointer(
		mVertexColorID,
		3,
		GL_FLOAT,
		GL_FALSE,
		(0),
		(void*)0
	);
	glEnableVertexAttribArray(mVertexColorID);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	glUseProgram(0);

	//mNumberOfIndices = indices.size();
	mNumberOfVertices = vertices.size();
	std::cout<<"mNumberOfVertices "<<mNumberOfVertices<<std::endl;
	std::cout << "mNumberOfIndices " << mNumberOfIndices << std::endl;
	std::cout << "colors " << colors.size() << std::endl;

	mColors.clear();
	mColors = colors;*/
}

void Shader::mSetUniform1f(const std::string &name, float value)
{
	GLint location = mGetUniformLocation(name);
	glCheckError();
	glUniform1f(location, value);
	glCheckError();
}

void Shader::mSetUniformVec3(const std::string &name, const glm::vec3 &value)
{
	GLint location = mGetUniformLocation(name);
	glCheckError();
	glUniform3f(location, value.x, value.y, value.z);
	glCheckError();
}

GLint Shader::mGetUniformLocation(const std::string &name) const
{
	//std::cout << name<< std::endl;
	if (mUniformCache.find(name) == mUniformCache.end())
	{
		mUniformCache[name] = glGetUniformLocation(mProgramID, name.c_str());
	}
	return mUniformCache[name];
}

/*void Shader::SetColors(std::vector<glm::vec3> &colors)
{
	UseShader();
	glGenBuffers(1, &mColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

	mVertexColorID = glGetAttribLocation(mProgramID, "vertexColor");

	glVertexAttribPointer(
		mVertexColorID,
		3,
		GL_FLOAT,
		GL_FALSE,
		(0),
		(void*)0
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	mColors.clear();
	mColors = colors;
	colors.clear();
}*/


/*void Shader::SetRadius(std::vector<GLfloat> &radius)
{
	mRadius.clear();
	mRadius = radius;
	radius.clear();
}*/

/*void Shader::SetOffset(std::vector<glm::vec3> &offset)
{
	mOffset.clear();
	mOffset = offset;
	offset.clear();
}*/

/*void Shader::UseShader()
{
	glUseProgram(mProgramID);
}*/

void Shader::SetModelviewMatrix()
{
	GLfloat modelview[16];
	//glm::mat4 modelview;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glUniformMatrix4fv(mMatrixID, 1, GL_FALSE, modelview);
	glCheckError();
	
	glNamedBufferSubData(mPerFrameDataBuffer, 0, sizeof(glm::mat4),modelview);
	glCheckError();
}

void Shader::DrawShader(const std::vector<glm::vec3> *offset, const std::vector<GLfloat> *radius, const std::vector<glm::vec3> *colors,
	const glm::vec3 *lightPos, const glm::vec3 *lightColor)
{
	glCheckError();
	glUseProgram(mProgramID);
	glCheckError();
	glBindVertexArray(mVertexArray); // to prevent the error: Invalid VAO/VBO/pointer usage
	glCheckError();

	SetModelviewMatrix();
	glCheckError();

	glEnableVertexAttribArray(mVertexPositionID); 
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glVertexAttribPointer(mVertexPositionID, 3, GL_FLOAT, GL_FALSE, (0), (void*)0);
	glCheckError();


	if (mMode == mObjectToDraw::mLine)
	{
		glEnableVertexAttribArray(mVertexColorID); 
		glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer); 
		glVertexAttribPointer(mVertexColorID, 3, GL_FLOAT, GL_FALSE, (0), (void*)0);
		glCheckError();
		glDrawElements(GL_LINES, mNumberOfIndices, GL_UNSIGNED_INT, 0);
		glCheckError();
		//glDrawArrays(GL_LINES, 0, mNumberOfVertices);
	}
	glCheckError();
	if (mMode == mObjectToDraw::mSphere)
	{
		glEnableVertexAttribArray(mVertexNormalID);
		glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
		glVertexAttribPointer(mVertexNormalID, 3, GL_FLOAT, GL_FALSE, (0), (void*)0);
		glCheckError();
		mSetUniformVec3("u_lightPos", *lightPos);
		mSetUniformVec3("u_lightColor", *lightColor);
		for (unsigned int i = 0; i < radius->size(); i++)
		{
			mSetUniformVec3("u_color", colors->at(i));
			mSetUniformVec3("u_offset", offset->at(i));
			mSetUniform1f("u_radius", radius->at(i));
			glDrawElements(GL_TRIANGLES, mNumberOfIndices, GL_UNSIGNED_INT, 0);
		}
		glCheckError();
	}
	glCheckError();
	//glDisableVertexAttribArray(mVertexPositionID);
	//glDisableVertexAttribArray(mVertexColorID);
	glUseProgram(0);


	/*glBindVertexArray(mVertexArray);
	glEnableVertexAttribArray(mVertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	if (mMode == mObjectToDraw::mLine)
		glDrawArrays(GL_LINES, 0, mNumberOfVertices);
	if (mMode == mObjectToDraw::mSphere)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		//glDrawArrays(GL_LINE_STRIP, 0, mNumberOfVertices); // 12*3 indices starting at 0 -> 12 triangles
		GLuint mRadiusUniform = glGetUniformLocation(mProgramID, "radius");
		GLuint mOffsetUniform = glGetUniformLocation(mProgramID, "offset");
		GLuint mColorUniform = glGetUniformLocation(mProgramID, "vertexColor");
		for (unsigned int i = 0; i < mRadius.size(); i++)
		{
			glUniform3f(mOffsetUniform, mOffset.at(i)[0], mOffset.at(i)[1], mOffset.at(i)[2]);
			glUniform3f(mColorUniform, mColors.at(i)[0], mColors.at(i)[1], mColors.at(i)[2]);
			glUniform1f(mRadiusUniform, mRadius.at(i));
			glDrawElements(GL_TRIANGLES, mNumberOfIndices, GL_UNSIGNED_SHORT, 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	if (mMode == mObjectToDraw::mTriangle)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		for (unsigned int i = 0; i < 1; i++)
		{
			glDrawElements(GL_TRIANGLES, mNumberOfIndices, GL_UNSIGNED_SHORT, 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	UseShader();
	SetModelviewMatrix();
	EnableVertexAttribArrayVertex();
	glDrawArrays(GL_LINES, 0, mNumberOfVertices);
	DisableVertexAttribArrayVertex();*/
	
}

/*void Shader::DisableVertexAttribArrayVertex()
{
	glDisableVertexAttribArray(mVertexPosition_modelspaceID);
}

void Shader::DisableVertexAttribArrayColor()
{
	glDisableVertexAttribArray(mVertexColorID);
}

void Shader::EnableVertexAttribArrayVertex()
{
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(mVertexPosition_modelspaceID);
	glBindVertexArray(mVertexArray);
}

void Shader::EnableVertexAttribArrayColor()
{
	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(mVertexColorID);
}*/
void Shader::CleanUp()
{
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mColorBuffer);
	glDeleteBuffers(1, &mNormalBuffer);
	glDeleteBuffers(1, &mPerFrameDataBuffer);
	glDeleteProgram(mProgramID);
}




