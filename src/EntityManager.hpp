#pragma once
#include "ComponentManager.hpp"

class ComponentManager;

struct BitTracker
{
public:
	static const uint64_t TOTAL_BITS = 64;
private:
	friend class EntityManager;
	friend struct Entity;
	uint64_t bits = 0;
	/**
	* Checks if bit at given index is flipped
	* @param index - bit index
	* @return true or false if the bit is flipped
	*/
	bool Has(uint64_t index) { return (bits & (1ULL << index)) == (1ULL << index); };
	/**
	* Flips bit on at given index 
	* @param index - bit index
	*/
	void Add(uint64_t index) {
		if (!Has(index)) {
			bits = bits | (1ULL << index);
		}
	}
	/**
	* Flips bit off at given index if bitracker has bit otherwise it does nothing
	* @param index - bit index
	*/
	void Remove(uint64_t index) {
		if (Has(index)) {
			bits = bits ^ (1ULL << index);
		}
	}
	BitTracker(uint64_t bits) {
		bits = bits;
	}
	BitTracker() { bits = 0; }
};

struct Entity
{
private:
	uint16_t _id;
public:
	uint8_t componentCount = 0;
	inline Entity(uint16_t id);
	template <ComponentChild T>
	inline bool hasComponent();
	template <ComponentChild T>
	inline Entity addComponent(const T& data);
	template <ComponentChild T>
	inline 	Entity removeComponent();
	template <ComponentChild T>
	inline T& getComponent();
	inline void destroy();
	inline bool equals(Entity other);
	inline void setActive(bool isActive);
	inline bool isActive();
};


class EntityManager
{
private:
	friend struct Entity;
	inline static bool activeEntities[Sets::MAX_ENTITIES];
	inline static BitTracker entityBits[Sets::MAX_ENTITIES];
	inline static std::vector<uint16_t> deadEntities;
	inline static uint16_t nextEntityId = 0;
	template<ComponentChild T>
	inline static void AddKey(uint16_t id);
	template<ComponentChild T>
	inline static void RemoveKey(uint16_t id);
	inline static void DestroyEntity(uint16_t id);
public:
	inline static Entity createEntity();
	inline static void destroyAll();
	inline static void activateAllEntities();
	inline static void deactivateAllEntities();
};

#pragma region Entity

Entity::Entity(uint16_t id) {
	_id = id;
}

void Entity::setActive(bool isActive) {
	EntityManager::activeEntities[_id] = isActive;
}

bool Entity::isActive() {
	return EntityManager::activeEntities[_id];
}
bool Entity::equals(Entity other) {
	return _id == other._id;
}

void Entity::destroy() {
	EntityManager::DestroyEntity(_id);
}

template <ComponentChild T>
bool Entity::hasComponent() {
	return EntityManager::entityBits[_id].Has(ComponentSet<T>::key) && ComponentSet<T>::key < 64;
}

template <ComponentChild T>
Entity Entity::addComponent(const T& data) {
	assert(!ComponentSet<T>::HasEntity(_id));
	componentCount++;
	ComponentSet<T>::AddEntity(_id, data);
	EntityManager::AddKey<T>(_id);
	return Entity(_id);
}

template <ComponentChild T>
Entity Entity::removeComponent() {
	assert(ComponentSet<T>::HasEntity(_id));
	ComponentSet<T>::RemoveEntity(_id);
	EntityManager::RemoveKey<T>(_id);
	return Entity(_id);
}

template <ComponentChild T>
T& Entity::getComponent() {
	assert(ComponentSet<T>::HasEntity(_id));
	return ComponentSet<T>::GetComponent(_id);
}
#pragma endregion

#pragma region EntityManager

Entity EntityManager::createEntity()
{
	uint16_t id = EntityManager::nextEntityId;
	if (EntityManager::deadEntities.size() > 0) {
		id = deadEntities.back();
		deadEntities.pop_back();
	}
	else {
		EntityManager::nextEntityId++;
	}

	EntityManager::entityBits[id] = BitTracker();
	return Entity(id);
}

void EntityManager::activateAllEntities() {
	for (uint16_t id = 0; id < nextEntityId;id++)
	{
		Entity(id).setActive(true);
	}
}
void EntityManager::deactivateAllEntities() {
	for (uint16_t id = 0; id < nextEntityId;id++)
	{
		Entity(id).setActive(false);
	}
}

template<ComponentChild T>
void EntityManager::AddKey(uint16_t id) {
	auto& entityBits = EntityManager::entityBits[id];
	entityBits.Add(ComponentSet<T>::key);
};

template<ComponentChild T>
void EntityManager::RemoveKey(uint16_t id) {
	auto& entityBits = EntityManager::entityBits[id];
	entityBits.Remove(ComponentSet<T>::key);
};

void EntityManager::DestroyEntity(uint16_t id)
{
	EntityManager::deadEntities.push_back(id);
	auto& bits = EntityManager::entityBits[id];
	for (uint64_t i = 0; i < BitTracker::TOTAL_BITS; i++)
	{
		if (bits.Has(i))
		{
			bits.Remove(i);
			ComponentManager::allComponentSets[i]->Remove(id);
		}
	}
	EntityManager::entityBits[id] = BitTracker();
	EntityManager::activeEntities[id] = false;
}

void EntityManager::destroyAll() {
	for (uint16_t id = 0; id < nextEntityId;id++)
	{
		Entity(id).destroy();
	}
	EntityManager::deadEntities.clear();
	nextEntityId = 0;
	for (auto& componentset : ComponentManager::allComponentSets) {
		componentset->Reset();
	}
	ComponentManager::allComponentSets.clear();
	ComponentManager::componentCount = 0;
}
#pragma endregion

