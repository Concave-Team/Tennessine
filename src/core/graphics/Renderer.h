#pragma once
#ifndef EXAMPLE_H
#define EXAMPLE_H
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

enum class ShaderType
{
	UNKNOWN = 0,
	FRAGMENT,
	VERTEX
};

struct Shader
{
	GLuint ShaderId; // the shader's id(OpenGL)
	ShaderType Type; // the type of shader.
	int CompileShader(Shader shd);
	void DestroyShader(Shader shd);
};

struct ShaderProgram
{
public:
	GLuint ProgramId;

	// UNIFORMS
	void SetFloat(const std::string& name, float value) const;
	void SetInt(const std::string& name, int value) const;
	void SetBool(const std::string& name, bool value) const;
	void SetMat4(const std::string& name, glm::mat4 value) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	// UTIL
	void Use();
	int Link();
	void DestroyProgram();
};

struct FBOTexture
{
	GLuint FBOid, RBOid, TEXid;
};

class Renderer
{
public:
	// VARIABLES(PUBLIC)
	GLuint BoundVAO;
	GLuint BoundVBO;
	GLuint BoundEBO;
	GLuint BoundFBO;

	GLuint gBuffer;
	GLuint gPos, gNorm, gColorSpec;
	GLuint rboDepth;
	GLuint Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	GLuint SkyboxVAO;
	GLuint SkyboxVBO;
	ShaderProgram SkyboxShader;
	GLuint SkyboxCubemap;
	ShaderProgram CurrentShader;
	glm::mat4 ProjectionMat;
	glm::mat4 ViewMat;
	glm::vec3 Position, Target, Up;

	// CTOR/DTOR
	Renderer();
	~Renderer();

	static Renderer& GetInstance() {
		static Renderer instance; // This ensures it's created only once.
		return instance;
	}

	// UTIL
	void Init(int width, int height);
	void CreateProjection();
	void CreateView(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	//void Destroy();

	// SHADERS
	static Shader CreateShader(std::string code, ShaderType ty);
	static ShaderProgram CreateProgram(Shader frag, Shader vert);

	// TEXTURES
	GLuint CreateTexture(std::string path);
	GLuint CreateCubemap(std::vector<std::string> paths);

	// STATES
	//void SetBlendMode(bool enable);
	void SetDepthTesting(bool enable);
	void SetViewport(int x, int y, int width, int height);

	// OPENGL OBJECTS
	GLuint CreateVAO();
	GLuint CreateVBO();
	GLuint CreateEBO();
	GLuint CreateFBO();
	void BindVAO(GLuint id);
	void BindVBO(GLuint id);
	void BindEBO(GLuint id);
	void BindFBO(GLuint id);
	void UnbindVAO();
	void UnbindVBO();
	void UnbindEBO();
	void UnbindFBO();

	// starts doing the whole renderbuffer thing
	FBOTexture CreateFBOTexture(GLuint id, int sizeX, int sizeY);

	// RENDERING
	void Clear();
	[[deprecated("Use RenderMesh(Mesh, glm::mat4) instead.")]] void RenderObject(ShaderProgram program, const GLuint& vao, GLuint numIndices, GLuint indices[], const GLuint& texture, const glm::mat4& modelMatrix);
	void RenderMesh(Mesh msh, const glm::mat4& modelMatrix);
	void RenderSkyboxCubemap(GLuint cubemap);
	//void OnRender(float dt);

	// ERROR-HANDLING
	static void CheckGLErrors(std::string ctx);
};

#endif