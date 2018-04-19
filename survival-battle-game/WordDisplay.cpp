#include "WordDisplay.h"
#include "Components.h"
#include "Renderer.h"
#include "Message.h"
#include "Game.h"

/******************************************************************************************************************/

WordDisplay::WordDisplay()
	: GameObject("wordDisplay")
{
	_colour = Colour::Yellow();
}

/******************************************************************************************************************/

WordDisplay::~WordDisplay()
{
}

/******************************************************************************************************************/

void WordDisplay::Update(double deltaTime)
{
}

/******************************************************************************************************************/

void WordDisplay::OnMessage(Message* msg)
{
}

/******************************************************************************************************************/

void WordDisplay::Reset()
{
}

/******************************************************************************************************************/

void WordDisplay::Render(Renderer* r, glm::mat4 MVM)
{
	Vector4 pos = _position;
	for (int i = 0; i < _word.length(); i++)
	{
		char c = _word[i];

		MultiRenderComponent* mrc = NULL;
		if (_letterDisplay.SetCurrentLetter(c))
		{
			_letterDisplay.Update(0);
			_letterDisplay.SetPosition(pos);
			mrc = (MultiRenderComponent*)(_letterDisplay.GetComponent("render"));
			mrc->SetColour(_colour);
		}

		pos.x(pos.x() + 0.1f);

		if (mrc)
		{
			r->Draw(mrc, MVM);
		}
	}
}

/******************************************************************************************************************/
