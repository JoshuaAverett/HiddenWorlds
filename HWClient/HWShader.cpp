#include "pch.h"
#include "HWShader.h"
#include "../HWLib/HWFile.h"

HWShader::HWShader()
{

}

HWShader::HWShader(HWShaderType _type)
{
	type = _type;
	shader = glCreateShader((GLenum)type);
}

HWShader::~HWShader()
{
}

bool HWShader::ReadSource(const std::string& path)
{
	HWFile file = HWFile(path, HWFileOperation::Read);
	source = file.Read();
	is_dirty = true;

	return true;
}

bool HWShader::Compile()
{
	if(is_dirty)
	{
		const GLchar* buffer = source.c_str();
		glShaderSource(shader, 1, &buffer, nullptr);

		glCompileShader(shader);

		GLint result = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

		if(!result)
		{
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
			GLchar* buffer = new GLchar[result];
			glGetShaderInfoLog(shader, result, &result, buffer);
			return false;
		}

		is_dirty = false;
	}

	return true;
}

std::string HWShader::GetSource()
{
	return source;
}

void HWShader::SetSource(const std::string& _source)
{
	source = _source;
	is_dirty = true;
}
