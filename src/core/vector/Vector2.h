#pragma once
#include <glm/glm.hpp>

class Vector2
{
public:
	float x, y;
};

class Vector3
{
public:
	float x, y, z;

	glm::vec3 ToGLMVec3();
};

