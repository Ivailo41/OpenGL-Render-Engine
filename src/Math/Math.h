#pragma once

#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/matrix_decompose.hpp>

namespace Engine::Math
{
	bool DecomposeMatrix(const glm::mat4& modelMatrix, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
}