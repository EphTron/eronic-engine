#pragma once
#include "Game.h"

// STL
#include <vector>
#include <time.h>
#include <stdlib.h>

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

// Game classes
#include "Ship.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Asteroid.h"
#include "ScoreDisplay.h"
#include "TCPListener.h"
#include "TCPClient.h"

// Forward declarations

// Simple game state enum for the Asteroids game
enum GameState
{
	Playing,
	Paused,
	Dead,
	Resetting,
	Quit
};


class AsteroidsGame : 
	public Game
{
	// Constants and statics
public:
	static const int		NUM_ASTEROIDS = 5;
	static const int		NUM_BULLETS = 5;

	// Data
private:

	// Networking
	eronic::TCPListener _listener;
	eronic::TCPClient  _sender;


	// Matrices
	glm::mat4				_IM;  // Identity matrix
	glm::mat4				_MVM; // ModelView matrix

	// Meshes
	Mesh					_meshShip;
	Mesh					_meshAsteroid;
	Mesh					_meshBullet;
	Mesh					_meshExplosion;

	// Game Data
	Ship*					_ship;
	Explosion*				_explosion;
	std::vector<Bullet*>	_bullets;
	std::vector<Asteroid*>	_asteroids;

	ScoreDisplay*			_scoreDisplay;

	int						_score;
	float					_scoreCountdown;
	float					_fader;
	float					_faderInc;
	bool					_godMode;
	GameState				_gameState;

	// Structors
public:
	AsteroidsGame();
	virtual ~AsteroidsGame();

	// Accessors
public:

	GameState GetGameState()				const	{ return _gameState; }

	virtual bool IsRunning()				const	{ return _gameState != Quit; }

	// Functions
public:

	// Initialise the game
	virtual void Initialise(Window* w);

	// Keyboard input
	virtual void OnKeyboard(int key, bool down);

	// Render everything
	virtual void Render();

	// Main game loop
	virtual void Run();

	// Reset the game
	void Reset();
};

