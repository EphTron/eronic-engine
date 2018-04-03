#include "Vector4.h"
#include <exception>
using namespace std;

/*******************************************************************/
// Structors
/*******************************************************************/

Vector4::Vector4()
{
	set(0,0,0,1);
}

/*******************************************************************/

Vector4::Vector4(float x, float y, float z, float w)
{
	set(x,y,z,w);
}

/*******************************************************************/

// This constructor accepts float arrays of 2, 3, or 4 floats
Vector4::Vector4(float values[])
{
	set(values);
}

/*******************************************************************/

Vector4::~Vector4()
{
}

/*******************************************************************/

// Copy constructor
Vector4::Vector4(const Vector4& toCopy)
{
	set(toCopy);
}

/*******************************************************************/

// Assignment operator
Vector4& Vector4::operator=(const Vector4& toCopy)
{
	if (this == &toCopy) return *this;
	set(toCopy);

	return *this;
}

/*******************************************************************/
// Mutators
/*******************************************************************/

void Vector4::set(float values[])
{
	// Pre-initialise
	set(0,0,0,1);

	// Check array length
	int arrayLen = sizeof(values);

	if (arrayLen < 2)
	{
		throw exception("Vector initialisation must have at least 2 floats");
	}
	else if (arrayLen > 4)
	{
		throw exception("Vector initialisation must have at most 4 floats");
	}

	// Assign values
	for (int i = 0; i < arrayLen; i++)
	{
		_elements[i] = values[i];
	}
}

/*******************************************************************/
// Functions
/*******************************************************************/

void Vector4::copyToArray(float targetArray[], int offset) const
{
	// Check array is big enough first
	if (sizeof(targetArray) - offset < 4)
	{
		throw exception("Array not big enough to copy vector into");
	}

	// Copy elements
	for (int i = 0; i < MAX_ELEMENTS; i++)
	{
		targetArray[i+offset] = _elements[i];
	}
}

/*******************************************************************/
// Auxiliaries
/*******************************************************************/

ostream& operator << (ostream& o, const Vector4& v)
{
	o << "[" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << "]" << endl;
	return o;
}

/*******************************************************************/

