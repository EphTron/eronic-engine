#include "PhysicsSystem.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

PhysicsSystem::PhysicsSystem()
{
}

/******************************************************************************************************************/

PhysicsSystem::~PhysicsSystem()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void PhysicsSystem::Process(std::vector<GameObject*>& list, double deltaTime)
{
	for (auto o : list)
	{
		// Does this object even have a Physics component?
		if (PhysicsComponent* pc = (PhysicsComponent*)(o->GetComponent("physics")))
		{
			Vector4 newpos = pc->GetObjectPosition();
			Vector4 vel = pc->GetVelocity();
			vel *= deltaTime;
			newpos += vel;

			// Wrap around
			if (newpos.x() < -1)		newpos.x(newpos.x() + 2);
			else if (newpos.x() > 1)	newpos.x(newpos.x() - 2);
			if (newpos.y() < -1)		newpos.y(newpos.y() + 2);
			else if (newpos.y() > 1)	newpos.y(newpos.y() - 2);

			pc->SetObjectPosition(newpos);

			// Limit speed
			if (float maxSpeed = pc->GetMaxSpeed() >= 0)
			{
				pc->LimitToMaximumSpeed(maxSpeed);
			}
		}
	}
}

/******************************************************************************************************************/
