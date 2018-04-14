#include "LifeTimerComponent.h"
#include "GameObject.h"
#include "DeadObjectMessage.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

LifeTimerComponent::LifeTimerComponent(GameObject* gob, double startingLife)
	: GameObjectComponent("lifetimer", gob),
	_startingLifeTime(startingLife),
	_timeLeft(startingLife)
{

}

/******************************************************************************************************************/

LifeTimerComponent::~LifeTimerComponent()
{

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void LifeTimerComponent::Start()
{
	_timeLeft = _startingLifeTime;
}

/******************************************************************************************************************/

// Main update function (called every frame)
void LifeTimerComponent::Update(double deltaTime)
{
	DecreaseLifeTime(deltaTime);

	if (_timeLeft < 0)
	{
		_timeLeft = 0;
		
		// Send dead message
		DeadObjectMessage msg(_parent);
		_parent->OnMessage(&msg);
	}
}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void LifeTimerComponent::OnMessage(Message* msg)
{
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void LifeTimerComponent::End()
{
}

/******************************************************************************************************************/
