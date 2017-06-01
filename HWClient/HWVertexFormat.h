#pragma once

#include "HWShaderProgram.h"

#include <string>
#include <vector>

enum class HWVertexComponentType : GLenum
{
	Byte = GL_BYTE,
	UnsignedByte = GL_UNSIGNED_BYTE,
	Short = GL_SHORT,
	UnsignedShort = GL_UNSIGNED_SHORT,
	Int = GL_INT,
	UnsignedInt = GL_UNSIGNED_INT,
	Half = GL_HALF_FLOAT,
	Single = GL_FLOAT,
	Double = GL_DOUBLE,
	Fixed = GL_FIXED,
	Packed = GL_INT_2_10_10_10_REV,
	UnsignedPacked = GL_UNSIGNED_INT_2_10_10_10_REV,
};

class HWVertexComponent
{
public:
	friend class HWVertexFormat;
	HWVertexComponent();
	HWVertexComponent(const HWShaderProgram& program, const std::string& _name, HWVertexComponentType _type, S32 _count);
	HWVertexComponent(const HWVertexComponent& copy);

private:
	HWVertexComponentType type;
	S32 count;
	S32 id;
	S32 size;
};

class HWVertexFormat
{
public:
	HWVertexFormat();
	HWVertexFormat(S32 components);
	HWVertexFormat(const HWVertexFormat& copy);
	~HWVertexFormat();

	void AddComponent(const HWVertexComponent& comp);

	void SetAttributes();
	void DisableAttributes();

	void SetComponentCount(S32 components);

private:
	HWVertexComponent* parts;
	S32 total_size;
	S32 part_count;
	bool is_dirty;
	
	static HWVertexFormat* last_set;

	void UpdateSize();
};
