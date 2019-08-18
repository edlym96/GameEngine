#pragma once
#include <string>
#include <GL/glew.h>
#include <fstream>
#include <vector>
namespace DawnEngine {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath); //Read vertex and fragment shader from source and compile
		void linkShaders(); //link the shaders into final program
		void addAttribute(const std::string& attributeName); //abind attributes from shader language to the program
		GLint getUniformLocation(const std::string& uniformName);
		void use();
		void unuse();
	private:
		void compileShader(const std::string& filePath, GLuint ID);

		int m_numAttributes;
		GLuint m_programID; //ID that refers to shader program
		GLuint m_vertexShaderID; //ID for vertex shader
		GLuint m_fragmentShaderID; //ID to reference fragment shader
	};

}