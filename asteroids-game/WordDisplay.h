#pragma once
#include "GameObject.h"
#include "LetterDisplay.h"

// GLM
#include "GL\GLM\GLM.hpp"
#include "GL\GLM\GTC\matrix_transform.hpp"
#include "GL\GLM\GTC\type_ptr.hpp"

#include <cmath>

class Renderer;

class WordDisplay
	: public GameObject
{
	// Constants
public:

	// Data
private:
	Colour			_colour;					// Colour to use
	std::string		_word;						// Word to display
	LetterDisplay	_letterDisplay;				// The letter display

	// Structors
public:
	WordDisplay();
	~WordDisplay();

	// Gets/sets
public:
	Colour GetColour()		const	{ return _colour; }
	void SetColour(Colour c)		{ _colour = c; }

	std::string GetWord()	const	{ return _word; }
	void SetWord(std::string w)		{ _word = w; }

	// Functions
public:
	virtual void Update(double deltaTime);
	virtual void OnMessage(Message* msg);
	virtual void Reset();

	void Render(Renderer* r, glm::mat4 MVM); // This is cheating really but meh
};

