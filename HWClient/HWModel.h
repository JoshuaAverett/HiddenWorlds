#pragma once

#include "HWIndexBuffer.h"
#include "HWVertexBuffer.h"
#include "HWCamera.h"
#include "HWTexture.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/quaternion.hpp"
#include "../glm/gtc/quaternion.hpp"

struct HWOBJVertex
{
	glm::vec3 position;
	//glm::vec3 normal;
	glm::vec2 uv;
};

class HWModel
{
public:
	HWModel();
	HWModel(HWVertexBuffer& verts, HWIndexBuffer& indices);
	~HWModel();

	void AddVertexData(const void* data, S32 length);
	void AddIndexData(const void* data, S32 length);

	void ReadOBJ(const std::string& path);

	void Transfer();
	void Bind();
	void Render(HWCamera* camera, HWShaderProgram* program);
	
	HWIndexType GetIndexType() const;

	void SetVertexFormat(HWVertexFormat& format);

	glm::mat4 GetMatrix() const;

	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 pos);

	glm::vec3 GetScale() const;
	void SetScale(glm::vec3 scale);

	glm::quat GetRotation() const;
	void SetRotation(glm::quat rot);

	void SetTexture(S32 index, HWTexture* tex);

private:
	HWVertexBuffer* verts;
	HWIndexBuffer* indices;
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	std::vector<HWTexture*> textures;
};

