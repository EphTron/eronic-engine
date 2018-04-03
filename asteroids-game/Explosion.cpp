#include "Explosion.h"

const float Explosion::LIFETIME = 1.0f;
const float Explosion::MAX_SIZE = 1.0f;

/******************************************************************************************************************/

Explosion::Explosion()
{
	_alive = false;
}

/******************************************************************************************************************/

Explosion::~Explosion()
{
}

/******************************************************************************************************************/

void Explosion::Reset(const Vector4& pos)
{
	_position.x(pos.x());
	_position.y(pos.y());

	_scale = 0;

	_alive = true;
	_life = LIFETIME;
}

/******************************************************************************************************************/

void Explosion::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	_life -= (float)deltaTime;

	if (_life < 0)
	{
		_alive = false;
	}

	// Rotate (just for fun)
	_angle += 180 * deltaTime;

	// Scale according to lifetime
	float percentLifeLeft = 1 - (_life / LIFETIME);
	_scale = MAX_SIZE * percentLifeLeft;
}

/******************************************************************************************************************/
