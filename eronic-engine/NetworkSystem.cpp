#include "NetworkSystem.h"
#include "GameObject.h"
#include "NetworkComponent.h"
#include "PhysicsComponent.h"



NetworkSystem::NetworkSystem(std::string tag) :
	_tag(tag)
{
}


NetworkSystem::~NetworkSystem()
{
}

void NetworkSystem::Process(std::vector<GameObject*>& list, double deltaTime)
{
	OutputDebugStringA("P\n");
	for (auto o : list)
	{
		// Does this object even have a Physics component?
		if (NetworkComponent* nc = (NetworkComponent*)(o->GetComponent("network")))
		{
			if (nc->GetGameObject()->GetType() == "tank") {
				/*char buffer[100];
				sprintf_s(buffer, "My 1 variable is %s\n", _tag.c_str());
				OutputDebugStringA(buffer);
				sprintf_s(buffer, "My 2 variable is %s\n", nc->get_owner_tag().c_str());
				OutputDebugStringA(buffer);
				OutputDebugStringA("FOUND TANK");*/
				// check if opbject 
				if (strncmp(nc->get_owner_tag().c_str(), _tag.c_str(), _tag.size()) == 0) {
					OutputDebugStringA(" TANK IS OWNED");
					Vector4 pos = nc->GetObjectPosition();
					float angle = nc->GetGameObject()->GetAngle();
					PhysicsComponent* pc = (PhysicsComponent*)(o->GetComponent("physics"));
					Vector4 velo = pc->GetVelocity();

					_network_manager->broadcast_message(10, 0, 0, 0, pos.x(), pos.y(), pos.z(), angle, velo.x(), velo.y(), velo.z(), _tag);
				}
			}
			//_owner_tag
		//Vector4 newpos = pc->GetObjectPosition();
		//Vector4 vel = pc->GetVelocity();
		//vel *= deltaTime;
		//newpos += vel;

		//// Wrap around
		//if (newpos.x() < -1)		newpos.x(newpos.x() + 2);
		//else if (newpos.x() > 1)	newpos.x(newpos.x() - 2);
		//if (newpos.y() < -1)		newpos.y(newpos.y() + 2);
		//else if (newpos.y() > 1)	newpos.y(newpos.y() - 2);

		//pc->SetObjectPosition(newpos);

		//// Limit speed
		//if (float maxSpeed = pc->GetMaxSpeed() >= 0)
		//{
		//	pc->LimitToMaximumSpeed(maxSpeed);
		//}
		}
	}
}
