#include "AsteroidsGame.h"
#include "Window.h"
#include "Renderer.h"

/******************************************************************************************************************/

AsteroidsGame::AsteroidsGame()
{
	// Setup RNG and timer
	srand(time(NULL));
	_lastFrameTime = clock();
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

	///////////////
	// Vars
	_score = 0;
	_fader = 0;
	_faderInc = 1;
	_godMode = false;

	//////////////
	// Setup Meshes
	_meshShip.AddVertex({ 0, 0.05f, 0, 1, 1, 1, 1 });
	_meshShip.AddVertex({ 0.03f, -0.05f, 0, 1, 1, 1, 1 });
	_meshShip.AddVertex({ -0.03f, -0.05f, 0, 1, 1, 1, 1 });
	_meshShip.AddVertex({ 0, 0.05f, 0, 1, 1, 1, 1 });

	_meshBullet.AddVertex({ -0.01f, -0.01f, 0, 1, 1, 1, 1 });
	_meshBullet.AddVertex({ 0.01f, -0.01f, 0, 1, 1, 1, 1 });
	_meshBullet.AddVertex({ 0.01f, 0.01f, 0, 1, 1, 1, 1 });
	_meshBullet.AddVertex({ -0.01f, 0.01f, 0, 1, 1, 1, 1 });
	_meshBullet.AddVertex({ -0.01f, -0.01f, 0, 1, 1, 1, 1 });

	for (int a = 0; a <= 360; a += 45)
	{
		float x = sinDeg(a) * 0.05f;
		float y = cosDeg(a) * 0.05f;
		_meshAsteroid.AddVertex({ x, y, 0, 0.5f, 0.5f, 0.5f, 1 });
	}

	_meshExplosion.AddVertex({ 0.0f, 0.25f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ 0.05f, 0.10f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ 0.20f, 0.20f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ 0.10f, 0.05f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ 0.25f, 0.0f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ 0.10f, -0.05f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ 0.20f, -0.20f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ 0.05f, -0.10f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ 0.0f, -0.25f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ -0.05f, -0.10f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ -0.20f, -0.20f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ -0.10f, -0.05f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ -0.25f, -0.0f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ -0.10f, 0.05f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ -0.20f, 0.20f, 0, 1, 1, 0, 1 });
	_meshExplosion.AddVertex({ -0.05f, 0.10f, 0, 1, 0, 0, 1 });
	_meshExplosion.AddVertex({ 0.0f, 0.25f, 0, 1, 1, 0, 1 });

	_meshShip.CreateVBO(_renderer);
	_meshBullet.CreateVBO(_renderer);
	_meshAsteroid.CreateVBO(_renderer);
	_meshExplosion.CreateVBO(_renderer);

	////////////////
	// Setup Objects
	_ship = new Ship();
	_ship->SetMesh(&_meshShip);
	_ship->SetColour(Colour(0.5f, 0.5f, 1.0f, 1.0f));

	// Create bullets
	for (int i = 0; i < NUM_BULLETS; i++)
	{
		Bullet* b = new Bullet();
		b->SetMesh(&_meshBullet);
		b->SetColour(Colour(1.0f, 0.5f, 0.5f, 1.0f));
		_bullets.push_back(b);
	}

	// Create asteroids
	for (int i = 0; i < NUM_ASTEROIDS; i++)
	{
		Asteroid* asteroid = new Asteroid();
		asteroid->SetMesh(&_meshAsteroid);
		asteroid->Reset();
		_asteroids.push_back(asteroid);
	}

	_explosion = new Explosion();
	_explosion->SetMesh(&_meshExplosion);

	// Setup Score Display
	_scoreDisplay = new ScoreDisplay(_renderer);

	_gameState = Playing;
}

/******************************************************************************************************************/

void AsteroidsGame::OnKeyboard(int key, bool down)
{
	Game::OnKeyboard(key, down);
	
	if (down) return; // Ignore key down events

	// Switch key presses
	switch (key)
	{
	case 27: // Esc
		_gameState = Quit;
		break;
	case 17: // Ctrl
	case 32: // Space
		// FIRE
		if (_ship->IsAlive() &&
			_gameState == Playing)
		{
			for (int i = 0; i < (int)_bullets.size(); i++)
			{
				if (!_bullets[i]->IsAlive())
				{
					_bullets[i]->Reset(_ship->GetPosition(), _ship->GetAngle());
					break;
				}
			}
		}
		break;
	case 71: // G
		_godMode = !_godMode;
		break;
	case 80: // P
		if (_gameState == Playing)
		{
			_gameState = Paused;
		}
		else if (_gameState = Paused)
		{
			_gameState = Playing;
		}
		break;
	case 82: // R
		Reset();
		break;
	}
}

/******************************************************************************************************************/

void AsteroidsGame::Render()
{
	// Clear screen
	_renderer->ClearScreen();


	// Update MVM
	_MVM = _IM;
	_MVM = glm::translate(_MVM, glm::vec3(0, 0, 0));

	// Set global colour
	if (!_godMode)
	{
		_ship->SetColour(Colour(0.5f, 0.5f, 1.00f, 1.00f));
	}
	else
	{
		_ship->SetColour(Colour(_fader, _fader, 1.00f, 1.00f));
	}

	// Draw things
	if (_ship->IsAlive())
	{
		_renderer->Draw(_ship, _MVM);
	}

	for (int i = 0; i < (int)_bullets.size(); i++)
	{
		if (_bullets[i]->IsAlive())
		{
			_renderer->Draw(_bullets[i], _MVM);
		}
	}

	for (int i = 0; i < (int)_asteroids.size(); i++)
	{
		_renderer->Draw(_asteroids[i], _MVM);
	}

	if (_explosion->IsAlive())
	{
		_renderer->Draw(_explosion, _MVM);
	}

	_MVM = glm::translate(_MVM, glm::vec3(-0.95f, 0.90f, 0));
	_scoreDisplay->Draw(_renderer, _score, _MVM);

	// Swap buffers
	_renderer->SwapBuffers();
}

/******************************************************************************************************************/

void AsteroidsGame::Reset()
{
	_ship->SetPosition(Vector4());
	_ship->SetVelocity(Vector4());

	for (int i = 0; i < (int)_asteroids.size(); i++)
	{
		_asteroids[i]->Reset();
		if (i >= 5)
		{
			delete _asteroids[i];
			_asteroids.erase(_asteroids.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < (int)_bullets.size(); i++)
	{
		_bullets[i]->SetAlive(false);
	}

	_explosion->SetAlive(false);
	_ship->SetAlive(true);

	_score = 0;

	_gameState = Playing;
}

/******************************************************************************************************************/

void AsteroidsGame::Run()
{
	// Get delta time
	Game::Run();

	// Run main game states
	if (_gameState != Paused)
	{
		// Update ship
		if (_ship->IsAlive())
		{
			// Move ship
			if (_keyStates[37])	// Left
			{
				_ship->TurnLeft(180.0f * _deltaTime);
			}
			if (_keyStates[39])	// Right
			{
				_ship->TurnRight(180.0f * _deltaTime);
			}
			if (_keyStates[38])	// Up
			{
				_ship->Accelerate(Ship::ACCELERATION * _deltaTime);
			}

			_ship->Update(_deltaTime);
		}

		// Update bullets
		for (int i = 0; i < (int)_bullets.size(); i++)
		{
			if (_bullets[i]->IsAlive())
			{
				_bullets[i]->Update(_deltaTime);
			}
		}

		// Update asteroids
		for (int i = 0; i < (int)_asteroids.size(); i++)
		{
			_asteroids[i]->Update(_deltaTime);

			// Collision detections
			if (!_godMode &&
				_ship->IsAlive() &&
				_asteroids[i]->CollideWith(_ship, 0.05f * _asteroids[i]->GetScale()))
			{
				// Boom :(
				_ship->SetAlive(false);
				_explosion->Reset(_ship->GetPosition());
				_gameState = Dead;
			}
			else
			{
				for (int j = 0; j < (int)_bullets.size(); j++)
				{
					if (_bullets[j]->IsAlive())
					{
						if (_asteroids[i]->CollideWith(_bullets[j], 0.05f * _asteroids[i]->GetScale()))
						{
							_score++;

							// Split asteroid and kill bullet
							if (_asteroids[i]->GetScale() < 1)
							{
								// Kill it
								delete _asteroids[i];
								_asteroids.erase(_asteroids.begin() + i);
								i--;
								_bullets[j]->SetAlive(false);
								break;
							}
							else
							{
								// Split it
								Asteroid* a = new Asteroid();
								a->SetMesh(&_meshAsteroid);
								a->Reset(_asteroids[i]->GetScale() / 2, _asteroids[i]->GetPosition());
								_asteroids.push_back(a);

								_asteroids[i]->Reset(_asteroids[i]->GetScale() / 2, _asteroids[i]->GetPosition());
								_bullets[j]->SetAlive(false);
							}
						}
					}
				}
			}
		}

		// Add more asteroids if we have less than 5
		while (_asteroids.size() < 5)
		{
			Asteroid* a = new Asteroid();
			a->SetMesh(&_meshAsteroid);
			a->Reset();
			_asteroids.push_back(a);
		}

		// Update explosion
		if (_explosion->IsAlive())
		{
			_explosion->Update(_deltaTime);
		}

		_fader += _faderInc * _deltaTime;
		if (_fader < 0)
		{
			_faderInc = -_faderInc;
		}
		else if (_fader > 1)
		{
			_faderInc = -_faderInc;
		}

		// Change game state if necessary
		if (_gameState == Dead &&
			!_explosion->IsAlive())
		{
			_gameState = Resetting;
			_scoreCountdown = _score;
		}
		else if (_gameState == Resetting)
		{
			_scoreCountdown -= 60 * _deltaTime;
			_score = (int)_scoreCountdown;
			if (_score < 0)
			{
				Reset();
			}
		}
	}

	// Draw
	Render();

}

/******************************************************************************************************************/
