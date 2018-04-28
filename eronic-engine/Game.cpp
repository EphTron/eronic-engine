#include "Game.h"
#include "Window.h"
#include "GameObject.h"
#include "KeyPressMessage.h"

Game* Game::TheGame = NULL;

/******************************************************************************************************************/

Game::Game()
	: _quitFlag(false),
	  _sceneManager(this),
	  _networkSystem(),
	  _networkManager(new eronic::P2PNetworkManager(1, 9173, 10, true))
{

	_networkSystem.SetNetworkManager(_networkManager);

	for (int i = 0; i < 256; i++) _keyStates[i] = false;
	_currentTime = clock();

	TheGame = this;
}

/******************************************************************************************************************/

Game::~Game()
{
	for (MeshMapIterator i = _meshes.begin();
		i != _meshes.end();
		++i)
	{
		delete i->second;
	}
	_meshes.clear();
}

/******************************************************************************************************************/

void Game::Initialise(Window* w)
{
	_window = w;
	_renderer = w->GetRenderer();
	
	_renderSystem.SetRenderer(_renderer);
}

/******************************************************************************************************************/

void Game::OnKeyboard(int key, bool down)
{
	_keyStates[key] = down;

	// Create keypress message and send it to all objects
	KeyPressMessage msg(key, down);

	BroadcastMessage(&msg);
}

/******************************************************************************************************************/

void Game::Run()
{
	// Get delta time
	double temp_time = clock();
	_deltaTime = (temp_time - _currentTime) / CLOCKS_PER_SEC;
	_currentTime = temp_time;


}

/******************************************************************************************************************/

void Game::BroadcastMessage(Message* msg)
{
	ListenToMessage(msg);

	_sceneManager.OnMessage(msg);
}

/******************************************************************************************************************/
