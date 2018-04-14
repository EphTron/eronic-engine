#pragma once
#include <stack>
#include <vector>

class RenderSystem;
class Scene;
class Game;
class GameObject;
class Message;

/// Handles the Scenes for a game
class SceneManager
{
	// Constants
public:

	// Data
protected:
	Game*				_game;
	std::stack<Scene*>	_scenes;

	// Structors
public:
	SceneManager(Game* _game);
	~SceneManager();


	// Gets/Sets
public:
	Scene* GetCurrentScene()	const	{ if (_scenes.size() > 0) return _scenes.top(); else return NULL; }
	Game* GetGame()				const	{ return _game; }

	// Functions
public:
	// Game object handling
	void AddGameObject(GameObject* obj) ;
	std::vector<GameObject*>& GetGameObjects();

	/// Respond to input
	void OnKeyboard(int key, bool down)	;

	/// Respond to messages
	void OnMessage(Message* msg)		;

	/// Update current scene
	void Update(double deltaTime)		;

	/// Render current scene
	void Render(RenderSystem* renderer)	;

	/// Pop the top scene. If no scenes remain, we should quit.
	void PopScene()						{ _scenes.pop(); }

	/// Push a new scene
	void PushScene(Scene* s)			;
};

