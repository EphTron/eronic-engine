#pragma once
#include "Scene.h"
#include "WordDisplay.h"
#include "PhysicsSystem.h"
#include "ScoreDisplay.h"

class PauseScene :
	public Scene
{
	// Constants
public:

	// Data
protected:
	PhysicsSystem						_physicsSystem;
	WordDisplay*						_title;
	ScoreDisplay*						_scoreDisplay;
	int									_score;

	// Structors
public:
	PauseScene(int score);
	virtual ~PauseScene();


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

