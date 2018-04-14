#include "UserInputComponent.h"
#include "GameObject.h"
#include "Message.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

UserInputComponent::UserInputComponent(GameObject* gob)
	: GameObjectComponent("input", gob)
{

}

/******************************************************************************************************************/

UserInputComponent::~UserInputComponent()
{

}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void UserInputComponent::Start()
{
	// Register as a listener
	_parent->RegisterListener("keypress", this);
}

/******************************************************************************************************************/

// Main update function (called every frame)
void UserInputComponent::Update(double deltaTime)
{

}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void UserInputComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "keypress")
	{
		// Respond to this keypress somehow
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void UserInputComponent::End()
{
	_parent->UnregisterListener("keypress", this);
}

/******************************************************************************************************************/
