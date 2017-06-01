#pragma once

#include "HWGL.h"
#include <string>

enum class HWShaderType
{
	VertexShader = GL_VERTEX_SHADER,
	FragmentShader = GL_FRAGMENT_SHADER,
	TesselationControlShader = GL_TESS_CONTROL_SHADER,
	TesselationEvaluationShader = GL_TESS_EVALUATION_SHADER,
	GeometryShader = GL_GEOMETRY_SHADER,
	ComputeShader = GL_COMPUTE_SHADER,
};

class HWShader
{
public:
	friend class HWShaderProgram;
	HWShader();
	HWShader(HWShaderType _type);
	HWShader(const HWShader&);
	~HWShader();

	bool ReadSource(const std::string& path);
	bool Compile();

	std::string GetSource();
	void SetSource(const std::string& _source);

private:
	std::string source;
	HWShaderType type;
	GLuint shader;
	bool is_dirty;
};

