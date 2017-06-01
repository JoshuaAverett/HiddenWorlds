#include "pch.h"
#include "HWVertexFormat.h"

HWVertexFormat* HWVertexFormat::last_set = nullptr;

HWVertexComponent::HWVertexComponent()
{
}

HWVertexComponent::HWVertexComponent(const HWShaderProgram& program, const std::string& name, HWVertexComponentType _type, S32 _count)
{
	type = _type;
	count = _count;
	id = program.GetAttribID(name.c_str());
	switch(type)
	{
	case HWVertexComponentType::Byte:
	case HWVertexComponentType::UnsignedByte:
		size = 1;
		break;

	case HWVertexComponentType::Short:
	case HWVertexComponentType::UnsignedShort:
	case HWVertexComponentType::Half:
		size = 2;
		break;

	case HWVertexComponentType::Int:
	case HWVertexComponentType::UnsignedInt:
	case HWVertexComponentType::Fixed:
	case HWVertexComponentType::Packed:
	case HWVertexComponentType::UnsignedPacked:
	case HWVertexComponentType::Single:
		size = 4;
		break;

	case HWVertexComponentType::Double:
		size = 8;
		break;
	}
	size *= count;
}

HWVertexComponent::HWVertexComponent(const HWVertexComponent& copy)
{
	count = copy.count;
	id = copy.id;
	size = copy.size;
	type = copy.type;

}

HWVertexFormat::HWVertexFormat()
{
	is_dirty = true;
}

HWVertexFormat::HWVertexFormat(S32 components)
{
	SetComponentCount(components);
}

HWVertexFormat::HWVertexFormat(const HWVertexFormat& copy)
{
	is_dirty = copy.is_dirty;
	part_count = copy.part_count;
	total_size = copy.total_size;
	parts = new HWVertexComponent[part_count];
	memcpy(parts, copy.parts, part_count * sizeof(HWVertexComponent));
}

HWVertexFormat::~HWVertexFormat()
{
	//delete[] parts;
}

void HWVertexFormat::AddComponent(const HWVertexComponent& comp)
{
	parts[comp.id] = comp;
	is_dirty = true;
}

void HWVertexFormat::SetAttributes()
{
	if(last_set != this)
	{
		UpdateSize();

		S32 offset = 0;
		for(S32 i = 0; i < part_count; ++i)
		{
			HWVertexComponent comp = parts[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, comp.count, (GLenum)comp.type, GL_FALSE, total_size, (void*)offset);
			offset += comp.size;
		}

		last_set = this;
	}
}

void HWVertexFormat::DisableAttributes()
{
	for(S32 i = 0; i < part_count; ++i)
	{
		glEnableVertexAttribArray(i);
	}
}

void HWVertexFormat::UpdateSize()
{
	if(is_dirty)
	{
		total_size = 0;
		for(S32 i = 0; i < part_count; ++i)
		{
			HWVertexComponent comp = parts[i];
			total_size += comp.size;
		}
		is_dirty = false;
	}
}

void HWVertexFormat::SetComponentCount(S32 components)
{
	part_count = components;
	delete[] parts;
	parts = new HWVertexComponent[part_count];
	is_dirty = true;
}
