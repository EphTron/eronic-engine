#include "HitpointComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "DeadObjectMessage.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

HitpointComponent::HitpointComponent(GameObject* gob)
	: GameObjectComponent("hitpoints", gob),
	_hitPoints(0),
	_maxHitPoints(0)

{

}

/******************************************************************************************************************/

HitpointComponent::~HitpointComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void HitpointComponent::Start()
{
	// Register for damage event
	_parent->RegisterListener("damage", this);
}

/******************************************************************************************************************/

// Main update function (called every frame)
void HitpointComponent::Update(double deltaTime)
{
	// Send out death message if health < 0
	if (_hitPoints <= 0)
	{
		_hitPoints = 0;
		DeadObjectMessage msg(_parent);
		Game::TheGame->BroadcastMessage(&msg);
	}
}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void HitpointComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "damage")
	{
		// Reduce hitpoints
		DecrememtHitPoints();
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void HitpointComponent::End()
{
	_parent->UnregisterListener("damage", this);
}

/******************************************************************************************************************/
