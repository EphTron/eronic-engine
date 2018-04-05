#include "Ship.h"

const float Ship::ACCELERATION = 1.0f;
const float Ship::MAX_SPEED = 0.5f;

/******************************************************************************************************************/

Ship::Ship()
{
}

/******************************************************************************************************************/

Ship::~Ship()
{
}

/******************************************************************************************************************/

void Ship::Accelerate(float amt)
{
	Vector4 thrust(0, amt);
	thrust.rotate(_angle);
	_velocity += thrust;
}

/******************************************************************************************************************/

void Ship::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	// Cap speed
	_velocity.limitTo(MAX_SPEED);
}

/******************************************************************************************************************/