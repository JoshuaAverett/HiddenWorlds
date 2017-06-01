#include "pch.h"
#include "HWCamera.h"

HWCamera::HWCamera()
{
	width = 1024;
	height = 768;
	FOV = 60.0f;
	nearClip = 0.1f;
	farClip = 100.0f;

	lookAt = glm::vec3(0,0,-1);
	isDirty = true;
}

HWCamera::~HWCamera()
{
}

void HWCamera::SetScreenSize(S32 width, S32 height)
{
	this->width = width;
	this->height = height;
	glViewport(0, 0, width, height);
	isDirty = true;
}

glm::mat4 HWCamera::GetMatrix()
{
	if(isDirty)
	{
		matrix = glm::perspective(FOV, (F32)width / (F32)height, nearClip, farClip);
		matrix *= glm::lookAt(position, lookAt, glm::vec3(0,0,1));
		isDirty = false;
	}
	return matrix;
}

void HWCamera::SetPosition(glm::vec3 pos)
{
	position = pos;
	isDirty = true;
}

void HWCamera::SetLookAt(glm::vec3 target)
{
	lookAt = target;
	isDirty = true;
}
