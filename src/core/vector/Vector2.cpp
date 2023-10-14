#include "Vector2.h"

glm::vec3 Vector3::ToGLMVec3()
{
	return glm::vec3(this->x, this->y, this->z);
}