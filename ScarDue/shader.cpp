#include <iostream>
#include<sstream>
#include<fstream>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
using namespace std;

class shader {
public:
	unsigned int ID;

	shader(const string& vertexPath, const string& fragmentPath);
	void Use() const;

	void SetBool(const string& name, bool value) const;
	void SetInt(const string& name, int value) const;
	void SetFloat(const string& name, float value) const;
	void SetVec3(const string& name, const glm::vec3& value) const;
	void SetMat4(const string& name, const glm::mat4& mat) const;

	~shader();

};

shader::shader(const string& vertexPath, const string& fragmentPath)
{
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}

	catch (ifstream::failure& e)
	{
		cerr << "Error: file: can't read file successfully" << endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	glGetShader4(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog)
			cerr << "Error: shader: vertex: compilation failed" << infoLog << endl;
	}

	//fragment shader
	fragment = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);

	glGetShader4(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog)
			cerr << "Error: shader: vertex: compilation failed\n" << infoLog << endl;
	}

	ID = glCreateProgramme();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgramme(ID);
	glGetProgramme4(ID, GL_LINK_STATUS, &success);
	if (!succes) {
		glGetProgrammeInfoLog(ID, 512, nullptr, infoLog);
		cerr << "Error: shader: programme: linking failed\n" << infoLog << endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void shader::Use() const
{
	glUseProgramme(ID);
}

void shader::SetBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void shader::SetInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::SetFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::SetVec3(const string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void shader::SetMat4(const string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

shader::~shader()
{
	glDeleteProgramme(ID);
}
