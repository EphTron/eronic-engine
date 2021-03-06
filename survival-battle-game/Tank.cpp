#include "Tank.h"
#include "CollisionMatrix.h"
#include "CollisionMessage.h"
#include "Components.h"
#include "NetworkComponent.h"
#include "DeadObjectMessage.h"
#include "Game.h"

const float Tank::MAX_SPEED = 0.5f;

/******************************************************************************************************************/

Tank::Tank(std::string & tag,Mesh* mesh, Mesh* cannon)
	: GameObject("tank")
{
	_id_tag = tag;

	// Create components for Ship (they will add themselves)
	new TankControllerComponent(this);
	new GodModeComponent(this);
	new ExplodableComponent(this);

	PhysicsComponent* pc = new PhysicsComponent(this);
	pc->SetMaxSpeed(MAX_SPEED);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::Ship_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Asteroid_ID);
	cc->SetCollisionMatrixFlag(CollisionID::UFO_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Missile_ID);

	NetworkComponent* nc = new NetworkComponent(this, tag);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetColour(Colour(0.5f, 0.5f, 1.0f, 1.0f));
	rc->SetMesh(mesh);
	rc->ShouldDraw(true);
}

/******************************************************************************************************************/

Tank::~Tank()
{
}

/******************************************************************************************************************/

void Tank::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	// Cap speed
	PhysicsComponent* physics = (PhysicsComponent*)GetComponent("physics");
	physics->LimitToMaximumSpeed(MAX_SPEED);
	physics->SlowDown(deltaTime, 0.5);
}

/******************************************************************************************************************/

void Tank::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "collision")
	{
		CollisionMessage* col = (CollisionMessage*)msg;
		if (col->GetCollidee() == this ||
			col->GetCollider() == this)
		{
			RenderComponent* rc = (RenderComponent*)GetComponent("render");
			rc->ShouldDraw(false);
			_alive = false;

			// Send out death message
			DeadObjectMessage dom(this);
			OnMessage(&dom);

			// Change game state
			Message msg("state=dead");
			Game::TheGame->ListenToMessage(&msg);
		}
	}

	GameObject::OnMessage(msg);
}

/******************************************************************************************************************/

void Tank::Reset()
{
	_alive = true;

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);

	PhysicsComponent* pc = (PhysicsComponent*)GetComponent("physics");
	pc->SetVelocity(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	GodModeComponent* gc = (GodModeComponent*)GetComponent("godmode");
	gc->SetGodMode(false);

	TankControllerComponent* sc = (TankControllerComponent*)GetComponent("input");
	sc->Reset();

	_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

/******************************************************************************************************************/
