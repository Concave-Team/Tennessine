#include "Components.h"

void MeshRendererComponent::AddMeshAndMaterial(Mesh mesh, Material mat)
{
	this->Materials.emplace_back(mat);
	this->Meshes.emplace_back(mesh);

	auto eargs = std::make_shared<EntityEventArgs>();

	eargs->entity = this->parentId;

	EventHandler::FireEvent(EventType::MESHCP_MESH_ADDED, eargs);
	EventHandler::FireEvent(EventType::MESHCP_MAT_ADDED, eargs);
}