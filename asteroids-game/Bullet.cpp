#include "Bullet.h"

const float Bullet::BULLET_SPEED = 1.0f;
const float Bullet::MAX_LIFE = 1.0f;

/******************************************************************************************************************/

Bullet::Bullet()
{
	_alive = false;
}

/******************************************************************************************************************/

Bullet::~Bullet()
{
}

/******************************************************************************************************************/

void Bullet::Reset(const Vector4& pos, float angle)
{
	_position.x(pos.x());
	_position.y(pos.y());

	_angle = angle;

	_velocity.set(0, BULLET_SPEED, 0, 0);
	_velocity.rotate(angle);

	_alive = true;
	_life = MAX_LIFE;
}

/******************************************************************************************************************/

void Bullet::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	_life -= (float)deltaTime;

	if (_life < 0)
	{
		_alive = false;
	}
}

/******************************************************************************************************************/
