#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Game.h"
#include "SurvivalBattleGame.h"
#include "RenderSystem.h"
#include "Asteroid.h"
#include "Message.h"
#include "ExplodableComponent.h"
#include "Explosion.h"
#include "GameOverScene.h"
#include "PauseScene.h"

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

GamePlayScene::GamePlayScene()
	: _restartCounter(0),
	  _ufoTimer(MAX_UFO_WAIT),
	  _minAsteroids(NUM_ASTEROIDS),
	  _networkSystem("")
{
}

/******************************************************************************************************************/

GamePlayScene::~GamePlayScene()
{
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void GamePlayScene::Initialise()
{
	std::string tag = _sceneManager->GetGame()->GetNameTag();

	_networkManager = _sceneManager->GetGame()->GetNetworkManager();
	_networkSystem.SetNetworkManager(_networkManager);
	_networkSystem.set_tag(tag);
	
	std::stringstream sstm;
	sstm << tag << "_tank";
	tag = sstm.str();


	////////////////
	// Setup Objects
	_tank = new Tank(tag,
		_sceneManager->GetGame()->GetMesh("tank"), _sceneManager->GetGame()->GetMesh("tank"));
	_gameObjects.push_back(_tank);

	_ufo = new UFO(_sceneManager->GetGame()->GetMesh("ufo"));
	_gameObjects.push_back(_ufo);
	_ufo->SetAlive(false);

	// Create asteroids
	for (int i = 0; i < NUM_ASTEROIDS; i++)
	{
		Asteroid* asteroid = new Asteroid(_sceneManager->GetGame()->GetMesh("asteroid"));
		asteroid->Reset();
		_gameObjects.push_back(asteroid);
	}

	// Setup Score Display
	_scoreDisplay = new ScoreDisplay();
	_gameObjects.push_back(_scoreDisplay);

	// Start all objects to set them up
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		_gameObjects[i]->Start();
	}
}

/******************************************************************************************************************/

void GamePlayScene::OnKeyboard(int key, bool down)
{
	
	if (down) return; // Ignore key down events
	OutputDebugStringA("PLAY SCENE KEYBOARD");
	// Switch key presses
	switch (key)
	{
	case 80: // P = pause
		_sceneManager->PushScene(new PauseScene(_scoreDisplay->GetScore()));
		break;
	case 82: // R
		Reset();
		break;
	case 85: // U
		_ufoTimer = 0;
		break;
	case 27: // Escape
		_sceneManager->PopScene();
		break;
	}
}

void GamePlayScene::HandleNetworkData(eronic::DataPackage * data)
{
	if (data->type == 8) {
		GameObject * enemy_tank = new EnemyPlayerTank(_sceneManager->GetGame()->GetMesh("tank"), _sceneManager->GetGame()->GetMesh("tank"));
		_gameObjects.push_back(enemy_tank);
	}
}

/******************************************************************************************************************/

/// Update current scene
void GamePlayScene::Update(double deltaTime)
{
	Scene::Update(deltaTime);

	_networkSystem.Process(_gameObjects, deltaTime);

	_physicsSystem.Process(_gameObjects, deltaTime);
	_collisionSystem.Process(_gameObjects, deltaTime);

	// Perform legacy update on game objects (in theory, should be phased out altogether)
	int numAsteroids = 0;
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		if (_gameObjects[i]->IsAlive())
		{
			_gameObjects[i]->Update(deltaTime);
			if (_gameObjects[i]->GetType() == "asteroid")
			{
				numAsteroids++;
			}
		}
	}

	// Add more asteroids if we have less than 5
	while (numAsteroids < _minAsteroids)
	{
		Asteroid* a = new Asteroid(_sceneManager->GetGame()->GetMesh("asteroid"));
		a->Reset();
		_gameObjects.push_back(a);
		Asteroid* b = new Asteroid(_sceneManager->GetGame()->GetMesh("asteroid"));
		b->Reset();
		_gameObjects.push_back(b);		
		_minAsteroids++;
		numAsteroids += 2;
	}

	// Handle UFO
	if (_ufoTimer < 0 && _ufo->IsAlive() == false)
	{
		double rnd = rand() * 1.0 / RAND_MAX;
		_ufoTimer = rnd * (MAX_UFO_WAIT - MIN_UFO_WAIT) + MIN_UFO_WAIT;

		_ufo->SetAlive(true);
		_ufo->Reset();
	}
	else if (_ufo->IsAlive() == false)
	{
		_ufoTimer -= deltaTime;
	}

	// Change game state if necessary
	if (!_tank->IsAlive())
	{
		// Ship is dead
		if (ExplodableComponent* ec = (ExplodableComponent*)(_tank->GetComponent("explodable")))
		{
			if (!ec->GetExplosion()->IsAlive() && ec->HasExploded())
			{
				// Dead
				_sceneManager->PopScene();
				_sceneManager->PushScene(new GameOverScene(_scoreDisplay->GetScore()));
			}
		}
	}
}

/******************************************************************************************************************/

/// Render current scene
void GamePlayScene::Render(RenderSystem* renderer)
{
	glm::mat4 MVM;

	renderer->Process(_gameObjects, 0);
}

/******************************************************************************************************************/

void GamePlayScene::Reset()
{
	int numAsteroidsFound = 0;
	_minAsteroids = NUM_ASTEROIDS;

	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		// Delete excess asteroids
		if (_gameObjects[i]->GetType() == "asteroid")
		{
			numAsteroidsFound++;
			if (numAsteroidsFound > _minAsteroids)
			{
				delete _gameObjects[i];
				_gameObjects.erase(_gameObjects.begin() + i);
				i--;
				continue;
			}
		}

		// Reset
		_gameObjects[i]->Reset();
	}

	_ufo->SetAlive(false);
	_ufoTimer = MAX_UFO_WAIT;

}

/******************************************************************************************************************/
