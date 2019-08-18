#include "GLSLProgram.h"
#include "Errors.h"

namespace DawnEngine {
	GLSLProgram::GLSLProgram() : m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		// Vertex and fragment shaders are succesfully compiled
		// Link them together into a program.
		// Get program object
		m_programID = glCreateProgram();

		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) {
			//Print error
			fatalError("Vertex shader failed to be created");
		}

		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) {
			//Print error
			fatalError("Fragment shader failed to be created");
		}

		compileShader(vertexShaderFilePath, m_vertexShaderID);
		compileShader(fragmentShaderFilePath, m_fragmentShaderID);
	}


	void GLSLProgram::linkShaders() {

		//Atach shaders to the program
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		//Link the program
		glLinkProgram(m_programID);

		//Error checking
		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == 0) {
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//maxLenght includes NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			//Delete Program and shaders
			glDeleteProgram(m_programID);
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader failed to link");

			return;
		}

		//Detach shaders after succesful link
		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}


	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}

	void GLSLProgram::use() {
		glUseProgram(m_programID);
		for (int i = 0; i < m_numAttributes; ++i) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		for (int i = 0; i < m_numAttributes; ++i) {
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