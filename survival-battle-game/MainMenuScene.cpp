#include "MainMenuScene.h"
#include "SceneManager.h"
#include "Game.h"
#include "RenderSystem.h"
#include "GamePlayScene.h"
#include "GameOverScene.h"
#include "Asteroid.h"
#include <cmath>

/******************************************************************************************************************/
// Structors
/******************************************************************************************************************/

MainMenuScene::MainMenuScene()
	: _title(NULL),
	_network(nullptr),
	_menu(std::vector<MenuElement*>()),
	_menu_state(Idle),
	_menu_selection(0) //,
	//_networkManager(nullptr)
{
	std::cout << " createed" << std::endl;
}

/******************************************************************************************************************/

MainMenuScene::~MainMenuScene()
{
	delete _title;
	for (auto w : _menu) {
		delete w->text_display;
		delete w;
	}
}

/******************************************************************************************************************/
// Functions
/******************************************************************************************************************/

void MainMenuScene::Initialise()
{
	_title = new WordDisplay();
	_title->SetPosition(Vector4(-0.8f, 0.6f, 1.0));
	_title->SetColour(Colour::Green());
	_title->SetWord("BATTLE OF SURVIVAL");

	// beverley jennifer

	// calls at 4o clock on wednesday

	_menu_scale = 0.5f;

	MenuElement* play = new MenuElement(Vector4(-0.65f, 0.2f, 0), _menu_scale, "CREATE AND PLAY");
	MenuElement* find = new MenuElement(Vector4(-0.65f, 0.0f, 0), _menu_scale, "FIND NETWORK");
	MenuElement* connect = new MenuElement(Vector4(-0.65f, -0.2f, 0), _menu_scale, "JOIN AND PLAY");
	connect->text_display->SetColour(Colour::Grey());
	connect->selectable = false;
	MenuElement* exit = new MenuElement(Vector4(-0.65f, -0.4f, 0), _menu_scale, "EXIT");

	_menu.push_back(play);
	_menu.push_back(find);
	_menu.push_back(connect);
	_menu.push_back(exit);

	_menu[_menu_selection]->text_display->SetColour(Colour::Red());
	_menu[_menu_selection]->text_display->ChangeScale(_menu_scale + _menu_scale * 0.5);
	_menu[_menu_selection]->selected = true;

	// Create asteroids
	for (int i = 0; i < 50; i++)
	{
		Asteroid* asteroid = new Asteroid(_sceneManager->GetGame()->GetMesh("asteroid"));
		asteroid->Reset();
		asteroid->SetScale(2.0f * (rand() * 1.0f / RAND_MAX) + 0.5f);
		_gameObjects.push_back(asteroid);
	}

	// Start all objects to set them up
	for (int i = 0; i < (int)_gameObjects.size(); i++)
	{
		_gameObjects[i]->Start();
	}

	_networkManager = _sceneManager->GetGame()->GetNetworkManager();
}

/******************************************************************************************************************/

void MainMenuScene::OnKeyboard(int key, bool down)
{
	if (down) return; // Ignore key down events
	OutputDebugStringA("KEYBOARD");
	// Switch key presses
	switch (key)
	{
	case 13: // Enter or space
		switch (_menu_selection) {
			case 0: // CREATE AND PLAY
				OutputDebugStringA("Open network");
				//_sceneManager->GetGame()->GetNetworkManager()->open_network(1, 9175);
				//_sceneManager->GetGame()->RunNetwork(true);
				//_sceneManager->GetGame()->GetNetworkManager()->open_network(1,9175);
				//_sceneManager->GetGame()->GetNetworkManager()->run_peer_network();
				_sceneManager->PushScene(new GamePlayScene()); // Play game
				break;
			case 1: // FIND NETWORK
				OutputDebugStringA("Search for network");
				_search_promise = std::promise<int>();
				_search_future = _search_promise.get_future();
				_search_thread = std::thread(&eronic::P2PNetworkManager::find_future_networks,
					std::ref(*_networkManager),
					std::move(_search_promise), 5000, false);
				//_sceneManager->GetGame()->GetNetworkManager()->find_networks(5000, false);
				//_menu[2]->text_display->SetColour(Colour::Yellow());
				//_menu[2]->selectable = true;
				_menu_state = Searching;
				break;
			case 2: // JOIN AND PLAY
				_menu_state = Connecting;
				OutputDebugStringA("Join network");
				_sceneManager->GetGame()->GetNetworkManager()->join_network(_network->network_id,_network->network_port);
				_sceneManager->GetGame()->RunNetwork(true);
				_sceneManager->PushScene(new GamePlayScene()); // Play game
				break;
			case 3: // EXIT
				OutputDebugStringA("Exit game");
				_sceneManager->PopScene(); // Leave menu
				break;
		}
		break;
	case 27: // Escape
		_sceneManager->PopScene(); // Leave menu
		break;
	case 32: // Space
		_sceneManager->PopScene(); // Leave menu
		break;
	case 40: // Down
		_menu[_menu_selection]->text_display->SetColour(Colour::Yellow());
		_menu[_menu_selection]->text_display->ChangeScale(_menu_scale);
		_menu[_menu_selection]->selected = false;
		do {
			if (_menu_selection < _menu.size() - 1) {
				_menu_selection++;
			}
			else {
				_menu_selection = 0;
			}
		} while (_menu[_menu_selection]->selectable == false);

		_menu[_menu_selection]->text_display->SetColour(Colour::Red());
		_menu[_menu_selection]->text_display->ChangeScale(_menu_scale + _menu_scale * 0.5);
		_menu[_menu_selection]->selected = true;
		break;
	case 38: // Up
		_menu[_menu_selection]->text_display->SetColour(Colour::Yellow());
		_menu[_menu_selection]->text_display->ChangeScale(_menu_scale);
		_menu[_menu_selection]->selected = false;
		do {
			if (_menu_selection > 0) {
				_menu_selection--;
			}
			else {
				_menu_selection = _menu.size() - 1;
			}
		} while (_menu[_menu_selection]->selectable == false);
		_menu[_menu_selection]->text_display->SetColour(Colour::Red());
		_menu[_menu_selection]->text_display->ChangeScale(_menu_scale + _menu_scale * 0.5);
		_menu[_menu_selection]->selected = true;
		break;
	default: // Any other key
		//_sceneManager->PushScene(new GamePlayScene()); // Play game
		break;
	}
}

/******************************************************************************************************************/

/// Update current scene
void MainMenuScene::Update(double deltaTime)
{
	Scene::Update(deltaTime);
	_physicsSystem.Process(_gameObjects, deltaTime);
	
	if (_menu_state == Searching) {
		std::future_status status = _search_future.wait_for(std::chrono::seconds(0));
		if (status == std::future_status::ready) {
			if (_search_future.get() == true) {
				int found_networks = _sceneManager->GetGame()->GetNetworkManager()->get_networks().size();
				_menu[1]->text_display->SetWord("FOUND NETWORK");
				_menu[2]->text_display->SetColour(Colour::Yellow());
				_menu[2]->selectable = true;
				_menu_state = Idle;
				std::map<int, eronic::Network*> networks;
				networks = _sceneManager->GetGame()->GetNetworkManager()->get_networks();
				_network = networks.begin()->second;
				_search_thread.join();
				OutputDebugStringA("True");
			}
			else {
				_menu_state = Idle;
				OutputDebugStringA("False");
				_search_thread.join();
			}
		}
		else {
			//OutputDebugStringA("Not Ready");
		}
	}

}

/******************************************************************************************************************/

/// Render current scene
void MainMenuScene::Render(RenderSystem* renderer)
{
	glm::mat4 MVM;

	renderer->Process(_gameObjects, 0);

	_title->Render(renderer->GetRenderer(), MVM);
	for (auto w : _menu) {
		w->text_display->Render(renderer->GetRenderer(), MVM);
	}
}

/******************************************************************************************************************/
