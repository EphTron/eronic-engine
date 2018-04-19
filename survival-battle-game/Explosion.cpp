#include "Explosion.h"
#include "Components.h"
#include "Message.h"
#include "DeadObjectMessage.h"

const float Explosion::LIFETIME = 1.0f;
const float Explosion::MAX_SIZE = 1.0f;

/******************************************************************************************************************/

Explosion::Explosion(Mesh* mesh)
	: GameObject("explosion")
{
	// Create components for object (they will add themselves)
	new LifeTimerComponent(this, LIFETIME);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetMesh(mesh);
	rc->ShouldDraw(false);

	_alive = false;
}

/******************************************************************************************************************/

Explosion::~Explosion()
{
}

/******************************************************************************************************************/

void Explosion::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	// Rotate (just for fun)
	_angle += (float)(180 * deltaTime);

	// Scale according to lifetime
	LifeTimerComponent* c = (LifeTimerComponent*)GetComponent("lifetimer");
	
	float percentLifeLeft = 1 - (float)(c->GetPercentLifeLeft());
	_scale = MAX_SIZE * percentLifeLeft;
}

/******************************************************************************************************************/

void Explosion::OnMessage(Message* msg)
{
	// If the object's life is expired, stop drawing it
	if (msg->GetMessageType() == "dead")
	{
		DeadObjectMessage* dom = (DeadObjectMessage*)msg;
		if (dom->GetDeadObject() == this)
		{
			RenderComponent* rc = (RenderComponent*)GetComponent("render");
			rc->ShouldDraw(false);
			_alive = false;
		}
	}

	GameObject::OnMessage(msg);

}

/******************************************************************************************************************/

void Explosion::Reset()
{
	_alive = false;
	_scale = 0;

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(false);
}

/******************************************************************************************************************/

void Explosion::Spawn(const Vector4& pos)
{
	_position.x(pos.x());
	_position.y(pos.y());

	_scale = 0;

	_alive = true;

	LifeTimerComponent* c = (LifeTimerComponent*)GetComponent("lifetimer");
	c->ResetLifeTime();

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);
}

/******************************************************************************************************************/
