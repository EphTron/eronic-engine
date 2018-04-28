#pragma once
#include <vector>
#include "Scene.h"
#include "WordDisplay.h"
#include "PhysicsSystem.h"
#include "Network.h"

typedef struct MenuElement {
	MenuElement(Vector4 pos, float scl, std::string txt):
		selectable(true),
		selected(false),
		text_display(new WordDisplay())
	{
		text_display->SetPosition(pos);
		text_display->ChangeScale(scl);
		text_display->SetColour(Colour::Yellow());
		text_display->SetWord(txt);
	}

	bool selectable;
	bool selected;
	WordDisplay* text_display;
}MenuElement;

class MainMenuScene :
	public Scene
{
	// Constants
public:

	// Data
protected:
	PhysicsSystem						_physicsSystem;
	WordDisplay*						_title;
	std::vector<MenuElement*>			_menu;
	eronic::Network*					_network;

	int									_menu_state;
	int									_menu_selection;
	float								_menu_scale;

	// Structors
public:
	MainMenuScene();
	virtual ~MainMenuScene();


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

