// Author:					Todd Hamilton
// Last Change:				01/05/16
// Description of File:		Shader class

#pragma once

#include <vector>

// Static Shader class, creates and returns shader handle, Destroy shader manually after use!
class Shader
{
public:
	static unsigned int CreateShader(const char* a_vertexShaderPath = "", const char* a_fragmentShaderPath = "", const char* a_geometryShaderPath = "", const std::vector<const char*> a_varyings = std::vector<const char*>());

private:
	static void ReadShaderFile(const char* a_filePath);
	static char* shaderData;
};