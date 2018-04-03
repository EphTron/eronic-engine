#include "Asteroid.h"

const float Asteroid::MAX_DRIFT = 0.25f;

/******************************************************************************************************************/

Asteroid::Asteroid()
{
}

/******************************************************************************************************************/

Asteroid::~Asteroid()
{
}

/******************************************************************************************************************/

void Asteroid::Reset()
{
	float r = 0;
	r = rand() * 1.0f / RAND_MAX * 2 - 1;
	_velocity.x(MAX_DRIFT * r);

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

	r = rand() * 1.0f / RAND_MAX * 2 - 1;
	_velocity.y(MAX_DRIFT * r);

	_position.y(r);

	_scale = 4;
}

/******************************************************************************************************************/

void Asteroid::Reset(float scale, const Vector4& pos)
{
	float r = 0;
	r = rand() * 1.0f / RAND_MAX * 2 - 1;
	_velocity.x(MAX_DRIFT * r);

	r = rand() * 1.0f / RAND_MAX * 2 - 1;
	_velocity.y(MAX_DRIFT * r);

	_position.x(pos.x());
	_position.y(pos.y());

	float c = rand() * 0.5f / RAND_MAX + 0.25f;
	_colour.r(c);
	_colour.g(c);
	_colour.b(c);

	_scale = scale;
}

/******************************************************************************************************************/
