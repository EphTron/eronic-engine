#pragma once
#include "Scene.h"
#include "WordDisplay.h"
#include "PhysicsSystem.h"
#include "ScoreDisplay.h"

class GameOverScene :
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
	GameOverScene(int score);
	virtual ~GameOverScene();


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

