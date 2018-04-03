#include "Game.h"
#include "Window.h"
#include "Renderer.h"

/******************************************************************************************************************/

Game::Game()
{
	// Setup RNG and timer
	srand(time(NULL));
	_lastFrameTime = clock();
}

/******************************************************************************************************************/

Game::~Game()
{
}

/******************************************************************************************************************/

void Game::Initialise(Window* w)
{
	// Initialise parent
	_window = w;
	_renderer = w->GetRenderer();

}

/******************************************************************************************************************/

void Game::OnKeyboard(int key, bool down)
{
	_keyStates[key] = down;

	if (down) return; // Ignore key down events
}

/******************************************************************************************************************/

void Game::Run()
{
	// Get delta time
	double temp_time = clock();
	_deltaTime = (temp_time - _lastFrameTime) / CLOCKS_PER_SEC;
	_lastFrameTime = temp_time;


}

/******************************************************************************************************************/
