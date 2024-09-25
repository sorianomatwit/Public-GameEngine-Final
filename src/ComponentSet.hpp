#pragma once
#include <memory>
#include <vector>
#include <array>
#include <type_traits>
#include <format>
#include <iostream>
#include <stdexcept>
#include <cassert>

class Sets
{
public:
	static const uint16_t MAX_ENTITIES = UINT16_MAX;
	virtual void Remove(uint16_t entityID) = 0;
	virtual void Reset() = 0;
	virtual int GetComponentCount() = 0;
};

class ComponentBase { public: std::string stringData() { return "N/A"; } };

template <typename T>
concept ComponentChild = std::is_base_of_v<ComponentBase, T>&& std::derived_from<T, ComponentBase>;

template <ComponentChild T>
class ComponentSet : public Sets
{
private:
	inline static std::array<uint16_t, Sets::MAX_ENTITIES> sparseArray;
	uint8_t componentKey;
public:
	inline static std::vector<T> components;
	//friend class ComponentManager
	ComponentSet(uint8_t componentKey) : componentKey(componentKey) {};

	void Remove(uint16_t entityID) override {
		ComponentSet<T>::RemoveEntity(entityID);
	};
	void Reset() override {
		ComponentSet<T>::key = 64;
	}
	int GetComponentCount() {
		return ComponentSet<T>::count;
	}
	inline static std::vector<uint16_t> entities;
	inline static uint8_t key = 64;
	inline static size_t count = 0;

	inline static std::shared_ptr <ComponentSet<T>> init(uint8_t componentKey);
	/**
	* Checks if entity id is within ComponentSet
	* @param entityID - entity id
	*/
	inline static bool HasEntity(uint16_t entityID);
	/**
	* Removes entity id within ComponentSet
	* @param entityID - entity id
	*/
	inline static void RemoveEntity(uint16_t entityID);
	/**
	* Add entity id within ComponentSet
	* @param entityID - entity id
	*/
	inline static void AddEntity(uint16_t entityID, const T& data);
	/**
	* Get component linked to entity id within ComponentSet
	* @param entityID - entity id
	*/
	inline static T& GetComponent(uint16_t entityID);
};

template <ComponentChild T>
std::shared_ptr<ComponentSet<T>> ComponentSet<T>::init(uint8_t componentKey)
{
	key = componentKey;
	for (size_t i = 0; i < Sets::MAX_ENTITIES; i++)
	{
		sparseArray[i] = Sets::MAX_ENTITIES;
	}
	return std::make_shared<ComponentSet<T>>(componentKey);
}

template <ComponentChild T>
bool ComponentSet<T>::HasEntity(uint16_t entityID)
{
	bool cond1 = entityID < Sets::MAX_ENTITIES;
	cond1 = cond1 && sparseArray[entityID] < entities.size();
	cond1 = cond1 && sparseArray[entityID] != Sets::MAX_ENTITIES;
	cond1 = cond1 && entities.size() > 0 && entities[sparseArray[entityID]] == entityID;
	return cond1;

}
template <ComponentChild T>
void ComponentSet<T>::RemoveEntity(uint16_t removedEntity)
{
	assert(HasEntity(removedEntity));

	uint16_t removedEntityIndex = sparseArray[removedEntity];
	uint16_t lastEntity = entities[count - 1];

	sparseArray[removedEntity] = Sets::MAX_ENTITIES;

	entities[removedEntityIndex] = entities[count - 1];
	components[removedEntityIndex] =  components[count - 1];

	entities.pop_back();
	components.pop_back();

	sparseArray[lastEntity] = removedEntityIndex;
	count = entities.size();
}
template <ComponentChild T>
void ComponentSet<T>::AddEntity(uint16_t entityID, const T& data)
{
	assert(!HasEntity(entityID));
	sparseArray[entityID] = entities.size();
	entities.push_back(entityID);
	components.push_back(data);
	count = entities.size();
}


template <ComponentChild T>
T& ComponentSet<T>::GetComponent(uint16_t entityID)
{
	assert(HasEntity(entityID));
	return components[sparseArray[entityID]];
}

