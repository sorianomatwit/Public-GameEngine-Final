#pragma once
#include <map>
#include "ComponentSet.hpp"
#include "EntityManager.hpp"

struct BitTracker;
struct Entity;
class EntityManager;

template <typename T>
class Component : public ComponentBase {
protected:
	/**
	* This creates teh ComponetSet in it constructor
	* Syntax for createing a component:
	* class (C or T if a component or tag)[insert Component Name] : public Component<C[Insert Component Name]> {
	* public:
	* C[Insert Component Name]();
	* }
	*/
	Component();
};

class ComponentManager
{
private:
	template <typename T>
	friend class Component;
	friend class EntityManager;
	inline static uint16_t componentCount;

	template <ComponentChild T>
	static std::shared_ptr<ComponentSet<T>> CreateComponentSet();
public:
	inline static std::vector<std::shared_ptr<Sets>> allComponentSets;

};


template <ComponentChild T>
std::shared_ptr<ComponentSet<T>> ComponentManager::CreateComponentSet()
{
	//TODO Create assertion that assert that componentCount is not > than bitrackertotalbits
	if (ComponentSet<T>::key >= BitTracker::TOTAL_BITS) {
		auto componentSet = ComponentSet<T>::init(componentCount);
		allComponentSets.push_back(componentSet);
		componentCount++;
	}
	return std::reinterpret_pointer_cast<ComponentSet<T>>(allComponentSets[ComponentSet<T>::key]);
}



template <typename T>
Component<T>::Component() {
	ComponentManager::CreateComponentSet<T>();
}
