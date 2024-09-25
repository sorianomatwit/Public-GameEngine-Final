#pragma once
#include "EntityManager.hpp"


/**
* Abstract class that allows for systems to be store in arrays have genral fucntions that get called
*/
class SystemBase {
public:
	/** Controls if a system calls it's execute function **/
	bool isActive = true;
	virtual void Run(float deltaTime) = 0;
	virtual ~SystemBase() = default;
};

/**
 * Abtract class use to create system that will iterate over specified component
 * Syntax in creating a system:
 * [Insert name MUSt be a VERB]System : public System<[Insert Component]> {
 * protected:
 *	void Execute(std::vector<uint16_t> entites, float deltatime) override;
 * }
 */
template <ComponentChild T>
class System : public SystemBase {
protected:
	/**
	* @param entites - list of all the entities that contain the System's T component
	* @param deltatime - the time between frames in milisecs
	* Iterates through all entites that contains the System's T component
	*/
	virtual void Execute(std::vector<uint16_t> entities, float deltatime) = 0;
	System() {
		T();
	}
public:
	/**
	* Calls a System's execute function and gets all the entites of the the System's T component
	* 
	* @param deltatime - the time between frames in milisecs
	*/
	void Run(float deltaTime) override;
};


template <ComponentChild T>
void System<T>::Run(float deltaTime) {
	if (isActive) {
		Execute(ComponentSet<T>::entities, deltaTime);
	}
}