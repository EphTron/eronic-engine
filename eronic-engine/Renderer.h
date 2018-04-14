#pragma once
#include "Colour.h"

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

// Forward declarations
class Game;
class RenderComponent;
class Mesh;

// Platform independent renderer base class
// Basically represents a graphics context and its active shaders
class Renderer
{
	// Data
public:
	Colour										_clearColour;	// Screen clear colour

	// Structors
public:
	Renderer()									;
	virtual ~Renderer()							;

	// Gets/sets
public:
	Colour GetClearColour()				const	{ return _clearColour; }
	void SetClearColour(Colour c)				{ _clearColour = c; }

	// Functions
public:
	virtual void ClearScreen() = 0;

	virtual void Draw(const Mesh* mesh, glm::mat4 MVM, const Colour& colour) = 0;
	virtual void Draw(RenderComponent* gob, glm::mat4 MVM);

	virtual void Destroy() = 0;

	virtual void Initialise(int width, int height) = 0;
	
	virtual void SwapBuffers() = 0;
};

