#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void Use();
	void Delete();

	void SetValue(const std::string& name, bool value);
	void SetValue(const std::string& name, int value);
	void SetValue(const std::string& name, float value);
	void SetValue(const std::string& name, std::vector<float> vec);

private:
	enum class Type : uint8_t
	{
		Vertex = 0,
		Fragment,
		Geometry,
		Program,
		Count
	};

	static std::vector<std::string> typeNames;

	static void CheckCompileErrors(unsigned int shader, Type type);
	static bool IsCompileSuccess(unsigned int shader, Type type);
	static int GetShaderName(Type type);
	static unsigned int CreateShader(Type type, std::string code);
	static unsigned int CreateProgram(std::vector<unsigned int> shaders, bool bDeleteShader);
	static std::string GetInfoLog(unsigned int shader, Type type);
	static std::string GetShaderCode(const char* codePath);
};