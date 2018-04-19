#include "ExplodableComponent.h"
#include "Explosion.h"
#include "Game.h"
#include "DeadObjectMessage.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

ExplodableComponent::ExplodableComponent(GameObject* gob)
	: GameObjectComponent("explodable", gob),
	_explosion(NULL),
	_exploded(false)

{

}

/******************************************************************************************************************/

ExplodableComponent::~ExplodableComponent()
{
	// Explosion deleted by the game
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void ExplodableComponent::Start()
{
	// Create an explosion and add it to the game
	_explosion = new Explosion(Game::TheGame->GetMesh("explosion"));
	Game::TheGame->AddGameObject(_explosion);

	// Register for death event
	_parent->RegisterListener("dead", this);
}

/******************************************************************************************************************/

// Main update function (called every frame)
void ExplodableComponent::Update(double deltaTime)
{
	_exploding = _explosion->IsAlive();
}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void ExplodableComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "dead")
	{
		DeadObjectMessage* dom = (DeadObjectMessage*)msg;
		if (dom->GetDeadObject() == _parent)
		{
			// Boom
			_explosion->Spawn(_parent->GetPosition());
			_exploded = true;
			_exploding = true;
		}
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void ExplodableComponent::End()
{
	_parent->UnregisterListener("dead", this);
}

/******************************************************************************************************************/
