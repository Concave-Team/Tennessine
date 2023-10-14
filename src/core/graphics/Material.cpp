#include "Material.h"

Material Material::LoadDefaultMaterial()
{
	Asset FragmentShader = AssetManager::LoadAsset("shaders/deferred/dflighting.frag");
	Asset VertexShader = AssetManager::LoadAsset("shaders/lighting.vert");

	if (FragmentShader.data == "error")
	{
		SendErrorEvent("Could not load base fragment shader.");
		return {};
	}
	if (VertexShader.data == "error")
	{
		SendErrorEvent("Could not load base vertex shader.");
		return {};
	}

	Material mat;

	ShaderProgram shad = Renderer::CreateProgram(Renderer::CreateShader(FragmentShader.data, ShaderType::FRAGMENT), Renderer::CreateShader(VertexShader.data, ShaderType::VERTEX));

	mat.Name = "DefaultMaterial";
	mat.MaterialShader = shad;

	mat.Update();

	return mat;
}

/*
* from GLSL:
	struct Material
	{
		float diffuseStrength;
		float specularStrength;
		sampler2D albedoTexture;
		vec3 albedoColor;
		bool Textured;
	}
*/

// Updates the material's shaders with new data.
void Material::Update()
{
	this->MaterialShader.Use();
	/*this->MaterialShader.SetVec3("Light.Position", glm::vec3{-1.0f, 0.0f, 0.0f});
	this->MaterialShader.SetVec3("Light.Color", glm::vec3{ 1.0f, 1.0f, 1.0f });
	this->MaterialShader.SetVec3("Light.Direction", glm::vec3{ 0, 0, 1.0f });
	this->MaterialShader.SetVec3("viewPos", this->ViewPosition);
	this->MaterialShader.SetVec3("Material.albedoColor", this->AlbedoColor);
	this->MaterialShader.SetFloat("Material.Roughness", this->DiffuseStrength);
	this->MaterialShader.SetFloat("Material.specularStrength", this->SpecularStrength);
	if (this->AlbedoTexture != 0)
	{
		this->MaterialShader.SetBool("Material.Textured", true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->AlbedoTexture);
		this->MaterialShader.SetInt("Material.albedoTexture", 0);
	}
	else
	{
		this->MaterialShader.SetBool("Material.Textured", false);
	}*/

	this->MaterialShader.SetVec3("viewPos", this->ViewPosition);
}