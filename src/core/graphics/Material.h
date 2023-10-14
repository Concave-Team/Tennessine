#pragma once
#include <Renderer.h>
#include <assets/AssetManager.h>
#include <any>
#include <map>

class Material
{
public:
	std::string Name;
	ShaderProgram MaterialShader;
	
	float DiffuseStrength = 0.1f;
	float SpecularStrength = 1.0f;
	float NormalMapCOF = 1.0f;
	float Shininess = 0.4f;
	float Metallicness = 0.0f;

	GLuint NormalMap;
	GLuint SpecularMap;

	GLuint AlbedoTexture = 0;
	glm::vec3 AlbedoColor = glm::vec3(1,1,1);

	glm::vec3 ViewPosition = glm::vec3(0, 0, 0);

	void Update();

	static Material LoadDefaultMaterial();
};