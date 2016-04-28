#include "Shader.h"

#include "gl_core_4_4.h"

char* Shader::shaderData = nullptr;

unsigned int Shader::CreateShader(const char* a_vertexShaderPath, const char* a_fragmentShaderPath, const char* a_geometryShaderPath, const std::vector<const char*> a_varyings)
{
	int success = GL_FALSE;
	int programID = glCreateProgram();

	if (a_varyings.size() != 0)
		glTransformFeedbackVaryings(programID, a_varyings.size(), &a_varyings[0], GL_INTERLEAVED_ATTRIBS);

	bool usingVertexShader = a_vertexShaderPath[0] != '\0';
	bool usingFragmentShader = a_fragmentShaderPath[0] != '\0';
	bool usingGeometryShader = a_geometryShaderPath[0] != '\0';

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int geometryShader;

	// Read, compile and attach vertex shader
	if (usingVertexShader)
	{
		ReadShaderFile(a_vertexShaderPath);
		const char* vsSource = shaderData;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
		glCompileShader(vertexShader);
		glAttachShader(programID, vertexShader);

		int infoLogLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(vertexShader, infoLogLength, 0, infoLog);
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	// Read, compile and attach fragment shader
	if (usingFragmentShader)
	{
		ReadShaderFile(a_fragmentShaderPath);
		const char* fsSource = shaderData;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
		glCompileShader(fragmentShader);
		glAttachShader(programID, fragmentShader);

		int infoLogLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(fragmentShader, infoLogLength, 0, infoLog);
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	// Read, compile and attach geometry shader
	if (usingGeometryShader)
	{
		ReadShaderFile(a_geometryShaderPath);
		const char* fsSource = shaderData;
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, (const char**)&fsSource, 0);
		glCompileShader(geometryShader);
		glAttachShader(programID, geometryShader);

		int infoLogLength = 0;
		glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(geometryShader, infoLogLength, 0, infoLog);
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	// Link the shaders together to create program
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	// Check for success - Debug log on fail
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	// Delete shaders
	if (usingVertexShader)
		glDeleteShader(vertexShader);
	if (usingFragmentShader)
		glDeleteShader(fragmentShader);
	if (usingGeometryShader)
		glDeleteShader(fragmentShader);

	return programID;
}

void Shader::ReadShaderFile(const char* a_filePath)
{
	if (shaderData != nullptr)
	{
		delete[] shaderData;
		shaderData = nullptr;
	}

	FILE* file = fopen(a_filePath, "rb");
	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	shaderData = new char[size + 1];
	fread(shaderData, 1, size, file);
	shaderData[size] = 0;
	fclose(file);
}