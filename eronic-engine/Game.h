#pragma once
#include <time.h>
#include <map>
#include <vector>

// Systems
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "NetworkSystem.h"

#include "SceneManager.h"
#include "P2PNetworkManager.h"
#include "NetworkManager.h"

// Forward declarations
class GameObject;
class Mesh;
class Message;
class Renderer;
class Window;

// Typedefs
typedef std::map<std::string, Mesh*> MeshMap;
typedef std::map<std::string, Mesh*>::iterator MeshMapIterator;


// Generic game class
class Game
{
	// "Singleton"
public:
	static Game* TheGame;

	// Data
protected:
	std::string						_name;
	double							_currentTime;		// Current time for delta time purposes
	double							_deltaTime;			// Time since last frame
	bool							_keyStates[256];	// Keyboard status
	bool							_quitFlag;			// Quit flag; when true, we quit
	Renderer*						_renderer;			// The renderer
	Window*							_window;			// The game window

	// Meshes
	MeshMap							_meshes;			// The map of meshes

	// Systems
	RenderSystem					_renderSystem;		// To handle rendering
	NetworkSystem					_networkSystem;		// To handle network communication

	// Scene Manager
	SceneManager					_sceneManager;

	// Network Manager				
	eronic::P2PNetworkManager*		_networkManager;
	std::thread						_network_manager_thread;
	//eronic::NetworkManager*		_networkManager;

	// Structors
public:
	Game();
	virtual ~Game();


	// Gets/sets
public:
	// Meshes
	Mesh* GetMesh(std::string name)					;
	void AddMesh(std::string name, Mesh* mesh)		{ _meshes[name] = mesh; }

	// Quit flag
	bool GetQuitFlag()						const	{ return _quitFlag; }
	void SetQuitFlag(bool v)						{ _quitFlag = v; }

	std::string const& GetNameTag() const { return _name; };

	// Renderer
	Renderer* GetRenderer()							const { return _renderer; }
	eronic::P2PNetworkManager* GetNetworkManager()	const { return _networkManager; }

	// Functions
public:
	void AddGameObject(GameObject* obj)				{ _sceneManager.AddGameObject(obj); }
	std::vector<GameObject*>& GetGameObjects()		{ return _sceneManager.GetGameObjects(); }

	// Initialise game
	virtual void Initialise(Window* w);

	virtual void RunNetwork(bool flag);

	// Keyboard input
	virtual void OnKeyboard(int key, bool down);

	// Keyboard input
	virtual void HandleNetworkData(eronic::DataPackage* data);

	// Draw everything
	virtual void Render() = 0;

	// Main game loop (update)
	virtual void Run();

	// Message system
	void BroadcastMessage(Message* msg);

	// The game can respond to messages too
	virtual void ListenToMessage(Message* msg)		{}
};


inline Mesh* Game::GetMesh(std::string name)
{
	// Found
	MeshMapIterator i = _meshes.find(name);
	if (i != _meshes.end())
	{
		return i->second;
	}
	// Not found
	return NULL;
}