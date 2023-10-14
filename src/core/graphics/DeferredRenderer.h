#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "EventHandler.h"
#include "AssetManager.h"
#include "errorhelper.h"
#include "stb_image.h"
#include "graphics/Mesh.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>

class DeferredRenderer
{
public:
	GLuint gBuffer;
	GLuint gPos, gNorm, gColorSpec;
	GLuint Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	void Init(int width, int height);
	void LightingPass();
};

