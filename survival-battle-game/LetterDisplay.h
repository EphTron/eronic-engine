#pragma once
#include "GameObject.h"

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

// Engine classes
#include "Vector4.h"
#include "Colour.h"
#include "MathsHelper.h"


class LetterDisplay
	: public GameObject
{
	// Data
private:
	char		_letter;

	// Structors
public:
	LetterDisplay();
	~LetterDisplay();

	// Functions
public:
	int GetCurrentLetter()			const	{ return _letter; }
	bool SetCurrentLetter(char c)			{ if (c >= 'A' && c <= 'Z') { _letter = c; return true; } return false; }

	// Functions
public:
	virtual void Update(double deltaTime);
};

