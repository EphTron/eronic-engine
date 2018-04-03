#include "ScoreDisplay.h"
#include "Renderer.h"

/******************************************************************************************************************/

ScoreDisplay::ScoreDisplay(Renderer* renderer)
{
	// Setup letter meshes
	_numbers[0].AddVertex({ -0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[0].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[0].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[0].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[0].AddVertex({ -0.04f, -0.09f, 0,		1, 1, 1, 1 });

	_numbers[1].AddVertex({  0.0f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[1].AddVertex({  0.0f,  0.09f, 0,		1, 1, 1, 1 });

	_numbers[2].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[2].AddVertex({ -0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[2].AddVertex({ -0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[2].AddVertex({  0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[2].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[2].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	 
	_numbers[3].AddVertex({ -0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[3].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[3].AddVertex({  0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[3].AddVertex({ -0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[3].AddVertex({  0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[3].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[3].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });

	_numbers[4].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[4].AddVertex({ -0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[4].AddVertex({  0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[4].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[4].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });

	_numbers[5].AddVertex({ -0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[5].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[5].AddVertex({  0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[5].AddVertex({ -0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[5].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[5].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });

	_numbers[6].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[6].AddVertex({ -0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[6].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[6].AddVertex({  0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[6].AddVertex({ -0.04f,  0.00f, 0,		1, 1, 1, 1 });

	_numbers[7].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[7].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[7].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });

	_numbers[8].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[8].AddVertex({ -0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[8].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[8].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[8].AddVertex({  0.04f, -0.00f, 0,		1, 1, 1, 1 });
	_numbers[8].AddVertex({ -0.04f, -0.00f, 0,		1, 1, 1, 1 });
	_numbers[8].AddVertex({  0.04f, -0.00f, 0,		1, 1, 1, 1 });
	_numbers[8].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });

	_numbers[9].AddVertex({  0.04f, -0.09f, 0,		1, 1, 1, 1 });
	_numbers[9].AddVertex({  0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[9].AddVertex({ -0.04f,  0.09f, 0,		1, 1, 1, 1 });
	_numbers[9].AddVertex({ -0.04f,  0.00f, 0,		1, 1, 1, 1 });
	_numbers[9].AddVertex({  0.04f,  0.00f, 0,		1, 1, 1, 1 });

	for (int i = 0; i < 10; i++)
	{
		_numbers[i].CreateVBO(renderer);
	}
}

/******************************************************************************************************************/

ScoreDisplay::~ScoreDisplay()
{
}

/******************************************************************************************************************/

void ScoreDisplay::Draw(Renderer* renderer, int score, glm::mat4 MVM)
{
	int number = 0;
	float offset = -0.1f;
	int numberOfDigits = 0;
	int tscore = score;
	while (tscore >= 10)
	{
		tscore /= 10;
		numberOfDigits++;
	}

	MVM = glm::translate(MVM, glm::vec3(-offset * numberOfDigits, 0, 0));
	glm::mat4 tempMVM;

	Colour colour = Colour::Yellow();

	while (score >= 10)
	{
		number = score % 10;
		renderer->Draw(&_numbers[number], MVM, colour);
		score /= 10;
		MVM = glm::translate(MVM, glm::vec3(offset, 0, 0));
	}

	renderer->Draw(&_numbers[score], MVM, colour);
}

/******************************************************************************************************************/
