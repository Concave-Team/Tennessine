#pragma once
#include <iostream>
#include <vector>
#include <functional>

class EComponent { public: long parentId; long componentId; void UpdateFunction() {} virtual ~EComponent() {} };

class Entity
{
public:
	long entityId;
	std::string Name;
	std::string Tag;
	std::vector<std::shared_ptr<Entity>> Children;
	std::vector<std::shared_ptr<EComponent>> Components;

	// Updates an entity, must be filled in with custom data on a per-inherited class basis.
	void Update(float dt) {};
	// Draws an entity, must be filled in with custom data on a per-inherited class basis.
	void Draw() {};
	// Registers and enters in a component id.
	void RegisterComponent(std::shared_ptr<EComponent> component);
	template <class T>
	std::shared_ptr<EComponent> GetComponentOfType()
	{
		for (int i = 0; i < this->Components.size(); i++)
		{
			if (dynamic_cast<T*>(this->Components[i].get()) != nullptr)
			{
				return this->Components[i];
			}
		}

		return nullptr;
	}

	template <class T>
	bool HasComponentOfType()
	{
		for (int i = 0; i < this->Components.size(); i++)
		{
			if (dynamic_cast<T*>(this->Components[i].get()) != nullptr)
			{
				return true;
			}
		}

		return false;
	}
	// Deletes and deregisters a given component.
	void RemoveComponent(std::shared_ptr<EComponent> component);
};

class World
{
public:
	// Number of entities
	int EntityCount = 0;
	// List of all entities
	std::vector<std::shared_ptr<Entity>> WorldEntities;
	// Get the instance of the World (use this.)
	static World& GetInstance() {
		static World instance; // This ensures it's created only once.
		return instance;
	}
	// Updates all of the currently loaded in world entities.
	void UpdateScene(float dt);
	// Entity is assigned an entityId, and is registered to the WorldEntities vector.
	void RegisterEntity(std::shared_ptr<Entity> ent); 
	// Destroys and deregisters a given entity.
	void DestroyEntity(std::shared_ptr<Entity> ent);
	// Adds entity to the WorldEntities list, do not use this if you are going to make an entity child.
	void AddEntity(std::shared_ptr<Entity> ent);

private:
	World() {} // Private constructor to prevent external instantiation.

	// Private copy constructor and assignment operator to prevent cloning.
	World(const World&) = delete;
	World& operator=(const World&) = delete;
};