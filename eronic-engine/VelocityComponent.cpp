#include "VelocityComponent.h"
#include "GameObject.h"
#include "Message.h"
#include "ThrustMessage.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

VelocityComponent::VelocityComponent(GameObject* gob)
	: GameObjectComponent("velocity", gob),
	_velocity(0.0f, 0.0f, 0.0f, 0.0f)
{

}

/******************************************************************************************************************/

VelocityComponent::~VelocityComponent()
{

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void VelocityComponent::Start()
{
	_parent->RegisterListener("thrust", this);
}

/******************************************************************************************************************/

// Main update function (called every frame)
void VelocityComponent::Update(double deltaTime)
{
	Vector4 newpos = _parent->GetPosition();
	Vector4 vel = _velocity;
	vel *= deltaTime;
	newpos += vel;

	// Wrap around
	if (newpos.x() < -1)		newpos.x(newpos.x() + 2);
	else if (newpos.x() > 1)	newpos.x(newpos.x() - 2);
	if (newpos.y() < -1)		newpos.y(newpos.y() + 2);
	else if (newpos.y() > 1)	newpos.y(newpos.y() - 2);

	_parent->SetPosition(newpos);

}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void VelocityComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "thrust")
	{
		ThrustMessage* tm = (ThrustMessage*)msg;
		_velocity += tm->GetThrust();
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void VelocityComponent::End()
{
	_parent->UnregisterListener("thrust", this);
}

/******************************************************************************************************************/

void VelocityComponent::LimitToMaximumSpeed(float max)
{
	_velocity.limitTo(max);
}

/******************************************************************************************************************/
