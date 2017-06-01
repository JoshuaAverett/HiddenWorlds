#pragma once

#include "HWShader.h"

#include <vector>
#include "../glm/glm.hpp"

class HWShaderProgram
{
public:
	HWShaderProgram();
	HWShaderProgram(const HWShaderProgram&);
	~HWShaderProgram();

	void AddShader(HWShader& shader);
	bool CompileAndLink();
	void UseProgram() const;

	GLuint GetUniformID(const char* name) const;
	GLuint GetAttribID(const char* name) const;

	void SetUniformValue(const char* name, glm::mat4 value) const;
	void SetUniformValue(const char* name, glm::vec2 value) const;

private:
	std::vector<HWShader*> shaders;
	GLuint program;
	bool is_dirty;
};

