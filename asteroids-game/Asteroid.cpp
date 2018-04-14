#include "Asteroid.h"
#include "CollisionMatrix.h"
#include "CollisionMessage.h"
#include "Components.h"
#include "DeadObjectMessage.h"
#include "Game.h"

const float Asteroid::MAX_DRIFT = 0.25f;
int Asteroid::count = 0;

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

Asteroid::Asteroid(Mesh* mesh)
	: GameObject("asteroid")
{
	// Create components for object (they will add themselves)
	PhysicsComponent* pc = new PhysicsComponent(this);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::Asteroid_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Ship_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetColour(Colour(0.5f, 0.5f, 0.5f, 1.0f));
	rc->SetMesh(mesh);
	rc->ShouldDraw(true);

	count++;
}

/******************************************************************************************************************/

Asteroid::~Asteroid()
{
	count--;
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void Asteroid::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

/******************************************************************************************************************/

void Asteroid::OnMessage(Message* msg)
{
	// If the object is dead, stop drawing it
	if (msg->GetMessageType() == "dead")
	{
		DeadObjectMessage* dead = (DeadObjectMessage*)msg;
		if (dead->GetDeadObject() == this)
		{
			RenderComponent* rc = (RenderComponent*)GetComponent("render");
			rc->ShouldDraw(false);
			_alive = false;

			// Spawn new asteroids if above scale 0.5
			if (_scale > 0.5f)
			{
				Asteroid* a = new Asteroid(rc->GetMesh());
				Asteroid* b = new Asteroid(rc->GetMesh());
				a->Spawn(_scale / 2, _position);
				b->Spawn(_scale / 2, _position);
				Game::TheGame->AddGameObject(a);
				Game::TheGame->AddGameObject(b);
			}

			// Delete this asteroid 
			SetDeleteFlag(true);
		}
	}
	else if (msg->GetMessageType() == "collision")
	{
		CollisionMessage* col = (CollisionMessage*)msg;
		if (col->GetCollidee() == this ||
			col->GetCollider() == this)
		{
			// Send out death message
			DeadObjectMessage dom(this);
			OnMessage(&dom);

			// Send increase score message
			if (col->GetOtherCollisionObject(this)->GetType() == "bullet")
			{
				Message msg("scoreUp");
				Game::TheGame->BroadcastMessage(&msg);
			}
		}
	}

	GameObject::OnMessage(msg);

}

/******************************************************************************************************************/

void Asteroid::Reset()
{
	float r = 0;
	r = rand() * 1.0f / RAND_MAX * 2 - 1;
	if (r < 0)
	{
		// Put on right side of screen
		_position.x(1);
	}
	else
	{
		// Put on left side
		_position.x(-1);
	}
	_position.y(r);

	_scale = 4;


	PhysicsComponent* physics = (PhysicsComponent*)GetComponent("physics");
	if (physics)
	{
		Vector4 velocity = physics->GetVelocity();

		velocity.x(MAX_DRIFT * r);

		r = rand() * 1.0f / RAND_MAX * 2 - 1;
		velocity.y(MAX_DRIFT * r);

		_position.y(r);

		physics->SetVelocity(velocity);
	}

	RenderComponent* render = (RenderComponent*)GetComponent("render");
	if (render)
	{
		Colour colour = render->GetColour();

		float c = rand() * 0.5f / RAND_MAX + 0.25f;
		colour.r(c);
		colour.g(c);
		colour.b(c);

		render->SetColour(colour);
	}
}

/******************************************************************************************************************/

void Asteroid::Spawn(float scale, const Vector4& pos)
{
	_scale = scale;

	PhysicsComponent* physics = (PhysicsComponent*)GetComponent("physics");
	if (physics)
	{
		Vector4 velocity = physics->GetVelocity();


		float r = 0;
		r = rand() * 1.0f / RAND_MAX * 2 - 1;
		velocity.x(MAX_DRIFT * r);

		r = rand() * 1.0f / RAND_MAX * 2 - 1;
		velocity.y(MAX_DRIFT * r);

		_position.x(pos.x());
		_position.y(pos.y());

		physics->SetVelocity(velocity);
	}

	RenderComponent* render = (RenderComponent*)GetComponent("render");
	if (render)
	{
		Colour colour = render->GetColour();
		float c = rand() * 0.5f / RAND_MAX + 0.25f;
		colour.r(c);
		colour.g(c);
		colour.b(c);

		render->SetColour(colour);
	}
}

/******************************************************************************************************************/
