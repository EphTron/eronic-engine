#include "GameObject.h"
#include "GameObjectComponent.h"
#include "Message.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GameObject::GameObject(std::string type)
	:	_angle(0),
		_scale(1),
		_position(0,0),
		_alive(true),
		_type(type)
{
}

/******************************************************************************************************************/

GameObject::~GameObject()
{
	End();
}

/******************************************************************************************************************/
// Component Functions
/******************************************************************************************************************/

bool GameObject::AddComponent(GameObjectComponent* goc)
{
	if (_components.find(goc->GetComponentType()) != _components.end())
	{
		// Error - already have this component!
		return false;
	}
	_components[goc->GetComponentType()] = goc;

	return true;
}

/******************************************************************************************************************/

bool GameObject::RemoveComponent(GameObjectComponent* goc)
{
	return RemoveComponent(goc->GetComponentType());
}

/******************************************************************************************************************/

bool GameObject::RemoveComponent(std::string type)
{
	ComponentMapIterator i = _components.find(type);
	if (i != _components.end())
	{
		// Remove it
		_components.erase(i);

		// Delete it
		i->second->End();
		delete i->second;

		return true;
	}

	// Couldn't find it
	return false;
}

/******************************************************************************************************************/

GameObjectComponent* GameObject::GetComponent(std::string type)
{
	ComponentMapIterator i = _components.find(type);
	if (i != _components.end())
	{
		// Return it
		return i->second;
	}

	// Couldn't find it
	return NULL;
}

/******************************************************************************************************************/
// General Functions
/******************************************************************************************************************/

// Setup function -- called to initialise object and its components
void GameObject::Start()
{
	// Initialise all objects
	for (ComponentMapIterator i = _components.begin();
		i != _components.end();
		++i)
	{
		i->second->Start();
	}
}

/******************************************************************************************************************/

// Main update function (called every frame)
void GameObject::Update(double deltaTime)
{
	// Update all objects
	for (ComponentMapIterator i = _components.begin();
		i != _components.end();
		++i)
	{
		i->second->Update(deltaTime);
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when object is destroyed (i.e., from destructor)
void GameObject::End()
{
	// End all objects
	for (ComponentMapIterator i = _components.begin();
		i != _components.end();
		++i)
	{
		GameObjectComponent* component = i->second;
		component->End();
		delete component;
	}

	// Clear list
	_components.clear();
}

/******************************************************************************************************************/

// Resets the game object to the start state (similar to Start(), but may be called more than once)
void GameObject::Reset()
{

}

/******************************************************************************************************************/
