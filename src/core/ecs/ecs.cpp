#include "ecs.h"

void World::RegisterEntity(std::shared_ptr<Entity> ent)
{
	this->EntityCount++;
	ent->entityId = this->EntityCount;
}

void World::AddEntity(std::shared_ptr<Entity> ent)
{
	this->WorldEntities.emplace_back(ent);
}

void World::DestroyEntity(std::shared_ptr<Entity> ent)
{
	int id = ent->entityId;
	this->WorldEntities.erase(std::remove_if(this->WorldEntities.begin(), this->WorldEntities.end(),
		[id](const std::shared_ptr<Entity> entity) {
			return entity->entityId == id;
		}), this->WorldEntities.end());
	EntityCount--;
}

void World::UpdateScene(float dt)
{
	for (auto& ent : this->WorldEntities)
	{
		ent->Update(dt);
	}
}

void Entity::RegisterComponent(std::shared_ptr<EComponent> component)
{
	component->componentId = this->Components.size()+1;
	component->parentId = this->entityId;
	this->Components.emplace_back(component);
}

void Entity::RemoveComponent(std::shared_ptr<EComponent> component)
{
	int id = component->componentId;
	this->Components.erase(std::remove_if(this->Components.begin(), this->Components.end(),
		[id](const std::shared_ptr<EComponent> com) {
			return com->componentId == id;
		}), this->Components.end());
}
