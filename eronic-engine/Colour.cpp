#include "Colour.h"
#include <exception>
using namespace std;

/*******************************************************************/
// Structors
/*******************************************************************/

Colour::Colour()
{
	set(1,1,1,1);
}

/*******************************************************************/

Colour::Colour(float r, float g, float b, float a)
{
	set(r,g,b,a);
}

/*******************************************************************/

// This constructor accepts float arrays of 2, 3, or 4 floats
Colour::Colour(float values[])
{
	set(values);
}

/*******************************************************************/

Colour::~Colour()
{
}

/*******************************************************************/

// Copy constructor
Colour::Colour(const Vector4& toCopy)
{
	set(toCopy);
}

/*******************************************************************/

// Assignment operator
Colour& Colour::operator=(const Colour& toCopy)
{
	if (this == &toCopy) return *this;
	set(toCopy);

	return *this;
}

/*******************************************************************/
// Functions
/*******************************************************************/

Colour Colour::Red()
{
	return Colour(1,0,0);
}

Colour Colour::Blue()
{
	return Colour(0,0,1);
}

Colour Colour::Green()
{
	return Colour(0,1,0);
}

Colour Colour::Yellow()
{
	return Colour(1,1,0);
}

Colour Colour::Black()
{
	return Colour(0,0,0);
}

Colour Colour::White()
{
	return Colour(1,1,1);
}

Colour Colour::Grey()
{
	return Colour(0.5,0.5,0.5);
}

Colour Colour::Null()
{
	return Colour(0,0,0,0);
}
