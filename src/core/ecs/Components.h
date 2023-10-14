#pragma once
#include "ecs.h"
#include "EventHandler.h"
#include "Vector2.h"
#include "Mesh.h"
#include "Material.h"
class EntityEventArgs : public EventArgs
{
public:
	// id of the entity
	long entity;
};

class TransformComponent : public EComponent
{
public:
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
};

class MeshRendererComponent : public EComponent
{
public:
	std::vector<Mesh> Meshes;
	std::vector<Material> Materials;

	void AddMeshAndMaterial(Mesh mesh, Material mat);
};
