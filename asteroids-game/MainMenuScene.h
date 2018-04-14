#pragma once
#include "Scene.h"
#include "WordDisplay.h"
#include "PhysicsSystem.h"

class MainMenuScene :
	public Scene
{
	// Constants
public:

	// Data
protected:
	PhysicsSystem						_physicsSystem;
	WordDisplay*						_title;

	// Structors
public:
	MainMenuScene();
	virtual ~MainMenuScene();


	// Gets/Sets
public:

	// Functions
public:

	// Setup
	virtual void Initialise();

	/// Respond to input
	virtual void OnKeyboard(int key, bool down);

	/// Update current scene
	virtual void Update(double deltaTime);

	/// Render current scene
	virtual void Render(RenderSystem* renderer);


};

