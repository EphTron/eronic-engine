
#include "SurvivalBattleGame.h"
#include "Components.h"
#include "Message.h"
#include "MultiMesh.h"
#include "MainMenuScene.h"
#include <sstream>

/******************************************************************************************************************/

SurvivalBattleGame::SurvivalBattleGame()
{
}

/******************************************************************************************************************/

SurvivalBattleGame::~SurvivalBattleGame()
{
}

/******************************************************************************************************************/

void SurvivalBattleGame::Initialise(Window* w)
{
	std::cout << "setup " << std::endl;
	int id = clock();
	std::stringstream sstm;
	_name = "Player_";
	sstm << _name << id;
	_name = sstm.str();
	_networkSystem.set_tag(_name);

	// Initialise parent
	Game::Initialise(w);

	TwBar *myBar;
	myBar = TwNewBar("Test");
	int* myvar = new int(10);
	TwAddVarRW(myBar, "NameOfMyVariable", TW_TYPE_INT32, myvar, "");

	//////////////
	// Setup Meshes
	Mesh* meshTank = new Mesh();
	meshTank->LoadFromFile("TankMesh.txt");
	AddMesh("tank", meshTank);

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
	std::cout << " setup all meshes" << std::endl;
	_sceneManager.PushScene(new MainMenuScene());
}

/******************************************************************************************************************/

void SurvivalBattleGame::OnKeyboard(int key, bool down)
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

void SurvivalBattleGame::HandleNetworkData(eronic::DataPackage * data)
{
}

/******************************************************************************************************************/

void SurvivalBattleGame::Render()
{
	// Clear screen
	_renderer->ClearScreen();

	_sceneManager.Render(&_renderSystem);

	
	// Swap buffers
	_renderer->SwapBuffers();
}

/******************************************************************************************************************/

void SurvivalBattleGame::Run()
{
	// Run parent method to get delta time etc
	Game::Run();

	//_networkManager->read_data();

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
