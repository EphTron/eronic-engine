#pragma once
#include "Scene.h"
#include "WordDisplay.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"

// Game classes
#include "Asteroid.h"
#include "Tank.h"
#include "EnemyPlayerTank.h"
#include "UFO.h"
#include "ScoreDisplay.h"
#include "WordDisplay.h"
#include "NetworkSystem.h"
#include "P2PNetworkManager.h"

class GamePlayScene :
	public Scene
{
	// Constants
public:
	static const int					NUM_ASTEROIDS = 5;
	static const int					RESTART_DELAY = 1; // 1 sec
	static const int					MIN_UFO_WAIT = 10; // 10 sec
	static const int					MAX_UFO_WAIT = 30; // 30 sec

	// Data
protected:
	PhysicsSystem						_physicsSystem;
	CollisionSystem						_collisionSystem;
	NetworkSystem						_networkSystem;
	eronic::P2PNetworkManager*			_networkManager;

	// Matrices
	glm::mat4							_IM;  // Identity matrix
	glm::mat4							_MVM; // ModelView matrix

	Tank*								_tank;
	UFO*								_ufo;
	ScoreDisplay*						_scoreDisplay;

	double								_restartCounter;
	double								_ufoTimer;
	int									_minAsteroids;

	// Structors
public:
	GamePlayScene();
	virtual ~GamePlayScene();


	// Gets/Sets
public:

	// Functions
public:

	// Setup
	virtual void Initialise();

	/// Respond to input
	virtual void OnKeyboard(int key, bool down);

	//virtual void OnMessage(Message* msg);

	virtual void HandleNetworkData(eronic::DataPackage * data);
	
	/// Update current scene
	virtual void Update(double deltaTime);

	/// Render current scene
	virtual void Render(RenderSystem* renderer);


	// Reset the game
	void Reset();
};

