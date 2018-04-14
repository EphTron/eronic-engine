#include "AsteroidsGame.h"
#include "Components.h"
#include "Message.h"
#include "MultiMesh.h"
#include "MainMenuScene.h"
#include <sstream>

/******************************************************************************************************************/

AsteroidsGame::AsteroidsGame()
{
}

/******************************************************************************************************************/

AsteroidsGame::~AsteroidsGame()
{
}

/******************************************************************************************************************/

void AsteroidsGame::Initialise(Window* w)
{
	// Initialise parent
	Game::Initialise(w);

	//////////////
	// Setup Meshes
	Mesh* meshShip = new Mesh();
	meshShip->LoadFromFile("ShipMesh.txt");
	AddMesh("ship", meshShip);

	Mesh* meshBullet = new Mesh();
	meshBullet->LoadFromFile("BulletMesh.txt");
	AddMesh("bullet", meshBullet);

	Mesh* meshAsteroid = new Mesh();
	for (int a = 0; a <= 360; a += 45)
	{
		float x = sinDeg(a) * 0.05f;
		float y = cosDeg(a) * 0.05f;
		meshAsteroid->AddVertex({ x, y, 0, 0.5f, 0.5f, 0.5f, 1 });
	}
	AddMesh("asteroid", meshAsteroid);

	Mesh* meshExplosion = new Mesh();
	meshExplosion->LoadFromFile("ExplosionMesh.txt");
	AddMesh("explosion", meshExplosion);

	Mesh* meshMissile = new Mesh();
	meshMissile->LoadFromFile("MissileMesh.txt");
	AddMesh("missile", meshMissile);

	Mesh* meshUFO= new Mesh();
	meshUFO->LoadFromFile("UFOMesh.txt");
	AddMesh("ufo", meshUFO);

	// Setup digit/letter meshes
	_numbers.LoadFromFile("DigitsMesh.txt");
	_letters.LoadFromFile("LettersMesh.txt");

	for (int i = 0; i < _numbers.NumMeshes(); i++)
	{
		std::stringstream num;
		num << i;
		AddMesh(num.str(), _numbers.GetMesh(num.str()));
	}
	char letter = 'A';
	for (int i = 0; i < _letters.NumMeshes(); i++)
	{
		std::stringstream num;
		num << letter++;
		AddMesh(num.str(), _letters.GetMesh(num.str()));
	}

	// Create VBOs
	for (MeshMapIterator i = _meshes.begin();
		i != _meshes.end();
		++i)
	{
		i->second->CreateVBO(_renderer);
	}

	_sceneManager.PushScene(new MainMenuScene());
}

/******************************************************************************************************************/

void AsteroidsGame::OnKeyboard(int key, bool down)
{
	Game::OnKeyboard(key, down);

	_sceneManager.OnKeyboard(key, down);

	//if (down) return; // Ignore key down events

	//// Switch key presses
	//switch (key)
	//{
	//case 80: // P
	//	if (_gameState == Playing)
	//	{
	//		_gameState = Paused;
	//	}
	//	else if (_gameState = Paused)
	//	{
	//		_gameState = Playing;
	//	}
	//	break;
	//case 82: // R
	//	Reset();
	//	break;
	//case 85: // U
	//	_ufoTimer = 0;
	//	break;
	//case 27: // Escape
	//	SetQuitFlag(true);
	//	break;
	//}
}

/******************************************************************************************************************/

void AsteroidsGame::Render()
{
	// Clear screen
	_renderer->ClearScreen();

	_sceneManager.Render(&_renderSystem);

	// Swap buffers
	_renderer->SwapBuffers();
}

/******************************************************************************************************************/

void AsteroidsGame::Run()
{
	// Run parent method to get delta time etc
	Game::Run();

	// Update scenes
	_sceneManager.Update(_deltaTime);

	// Check for exit
	if (_sceneManager.GetCurrentScene() == NULL)
	{
		SetQuitFlag(true);
	}

	// Draw
	Render();
}

/******************************************************************************************************************/
