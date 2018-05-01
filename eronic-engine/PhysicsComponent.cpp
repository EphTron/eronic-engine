#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Message.h"
#include "ThrustMessage.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

PhysicsComponent::PhysicsComponent(GameObject* gob)
	: GameObjectComponent("physics", gob),
	_velocity(0.0f, 0.0f, 0.0f, 0.0f)
{

}

/******************************************************************************************************************/

PhysicsComponent::~PhysicsComponent()
{

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void PhysicsComponent::Start()
{
	_parent->RegisterListener("thrust", this);
}

/******************************************************************************************************************/

// Main update function (called every frame)
void PhysicsComponent::Update(double deltaTime)
{


}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void PhysicsComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "thrust")
	{
		ThrustMessage* tm = (ThrustMessage*)msg;
		_velocity += tm->GetThrust();
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void PhysicsComponent::End()
{
	_parent->UnregisterListener("thrust", this);
}

/******************************************************************************************************************/

void PhysicsComponent::LimitToMaximumSpeed(float max)
{
	_velocity.limitTo(max);
}

void PhysicsComponent::SlowDown(float delta,float slow)
{
	Vector4 v(_velocity.x() - (_velocity.x() / slow) * delta, 
			  _velocity.y() - (_velocity.y() / slow) * delta,
		      _velocity.z() - (_velocity.z() / slow) * delta,
			  _velocity.w());
	_velocity = v;
}

/******************************************************************************************************************/

Vector4 PhysicsComponent::GetObjectPosition()
{
	return _parent->GetPosition();
}

/******************************************************************************************************************/

void PhysicsComponent::SetObjectPosition(Vector4 v)
{
	_parent->SetPosition(v);
}

/******************************************************************************************************************/
