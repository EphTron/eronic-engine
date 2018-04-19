#include "LetterDisplay.h"
#include "Components.h"
#include "Game.h"
#include <sstream>

/******************************************************************************************************************/

LetterDisplay::LetterDisplay()
	: GameObject("letterDisplay")
{
	// Create components (they will add themselves)
	MultiRenderComponent* rc = new MultiRenderComponent(this);
	rc->SetColour(Colour(0.5f, 0.5f, 1.0f, 1.0f));
	for (char i = 'A'; i <= 'Z'; i++)
	{
		std::stringstream num;
		num << i;
		Mesh* m = Game::TheGame->GetMesh(num.str());
		if (m)
		{
			rc->AddMesh(num.str(), m);
		}
	}
	rc->SelectCurrentMesh("A");
	rc->ShouldDraw(true);

	_letter = 'A';
}

/******************************************************************************************************************/

LetterDisplay::~LetterDisplay()
{
}

/******************************************************************************************************************/

void LetterDisplay::Update(double deltaTime)
{
	MultiRenderComponent* rc = (MultiRenderComponent*)GetComponent("render");
	std::stringstream num;
	num << _letter;
	rc->SelectCurrentMesh(num.str());
}

/******************************************************************************************************************/
