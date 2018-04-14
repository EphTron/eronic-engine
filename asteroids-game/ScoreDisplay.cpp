#include "ScoreDisplay.h"
#include "Components.h"
#include "Renderer.h"
#include "Message.h"
#include "Game.h"

/******************************************************************************************************************/

ScoreDisplay::ScoreDisplay()
	: GameObject("scoreDisplay")
{
	Colour yellow = Colour::Yellow();
	Vector4 startPos(-0.95f, 0.90f, 0, 1);
	for (int i = 0; i < NUM_DIGITS; i++)
	{
		_digits[i] = new DigitDisplay();
		_digits[i]->SetPosition(startPos);
		_digits[i]->SetAlive(false);

		MultiRenderComponent* mrc = (MultiRenderComponent*)_digits[i]->GetComponent("render");
		mrc->SetColour(yellow);

		startPos.x(startPos.x() + 0.1f);
		Game::TheGame->AddGameObject(_digits[i]);
	}
}

/******************************************************************************************************************/

ScoreDisplay::~ScoreDisplay()
{
}

/******************************************************************************************************************/

void ScoreDisplay::Update(double deltaTime)
{
	if (InCountdownMode())
	{
		_scoreCountdown -= (float)(30 * deltaTime);
		_score = (int)_scoreCountdown;
		
		if (_scoreCountdown < 0)
		{ 
			SetCountdownMode(false);
			_scoreCountdown = 0;
			_score = 0;

			Message msg("scoreCountdownFinished");
			Game::TheGame->ListenToMessage(&msg);
		}
	}

	// Based on current score, set the various digits
	int tempScore = _score;
	int numDigits = 1;
	while (tempScore >= 10)
	{
		tempScore /= 10;
		numDigits++;
	}

	for (int i = 0; i < NUM_DIGITS; i++)
	{
		if (i < numDigits)
		{
			_digits[i]->SetAlive(true); // Turn on digit
		}
		else
		{
			_digits[i]->SetAlive(false); // Turn off digit
		}
	}

	tempScore = _score;
	int number = 0;
	while (tempScore >= 10)
	{
		numDigits--;
		number = tempScore % 10;
		_digits[numDigits]->SetCurrentDigit(number);
		tempScore /= 10;
	}

	_digits[0]->SetCurrentDigit(tempScore);
}

/******************************************************************************************************************/

void ScoreDisplay::OnMessage(Message* msg)
{
	if (msg->GetMessageType() == "scoreUp")
	{
		_score++;

		// Cap score
		int maxScore = (int)pow(10, NUM_DIGITS);
		if (_score >= maxScore)
		{
			_score = maxScore - 1;
		}
	}
	else if (msg->GetMessageType() == "scoreDown")
	{
		_score--;

		// Cap score
		if (_score < 0)
		{
			_score = 0;
		}
	}
}

/******************************************************************************************************************/

void ScoreDisplay::Reset()
{
	for (int i = 0; i < NUM_DIGITS; i++)
	{
		_digits[i]->SetAlive(false);
	}

	_score = 0;
}

/******************************************************************************************************************/

void ScoreDisplay::SetPosition(Vector4 v)
{
	GameObject::SetPosition(v);
	Vector4 pos(v);
	for (int i = 0; i < NUM_DIGITS; i++)
	{
		_digits[i]->SetPosition(pos);
		pos.x(pos.x() + 0.1f);
	}
}

/******************************************************************************************************************/
