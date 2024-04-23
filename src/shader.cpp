#include <shader.h>

#include<fstream>
#include<sstream>
#include <iostream>
#include <stdexcept>
#include <utility>

std::vector<std::string> Shader::typeNames
{
	"Vertex",
	"Fragment",
	"Geometry",
	"Program"
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = GetShaderCode(vertexPath);
	std::string fragmentCode = GetShaderCode(fragmentPath);

	unsigned int vertex = CreateShader(Type::Vertex, std::move(vertexCode));
	unsigned int fragment = CreateShader(Type::Fragment, std::move(fragmentCode));

	ID = CreateProgram({ vertex, fragment }, true);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::SetValue(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetValue(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetValue(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetValue(const std::string& name, std::vector<float> vec)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1], vec[2], vec[3]);
}


bool Shader::IsCompileSuccess(unsigned int shader, Type type)
{
	int success;
	if (type == Type::Program)
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	}

	return static_cast<bool>(success);
}

std::string Shader::GetInfoLog(unsigned int shader, Type type)
{
	char infoLog[1024];
	if (type == Type::Program)
	{
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
	}
	else
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
	}

	return std::string(infoLog);
}

std::string Shader::GetShaderCode(const char* codePath)
{
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	shaderFile.open(codePath);
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	return shaderStream.str();
}

int Shader::GetShaderName(Type type)
{
	int shaderName;
	switch (type)
	{
	case Type::Vertex:
		shaderName = GL_VERTEX_SHADER;
		break;
	case Type::Fragment:
		shaderName = GL_FRAGMENT_SHADER;
		break;
	case Type::Geometry:
		shaderName = GL_GEOMETRY_SHADER;
		break;
	default:
		std::stringstream errorStream;
		errorStream << "Unknown shader type" << static_cast<uint8_t>(type) << std::endl;
		throw std::runtime_error(errorStream.str());
		break;
	}
	return shaderName;
}

unsigned int Shader::CreateShader(Type type, std::string code)
{
	unsigned int shader = glCreateShader(GetShaderName(type));
	const char* shaderCode = code.c_str();

	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	CheckCompileErrors(shader, type);

	return shader;
}

unsigned int Shader::CreateProgram(std::vector<unsigned int> shaders, bool bDeleteShader)
{
	unsigned int program = glCreateProgram();

	for (unsigned int shader : shaders)
	{
		glAttachShader(program, shader);
	}

	glLinkProgram(program);
	CheckCompileErrors(program, Type::Program);

	if (bDeleteShader)
	{
		for (unsigned int shader : shaders)
		{
			glDeleteShader(shader);
		}
	}

	return program;
}

void Shader::CheckCompileErrors(unsigned int shader, Type type)
{
	if (!IsCompileSuccess(shader, type))
	{
		std::stringstream errorStream;
		errorStream << "ERROR::"
			<< (type == Type::Program ? "PROGRAM_LINKING_ERROR" : "SHADER_COMPILATION_ERROR") << " of type: "
			<< typeNames[static_cast<uint8_t>(type)] << "\n"
			<< GetInfoLog(shader, type)
			<< "\n -- --------------------------------------------------- -- "
			<< std::endl;

		throw std::runtime_error(errorStream.str());
	}
}