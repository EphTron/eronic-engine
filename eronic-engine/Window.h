#pragma once

// Forward declarations
class Game;
class Renderer;


// Generic window class
// Handles window setup and input
class Window
{
	// Constants and statics
public:
	// Singleton (sort of)
	static Window*							TheWindow;

	// Data
public:
	Game*									_game;
	Renderer*								_renderer;
	int										_width, _height;


	// Structors
public:
	Window(Game* game, int width, int height);
	virtual ~Window();


	// Gets/sets
public:
	Game* GetGame()					const	{ return _game; }
	Renderer* GetRenderer()			const	{ return _renderer; }


	// Functions
public:
	virtual void Initialise()				= 0;
	
};

