#pragma once

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

// Engine classes
#include "Mesh.h"
#include "Vector4.h"
#include "Colour.h"
#include "MathsHelper.h"


class ScoreDisplay
{
	// Data
private:
	Mesh _numbers[10];

	// Structors
public:
	ScoreDisplay(Renderer* renderer);
	~ScoreDisplay();

	// Functions
public:
	void Draw(Renderer* renderer, int score, glm::mat4 MVM);
};

