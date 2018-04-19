#include "UFO.h"
#include "CollisionMatrix.h"
#include "CollisionMessage.h"
#include "Components.h"
#include "DeadObjectMessage.h"
#include "DamageMessage.h"
#include "Game.h"
#include "Missile.h"

const float UFO::MAX_SPEED = 0.25f;
const float UFO::FIRE_DELAY = 3.00f;

/******************************************************************************************************************/

UFO::UFO(Mesh* mesh)
	: GameObject("UFO")
{
	// Create components for UFO (they will add themselves)
	new PhysicsComponent(this);
	new ExplodableComponent(this);

	// TODO: Add HitpointComponent here

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::UFO_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Ship_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Bullet_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetMesh(mesh);
	rc->ShouldDraw(false);

	HitpointComponent* hc = new HitpointComponent(this);
	hc->SetMaxHitPoints(3);
	hc->SetHitPoints(3);

	ExplodableComponent* ec = new ExplodableComponent(this);

	_alive = false;
}

/******************************************************************************************************************/

UFO::~UFO()
{
}

/******************************************************************************************************************/

void UFO::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	_fireTimer -= deltaTime;
	if (_fireTimer < 0)
	{
		_fireTimer = FIRE_DELAY;

		// Fire a missile
		Missile* missile = new Missile(Game::TheGame->GetMesh("missile"));
		missile->SetValidTargetType("ship");
		missile->Spawn(_position);
		Game::TheGame->AddGameObject(missile);
	}
}

/******************************************************************************************************************/

void UFO::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "collision")
	{
		CollisionMessage* col = (CollisionMessage*)msg;
		if (col->GetCollidee() == this ||
			col->GetCollider() == this)
		{
			// TODO: Send out damage message instead of death message; increase score when the hitpoints 
			// reach 0 instead (inside the HitpointComponent)

			DamageMessage dmsg(this);
			OnMessage(&dmsg);

			// Send increase score message
			if (col->GetOtherCollisionObject(this)->GetType() == "bullet")
			{
				Message msg("scoreUp");
				Game::TheGame->BroadcastMessage(&msg);
			}
		}
	}
	else if (msg->GetMessageType() == "dead")
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

void UFO::Reset()
{
	_alive = true;
	_fireTimer = FIRE_DELAY;

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);

	PhysicsComponent* pc = (PhysicsComponent*)GetComponent("physics");

	double rnd = rand() * 1.0f / RAND_MAX;

	if (rnd < 0.25)
	{
		_position = Vector4(0.99f, 0.75f, 0.0f, 1.0f);
		pc->SetVelocity(Vector4(-MAX_SPEED, 0.0f, 0.0f, 0.0f));
	}
	else if (rnd < 0.5)
	{
		_position = Vector4(0.99f, -0.75f, 0.0f, 1.0f);
		pc->SetVelocity(Vector4(-MAX_SPEED, 0.0f, 0.0f, 0.0f));
	}
	else if (rnd < 0.75)
	{
		_position = Vector4(-0.99f, 0.75f, 0.0f, 1.0f);
		pc->SetVelocity(Vector4(MAX_SPEED, 0.0f, 0.0f, 0.0f));
	}
	else
	{
		_position = Vector4(-0.99f, -0.75f, 0.0f, 1.0f);
		pc->SetVelocity(Vector4(MAX_SPEED, 0.0f, 0.0f, 0.0f));
	}

	HitpointComponent* hc = (HitpointComponent*)GetComponent("hitpoints");
	if (hc)
	{
		// TODO: Reset the hitpoint component here
		hc->SetHitPoints(3);
	}
}

/******************************************************************************************************************/
