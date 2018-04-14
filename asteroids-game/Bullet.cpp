#include "Bullet.h"
#include "Components.h"
#include "CollisionMessage.h"
#include "DeadObjectMessage.h"
#include "CollisionMatrix.h"

const float Bullet::BULLET_SPEED = 1.0f;
const float Bullet::MAX_LIFE = 1.0f;

/******************************************************************************************************************/

Bullet::Bullet(Mesh* mesh)
	: GameObject("bullet")
{
	// Create components for object (they will add themselves)
	new PhysicsComponent(this);
	new LifeTimerComponent(this, MAX_LIFE);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::Bullet_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Asteroid_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Missile_ID);
	cc->SetCollisionMatrixFlag(CollisionID::UFO_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetColour(Colour(1.0f, 0.5f, 0.5f, 1.0f));
	rc->SetMesh(mesh);
	rc->ShouldDraw(false);
}

/******************************************************************************************************************/

Bullet::~Bullet()
{
}

/******************************************************************************************************************/

void Bullet::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

/******************************************************************************************************************/

void Bullet::OnMessage(Message* msg)
{
	// If the bullet's life is expired, stop drawing it
	if (msg->GetMessageType() == "dead")
	{
		DeadObjectMessage* dead = (DeadObjectMessage*)msg;
		if (dead->GetDeadObject() == this)
		{
			RenderComponent* rc = (RenderComponent*)GetComponent("render");
			rc->ShouldDraw(false);
			_alive = false;
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
		}
	}

	GameObject::OnMessage(msg);

}

/******************************************************************************************************************/

void Bullet::Reset()
{
	_alive = false;

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(false);

	_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

/******************************************************************************************************************/

void Bullet::Spawn(const Vector4& pos, float angle)
{
	_position.x(pos.x());
	_position.y(pos.y());

	_angle = angle;

	_alive = true;

	// Update components

	Vector4 vel(0, BULLET_SPEED, 0, 0);
	vel.rotate(angle);
	PhysicsComponent* p = (PhysicsComponent*)GetComponent("physics");
	p->SetVelocity(vel);

	LifeTimerComponent* c = (LifeTimerComponent*)GetComponent("lifetimer");
	c->ResetLifeTime();

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);
}

/******************************************************************************************************************/
