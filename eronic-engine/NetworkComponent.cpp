#include "NetworkComponent.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Message.h"
#include "ThrustMessage.h"



/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/


NetworkComponent::NetworkComponent(GameObject * gob, std::string tag) :
	GameObjectComponent("network", gob),
	_owner_tag(tag),
	_should_sent(false)
{
}

NetworkComponent::~NetworkComponent()
{
}


/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void NetworkComponent::Start()
{
}

/******************************************************************************************************************/

// Main update function (called every frame)
void NetworkComponent::Update(double deltaTime)
{


}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void NetworkComponent::OnMessage(Message* msg)
{
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void NetworkComponent::End()
{
}

/******************************************************************************************************************/

Vector4 NetworkComponent::GetObjectPosition()
{
	return _parent->GetPosition();
}

/******************************************************************************************************************/

void NetworkComponent::SetObjectPosition(Vector4 v)
{
	_parent->SetPosition(v);
}

/******************************************************************************************************************/
