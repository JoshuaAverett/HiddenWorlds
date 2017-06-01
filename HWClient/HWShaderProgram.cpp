#include "pch.h"
#include "HWShaderProgram.h"

HWShaderProgram::HWShaderProgram()
{
	program = glCreateProgram();
}

HWShaderProgram::~HWShaderProgram()
{
	glDeleteProgram(program);
}

void HWShaderProgram::AddShader(HWShader& shader)
{
	shaders.push_back(&shader);
}

bool HWShaderProgram::CompileAndLink()
{
	std::vector<HWShader*>::iterator iter = shaders.begin();
	for(; iter != shaders.end(); ++iter)
	{
		HWShader* shader = *iter;
		if(!shader->Compile())
		{
			return false;
		}

		glAttachShader(program, shader->shader);
	}

	glLinkProgram(program);

	GLint result = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if(!result)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
		GLchar* buffer = new GLchar[result];
		glGetProgramInfoLog(program, result, &result, buffer);
		return false;
	}

	return true;
}

void HWShaderProgram::UseProgram() const
{
	glUseProgram(program);
}

GLuint HWShaderProgram::GetUniformID(const char* name) const
{
	return glGetUniformLocation(program, name);
}

GLuint HWShaderProgram::GetAttribID(const char* name) const
{
	return glGetAttribLocation(program, name);
}

void HWShaderProgram::SetUniformValue(const char* name, glm::mat4 value) const
{
	glUniformMatrix4fv(GetUniformID(name), 1, GL_FALSE, &value[0][0]);
}

void HWShaderProgram::SetUniformValue(const char* name, glm::vec2 value) const
{
	glUniform2fv(GetUniformID(name), 1, &value[0]);
}