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


class DigitDisplay
	: public GameObject
{
	// Data
private:
	int		_number;

	// Structors
public:
	DigitDisplay();
	~DigitDisplay();

	// Functions
public:
	int GetCurrentDigit()		const	{ return _number; }
	bool SetCurrentDigit(int i)			{ if (i >= 0 && i < 10) { _number = i; return true; } return false; }

	// Functions
public:
	virtual void Update(double deltaTime);
};

