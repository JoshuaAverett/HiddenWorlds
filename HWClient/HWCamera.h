#pragma once

#include "pch.h"
#include "HWGL.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"

class HWCamera
{
public:
	HWCamera();
	~HWCamera();

	void SetScreenSize(S32 width, S32 height);

	glm::mat4 GetMatrix();

	void SetPosition(glm::vec3 pos);
	void SetLookAt(glm::vec3 target);

private:
	bool isDirty;

	S32 width, height;
	F32 FOV;
	F32 nearClip, farClip;

	glm::vec3 position;
	glm::vec3 lookAt;

	glm::mat4 matrix;
};
