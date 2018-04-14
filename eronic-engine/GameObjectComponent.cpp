#include "GameObjectComponent.h"
#include "GameObject.h"
#include "Game.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GameObjectComponent::GameObjectComponent(std::string type, GameObject* parent)
	: _parent(parent),
	  _componentType(type)
{
	// Add us to the parent component
	parent->AddComponent(this);
}

/******************************************************************************************************************/

GameObjectComponent::~GameObjectComponent()
{

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void GameObjectComponent::BroadcastMessage(Message* msg)
{
	Game::TheGame->BroadcastMessage(msg);
}

/******************************************************************************************************************/
