#include "Missile.h"
#include "Components.h"
#include "CollisionMessage.h"
#include "DeadObjectMessage.h"
#include "CollisionMatrix.h"
#include "Game.h"
#include "Vector4.h"

const float Missile::MISSILE_SPEED = 0.5f;
const float Missile::MAX_LIFE = 3.0f;
const float Missile::TURN_RATE = 90.0f;

/******************************************************************************************************************/

Missile::Missile(Mesh* mesh)
	: GameObject("Missile")
{
	// Create components for object (they will add themselves)
	new PhysicsComponent(this);
	new LifeTimerComponent(this, MAX_LIFE);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetCollisionRadius(mesh->CalculateMaxSize() * 0.9f);
	cc->SetCollisionID(CollisionID::Missile_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Ship_ID);
	cc->SetCollisionMatrixFlag(CollisionID::Bullet_ID);

	RenderComponent* rc = new RenderComponent(this);
	rc->SetMesh(mesh);
	rc->ShouldDraw(false);
}

/******************************************************************************************************************/

Missile::~Missile()
{
}

/******************************************************************************************************************/

void Missile::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	if (_target)
	{
		// Aim at target
		Vector4 vel(0, MISSILE_SPEED, 0, 0);
		float bearing = _position.bearingTo(_target->GetPosition());
		if (bearing > 180) bearing = -(360 - bearing);
		float tempAngle = _angle;
		if (tempAngle > 180) tempAngle = -(360 - tempAngle);
		 
		float diff = bearing - tempAngle;

		// Normalise again
		if (diff > 180) diff -= 360;
		if (diff < -180) diff += 360;

		if (diff > TURN_RATE * deltaTime)
		{
			_angle += (float)(TURN_RATE * deltaTime);
		}
		else if (diff < -TURN_RATE * deltaTime)
		{
			_angle -= (float)(TURN_RATE * deltaTime);
		}
		else
		{
			_angle = bearing;
		}
		vel.rotate(_angle);
		PhysicsComponent* p = (PhysicsComponent*)GetComponent("physics");
		p->SetVelocity(vel);
	}
	else
	{
		FindValidTarget();
	}
}

/******************************************************************************************************************/

void Missile::OnMessage(Message* msg)
{
	// If the Missile's life is expired, stop drawing it
	if (msg->GetMessageType() == "dead")
	{
		DeadObjectMessage* dead = (DeadObjectMessage*)msg;
		if (dead->GetDeadObject() == this)
		{
			RenderComponent* rc = (RenderComponent*)GetComponent("render");
			rc->ShouldDraw(false);
			SetAlive(false);
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

void Missile::Reset()
{
	_alive = false;

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(false);

	_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

/******************************************************************************************************************/

void Missile::Spawn(const Vector4& pos)
{
	_position.x(pos.x());
	_position.y(pos.y());

	_alive = true;

	// Update components

	LifeTimerComponent* c = (LifeTimerComponent*)GetComponent("lifetimer");
	c->ResetLifeTime();

	RenderComponent* rc = (RenderComponent*)GetComponent("render");
	rc->ShouldDraw(true);

	// Find Ship as target
	_target = NULL;
	FindValidTarget();

	// Aim at target
	Vector4 vel(0, MISSILE_SPEED, 0, 0);
	if (_target != NULL)
	{
		float bearing = _position.bearingTo(_target->GetPosition());
		_angle = bearing;
		vel.rotate(bearing);
	}
	PhysicsComponent* p = (PhysicsComponent*)GetComponent("physics");
	p->SetVelocity(vel);
}

/******************************************************************************************************************/

GameObject* Missile::FindValidTarget()
{
	// Find the nearest valid target
	_target = NULL;
	float dist2 = FLT_MAX;
	std::vector<GameObject*>& objects = Game::TheGame->GetGameObjects();
	for (int i = 0; i < (int)objects.size(); i++)
	{
		if (objects[i]->GetType() == _validTargetType)
		{
			Vector4 opos = objects[i]->GetPosition();
			opos -= _position;
			float d2 = opos.lengthSquared();
			if (d2 < dist2)
			{
				SetTarget(objects[i]);
				dist2 = d2;
			}
		}
	}
	return _target;
}

/******************************************************************************************************************/
