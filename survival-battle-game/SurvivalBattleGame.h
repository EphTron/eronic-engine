#pragma once
#include "Game.h"

// STL
#include <vector>

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

// Engine classes
#include "Renderer.h"
#include "Window.h"
#include "MultiMesh.h"

// Simple game state enum for the Asteroids game
enum GameState
{
	MainMenu,
	Playing,
	Paused,
	Dead,
	Resetting,
	GameOver
};


class SurvivalBattleGame :
	public Game
{
	// Constants and statics
public:


	// Data
private:

	// Meshes
	MultiMesh							_numbers;
	MultiMesh							_letters;

	// Structors
public:
	SurvivalBattleGame();
	virtual ~SurvivalBattleGame();

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

};

