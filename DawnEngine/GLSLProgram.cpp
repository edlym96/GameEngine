#include "GLSLProgram.h"
#include "Errors.h"

namespace DawnEngine {
	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		// Vertex and fragment shaders are succesfully compiled
		// Link them together into a program.
		// Get program object
		_programID = glCreateProgram();

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0) {
			//Print error
			fatalError("Vertex shader failed to be created");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0) {
			//Print error
			fatalError("Fragment shader failed to be created");
		}

		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}


	void GLSLProgram::linkShaders() {

		//Atach shaders to the program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link the program
		glLinkProgram(_programID);

		//Error checking
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == 0) {
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//maxLenght includes NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			//Delete Program and shaders
			glDeleteProgram(_programID);
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader failed to link");

			return;
		}

		//Detach shaders after succesful link
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}


	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}

	void GLSLProgram::use() {
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; ++i) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; ++i) {
			glDisableVertexAttribArray(i);
		}
	};

	void GLSLProgram::compileShader(const std::string& filePath, GLuint ID) {
		std::ifstream file(filePath); //Same as creating ifstream and calling .open()
		if (file.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(file, line)) {
			fileContents += line + "\n";
		}
		file.close();

		const char* contentPtr = fileContents.c_str();
		glShaderSource(ID, 1, &contentPtr, nullptr);
		glCompileShader(ID);

		GLint success = 0;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &maxLength); //length of log of gl

			//The maxlength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(ID, maxLength, &maxLength, &errorLog[0]);

			//Provide the infolog in whatever manner you deem best
			//Exit with failure
			glDeleteShader(ID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");
		}
	}
}