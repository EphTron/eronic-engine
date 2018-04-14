#include "CollisionSystem.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "CollisionMessage.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

CollisionSystem::CollisionSystem()
{
}

/******************************************************************************************************************/

CollisionSystem::~CollisionSystem()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void CollisionSystem::Process(std::vector<GameObject*>& list, double deltaTime)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		GameObject* collidee = list[i];

		if (!collidee->IsAlive()) continue; // Skip dead things

		// Does this object even have a Collision component?
		if (CollisionComponent* cc1 = (CollisionComponent*)(collidee->GetComponent("collision")))
		{
			if (cc1->GetCollisionID() == 0) continue; // Skip things with no collision ID

			// Collide against all objects we're meant to collide with
			for (size_t j = 0; j < list.size(); j++)
			{
				GameObject* collider = list[j];

				if (collidee == collider) continue; // Skip colliding against yourself
				if (!collider->IsAlive()) continue; // Skip dead things

				if (CollisionComponent* cc2 = (CollisionComponent*)(collider->GetComponent("collision")))
				{
					if (cc2->GetCollisionID() == 0) continue; // Skip things with no collision ID


					// Check collision matrix
					if (cc1->CheckCollisionMatrixFlag(cc2->GetCollisionID()))
					{
						// Collide with it...
						if (CollideWith(cc1, cc2))
						{
							// Send a collision message to both objects
							CollisionMessage msg(collidee, collider);
							collidee->OnMessage(&msg);
							collider->OnMessage(&msg);

							goto NextCollision;
						}
					}
				}
			}
		}
	NextCollision:
		;
	}
}

/******************************************************************************************************************/

bool CollisionSystem::CollideWith(const CollisionComponent* collidee, const CollisionComponent* collider)
{
	Vector4 diff = collider->GetGameObject()->GetPosition();
	diff -= collidee->GetGameObject()->GetPosition();

	return (diff.length() < collidee->GetCollisionRadius() + collider->GetCollisionRadius());
}

/******************************************************************************************************************/
