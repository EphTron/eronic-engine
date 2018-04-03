#pragma once
#include "Vector4.h"

/////////////////////////////////
//
// RGBA colour class
//
/////////////////////////////////

class Colour 
	: public Vector4
{

	////////////////////////////////
	// Structors
public:
	// Default constructor initialises to (0,0,0,1)
	Colour();

	// Destructor
	virtual ~Colour();

	// Create vector from individual floats, with defaults for a
	Colour(float r, float g, float b, float a=1);

	// Create vector from an array of floats (either 1x3, or 1x4)
	explicit Colour(float values[]);

	// Copy constructor
	explicit Colour(const Vector4& toCopy);

	// Assignment operator
	Colour& operator=(const Colour& toCopy);

	////////////////////////////////
	// Functions
public:
	const float* rgba() const		{ return xyzw(); }

	////////////////////////////////
	// Static Functions
public:
	static Colour Red();
	static Colour Blue();
	static Colour Green();
	static Colour Yellow();
	static Colour Black();
	static Colour White();
	static Colour Grey();
	static Colour Null();
};