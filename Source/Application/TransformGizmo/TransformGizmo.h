#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

class TransformGizmo
{	
public:
	struct Gizmo
	{
		unsigned verticesCount;
		unsigned indicesCount;
		double* vertices;
		unsigned* indices;
	};

public:
	static void draw(const Gizmo& gizmoType);

	static Gizmo translateGizmo;
	static unsigned translateVBO, translateEBO;
};

