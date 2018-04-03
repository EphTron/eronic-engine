#pragma once

// STL
#include <vector>
#include <time.h>
#include <stdlib.h>

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

// Engine classes
#include "Renderer.h"
#include "Window.h"

// Forward declarations
class Renderer;
class Window;

class Game
{
	// Constants and statics
public:

	// Data
protected:
	Window*					_window;
	Renderer*				_renderer;

	double					_lastFrameTime;		// Time of last frame
	double					_deltaTime;			// Time since last frame
	bool					_keyStates[256];	// Keyboard status

	// Structors
public:
	Game();
	virtual ~Game();

	// Accessors
public:

	virtual bool IsRunning() const = 0;

	// Functions
public:

	// Initialise the game
	virtual void Initialise(Window* w);

	// Keyboard input
	virtual void OnKeyboard(int key, bool down);

	// Render everything
	virtual void Render() = 0;

	// Main game loop
	virtual void Run();

};

