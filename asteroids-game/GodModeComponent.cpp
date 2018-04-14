#include "GodModeComponent.h"
#include "Message.h"
#include "Components.h"
#include "Ship.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GodModeComponent::GodModeComponent(GameObject* gob)
	: GameObjectComponent("godmode",gob),
	_godMode(false),
	_fader(1.0f),
	_faderInc(1),
	_collisionID(0)
{
}

/******************************************************************************************************************/

GodModeComponent::~GodModeComponent()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

// Setup function -- called when parent object is initialised (using its own Start method)
void GodModeComponent::Start()
{
	// Register as a listener
	_parent->RegisterListener("godmode", this);

	RenderComponent* rc = (RenderComponent*)_parent->GetComponent("render");
	if (rc)
	{
		_objectColour = rc->GetColour();
	}

	CollisionComponent* cc = (CollisionComponent*)_parent->GetComponent("collision");
	if (cc)
	{
		_collisionID = cc->GetCollisionID();
	}

}

/******************************************************************************************************************/

// Main update function (called every frame)
void GodModeComponent::Update(double deltaTime)
{
	_fader += (float)(_faderInc * deltaTime);
	if (_fader < 0.1f)
	{
		_fader = 0.1f;
		if (_faderInc < 0)
		{
			_faderInc = -_faderInc;
		}
	}
	else if (_fader > 1)
	{
		_fader = 1;
		if (_faderInc > 0)
		{
			_faderInc = -_faderInc;
		}
	}

	// Set colour
	RenderComponent* rc = (RenderComponent*)_parent->GetComponent("render");
	if (rc)
	{
		if (!_godMode)
		{
			rc->SetColour(_objectColour);
		}
		else
		{
			Colour c = _objectColour;
			c.r(c.r() * _fader);
			c.g(c.g() * _fader);
			c.b(c.b() * _fader);
			c.a(1.0f);
			rc->SetColour(c);
		}
	}
}

/******************************************************************************************************************/

// Message handler (called when message occurs)
void GodModeComponent::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "godmode")
	{
		ToggleGodMode();

		// Implement by disabling the collision matrix ID
		CollisionComponent* cc = (CollisionComponent*)_parent->GetComponent("collision");
		if (cc)
		{
			if (_godMode)
			{
				cc->SetCollisionID(0); // Disable collisions
			}
			else
			{
				cc->SetCollisionID(_collisionID); // Re-enable collisions
			}
		}
	}
}

/******************************************************************************************************************/

// Shutdown function -- called when parent object is destroyed
void GodModeComponent::End()
{
	_parent->UnregisterListener("godmode", this);
}

/******************************************************************************************************************/
