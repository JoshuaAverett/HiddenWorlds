#include "pch.h"
#include "HWModel.h"
#include "../HWLib/HWFile.h"
#include <sstream>

#include "HWCamera.h"
#include "HWShaderProgram.h"

HWModel::HWModel()
{
}

HWModel::HWModel(HWVertexBuffer& _verts, HWIndexBuffer& _indices)
{
	verts = &_verts;
	indices = &_indices;
	scale = glm::vec3(1.f);
}

HWModel::~HWModel()
{
}

void HWModel::AddVertexData(const void* data, S32 length)
{
	verts->BufferData(data, length);
}

void HWModel::AddIndexData(const void* data, S32 length)
{
	indices->BufferData(data, length);
}

HWIndexType HWModel::GetIndexType() const
{
	return indices->GetIndexType();
}

void HWModel::Transfer()
{
	verts->Transfer();
	indices->Transfer();
}

void HWModel::Bind()
{	
	verts->Bind();
	indices->Bind();
}

void HWModel::Render(HWCamera* camera, HWShaderProgram* program)
{
	textures[0]->Bind(program->GetUniformID("texture"), 0);
	program->SetUniformValue("MVP", camera->GetMatrix() * GetMatrix());
	Bind();
	glDrawElements(GL_TRIANGLES, indices->Count(), (GLenum)indices->GetIndexType(), (void*)0);
}

void HWModel::SetVertexFormat(HWVertexFormat& format)
{
	verts->SetFormat(format);
}

void HWModel::ReadOBJ(const std::string& path)
{
	HWFile file = HWFile(path, HWFileOperation::Read);

	std::string data = file.Read();
	std::stringstream ss(data);
	std::string item;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> norms;
	std::vector<glm::vec2> tex;

	std::vector<glm::ivec3> faceVerts;
	std::vector<HWOBJVertex> out;
	std::vector<U16> outIdx;

	S32 count;
	F32 t[3];
	U16 idx[9];
	HWOBJVertex tempVtx;

    while(std::getline(ss, item))
	{
		switch(item[0])
		{
		case 'v':
			switch(item[1])
			{
			case 't':
				count = sscanf_s(item.c_str(), "vt %f %f %f", &t[0], &t[1], &t[2]);
				tex.push_back(glm::vec2(t[0], t[1]));
				break;
			case 'n':
				count = sscanf_s(item.c_str(), "vn %f %f %f", &t[0], &t[1], &t[2]);
				norms.push_back(glm::vec3(t[0], t[1], t[2]));
				break;
			case ' ':
				count = sscanf_s(item.c_str(), "v %f %f %f", &t[0], &t[1], &t[2]);
				verts.push_back(glm::vec3(t[0], t[1], t[2]));
				break;
			}
			break;
		case 'f':
			count = sscanf_s(item.c_str(), "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu", &idx[0], &idx[1], &idx[2], &idx[3], &idx[4], &idx[5], &idx[6], &idx[7], &idx[8]);
			// TODO: make this reuse verts
			//glm::ivec3 vtx = glm::ivec3(idx);
			//std::find(faceVerts.begin(), faceVerts.end(), vtx);

			outIdx.push_back(out.size());
			tempVtx.position = verts[idx[0] - 1];
			tempVtx.uv = tex[idx[1] - 1];
			//tempVtx.normal = norms[idx[2] - 1];
			out.push_back(tempVtx);

			outIdx.push_back(out.size());
			tempVtx.position = verts[idx[3] - 1];
			tempVtx.uv = tex[idx[4] - 1];
			//tempVtx.normal = norms[idx[5] - 1];
			out.push_back(tempVtx);

			outIdx.push_back(out.size());
			tempVtx.position = verts[idx[6] - 1];
			tempVtx.uv = tex[idx[7] - 1];
			//tempVtx.normal = norms[idx[8] - 1];
			out.push_back(tempVtx);
			break;
		}
    }

	AddVertexData(&out[0], out.size() * sizeof(HWOBJVertex));
	AddIndexData(&outIdx[0], outIdx.size() * sizeof(U16));
	Transfer();
}

glm::vec3 HWModel::GetPosition() const
{
	return position;
}

void HWModel::SetPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 HWModel::GetScale() const
{
	return scale;
}

void HWModel::SetScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::quat HWModel::GetRotation() const
{
	return rotation;
}

void HWModel::SetRotation(glm::quat rot)
{
	rotation = rot;
}

glm::mat4 HWModel::GetMatrix() const
{
	return glm::scale(glm::translate(glm::mat4(1.0f), position), scale) * glm::mat4_cast(rotation);
}

void HWModel::SetTexture(S32 index, HWTexture* tex)
{
	if(textures.capacity() < index + 1)
	{
		textures.resize(index + 1);
	}

	textures[index] = tex;
}
