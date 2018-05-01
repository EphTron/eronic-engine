#pragma once
#include "GameObjectComponent.h"
#include "Vector4.h"

class NetworkComponent :
	public GameObjectComponent
{

	// Data
protected:
	std::string		_owner_tag;
	bool			_should_sent;	// Whether or not to draw

									// RenderComponent
public:
	NetworkComponent(GameObject* gob, std::string tag);
	virtual ~NetworkComponent();


	// Gets/Sets
public:
	std::string get_owner_tag()			const { return _owner_tag; }
	void set_owner_tag(std::string tag) { _owner_tag = tag; }
	bool got_sent()						const { return _should_sent; }
	void set_sent(bool flag) { _should_sent = flag; }

	// Functions
public:

	// Setup function -- called when parent object is initialised (using its own Start method)
	virtual void Start();

	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg);

	// Shutdown function -- called when parent object is destroyed
	virtual void End();

	// Extra Functions
	Vector4 GetObjectPosition();

	void SetObjectPosition(Vector4 v);
};

