#pragma once
#include "MathsHelper.h"
#include <iostream>

/////////////////////////////////
//
// Four-float Vector class
//
/////////////////////////////////

class Vector4
{
	////////////////////////////////
	// Constants
private:
	static const int _X = 0;
	static const int _Y = 1;
	static const int _Z = 2;
	static const int _W = 3;

	static const int _S = 0;
	static const int _T = 1;
	static const int _P = 2;
	static const int _Q = 3;	

	static const int _R = 0;
	static const int _G = 1;
	static const int _B = 2;
	static const int _A = 3;

public:
	static const int MAX_ELEMENTS = 4;

	////////////////////////////////
	// Class Members
private:
	float _elements[MAX_ELEMENTS];

	////////////////////////////////
	// Structors
public:
	// Default constructor initialises to (0,0,0,1)
	Vector4();

	// Destructor
	virtual ~Vector4();

	// Create vector from individual floats, with defaults for z and w
	Vector4(float x, float y, float z=0, float w=1);

	// Create vector from an array of floats (either 1x2, 1x3, or 1x4)
	explicit Vector4(float values[]);

	// Copy constructor
	Vector4(const Vector4& toCopy);

	// Assignment operator
	Vector4& operator=(const Vector4& toCopy);


	////////////////////////////////
	// Accessors
public:
	// For position/direction vectors
	float x() const					{ return _elements[_X]; }
	float y() const					{ return _elements[_Y]; }
	float z() const					{ return _elements[_Z]; }
	float w() const					{ return _elements[_W]; }

	// For texture coordinates
	float s() const					{ return _elements[_S]; }
	float t() const					{ return _elements[_T]; }
	float p() const					{ return _elements[_P]; } // AKA r but renamed p to avoid clash with colour R
	float q() const					{ return _elements[_Q]; }

	// For colours
	float r() const					{ return _elements[_R]; }
	float g() const					{ return _elements[_G]; }
	float b() const					{ return _elements[_B]; }
	float a() const					{ return _elements[_A]; }

	// Indexed get
	float get(int i) const			{ return _elements[i]; }

	// Get whole array
	const float* xyzw() const		{ return _elements; }


	////////////////////////////////
	// Mutators
public:
	void x(float val)				{ _elements[_X] = val; }
	void y(float val)				{ _elements[_Y] = val; }
	void z(float val)				{ _elements[_Z] = val; }
	void w(float val)				{ _elements[_W] = val; }

	void s(float val)				{ _elements[_S] = val; }
	void t(float val)				{ _elements[_T] = val; }
	void p(float val)				{ _elements[_P] = val; }
	void q(float val)				{ _elements[_Q] = val; }

	void r(float val)				{ _elements[_R] = val; }
	void g(float val)				{ _elements[_G] = val; }
	void b(float val)				{ _elements[_B] = val; }
	void a(float val)				{ _elements[_A] = val; }

	// Assignment
	void set(float x, float y, float z = 0, float w = 1);

	// Assignment with array (accepts 1x2, 1x3, 1x4 array)
	void set(float values[]);

	// Assignment from another Vector4
	void set(const Vector4& other);

	// Indexed assignment
	void set(int index, float val)	{ _elements[index] = val; }


	////////////////////////////////
	// Functions
public:

	// Copies content of vector to another array
	void copyToArray(float targetArray[], int offset=0) const;

	// MATHEMATICAL FUNCTIONS

	// Adds another vector to this one
	void add(const Vector4& other);
	
	// Adds a scalar to each element
	void add(float val);
	
	// Subtracts another vector from this one
	void subtract(const Vector4& other);
	
	// Subtracts a scalar from each element
	void subtract(float val);

	// Multiples this vector by another one
	void multiply(const Vector4& other);
	
	// Multiples this vector a scalar
	void multiply(float val);
	
	// Divides this vector by another one
	void divide(const Vector4& other);

	// Divides this vector by a scalar
	void divide(float val);

	// Returns the magnitude (length) of the vector
	float length() const;

	// Returns the magnitude (length) squared of the vector
	float lengthSquared() const;

	// Limit this vector to a maximum size
	void limitTo(float val);

	// Returns the dot product of this vector and another
	float dot(const Vector4& other) const;

	// Returns the dot product of this vector and another
	float dot4(const Vector4& other) const;

	// Performs a cross product of two other vectors and stores the result in this vector
	void cross(const Vector4& a, const Vector4& b);

	// Rotates this vector clockwise around the origin (only for 2D)
	void rotate(float degrees);

	// Normalise this vector (make length = 1)
	void normalise();

	// Bearing (angle in degrees) to a given position, relative to the current vector position
	float bearingTo(const Vector4& position);
	
	////////////////////////////////
	// Operators
public:
	void operator+=(const Vector4& other)	{ add(other); }
	void operator+=(float val)				{ add(val); }
	void operator-=(const Vector4& other)	{ subtract(other); }
	void operator-=(float val)				{ subtract(val); }
	void operator*=(const Vector4& other)	{ multiply(other); }
	void operator*=(float val)				{ multiply(val); }
	void operator/=(const Vector4& other)	{ divide(other); }
	void operator/=(float val)				{ divide(val); }

	// Indexed access
	float operator[](int index) const 		{ return _elements[index]; }
	//float& operator[](int index) 			{ return const_cast<float&>(static_cast<const Vector4&>(*this)[ index ] ); }

	
};

////////////////////////////////////
// Auxiliary Operators

// Console output
std::ostream& operator << (std::ostream& o, const Vector4& m);

// Unary negation
inline Vector4 operator-(const Vector4& a) 
{ 
	Vector4 result(a); 
	result *= -1; 
	return result; 
}

////////////////////////////////////
// Inlines

inline void Vector4::set(float x, float y, float z, float w)
{
	_elements[_X] = x;
	_elements[_Y] = y;
	_elements[_Z] = z;
	_elements[_W] = w;
}

inline void Vector4::set(const Vector4& other)
{
	_elements[_X] = other.x();
	_elements[_Y] = other.y();
	_elements[_Z] = other.z();
	_elements[_W] = other.w();
}

inline void Vector4::add(const Vector4& other)
{
	_elements[_X] += other.x();
	_elements[_Y] += other.y();
	_elements[_Z] += other.z();
	_elements[_W] += other.w();
}

inline void Vector4::add(float val)
{
	_elements[_X] += val;
	_elements[_Y] += val;
	_elements[_Z] += val;
	_elements[_W] += val;
}

inline void Vector4::subtract(const Vector4& other)
{
	_elements[_X] -= other.x();
	_elements[_Y] -= other.y();
	_elements[_Z] -= other.z();
	_elements[_W] -= other.w();
}

inline void Vector4::subtract(float val)
{
	_elements[_X] -= val;
	_elements[_Y] -= val;
	_elements[_Z] -= val;
	_elements[_W] -= val;
}

inline void Vector4::multiply(const Vector4& other)
{
	_elements[_X] *= other.x();
	_elements[_Y] *= other.y();
	_elements[_Z] *= other.z();
	_elements[_W] *= other.w();
}

inline void Vector4::multiply(float val)
{
	_elements[_X] *= val;
	_elements[_Y] *= val;
	_elements[_Z] *= val;
	_elements[_W] *= val;
}

inline void Vector4::divide(const Vector4& other)
{
	_elements[_X] /= other.x();
	_elements[_Y] /= other.y();
	_elements[_Z] /= other.z();
	_elements[_W] /= other.w();
}

inline void Vector4::divide(float val)
{
	_elements[_X] /= val;
	_elements[_Y] /= val;
	_elements[_Z] /= val;
	_elements[_W] /= val;
}

inline float Vector4::length() const
{
	return dist(_elements[_X], _elements[_Y], _elements[_Z]);
}

inline float Vector4::lengthSquared() const
{
	return dist2(_elements[_X], _elements[_Y], _elements[_Z]);
}

inline void Vector4::limitTo(float val)
{
	if (lengthSquared() > val * val)
	{
		normalise();
		multiply(val);
	}
}

inline float Vector4::dot(const Vector4& other) const
{
	return this->x() * other.x() +
		   this->y() * other.y() +
		   this->z() * other.z();
}

inline float Vector4::dot4(const Vector4& other) const
{
	return this->x() * other.x() +
		   this->y() * other.y() +
		   this->z() * other.z() +
		   this->w() * other.w();
}

inline void Vector4::cross(const Vector4& a, const Vector4& b)
{
	float nx = a.y() * b.z() - a.z() * b.y();
	float ny = a.z() * b.x() - a.x() * b.z();
	float nz = a.x() * b.y() - a.y() * b.x();

	set(nx, ny, nz);
}

inline void Vector4::rotate(float degrees)
{
	float nx = cosDeg(degrees) * _elements[_X] + sinDeg(degrees) * _elements[_Y];
	float ny = cosDeg(degrees) * _elements[_Y] - sinDeg(degrees) * _elements[_X];
	_elements[_X] = nx;
	_elements[_Y] = ny;
}

inline void Vector4::normalise()
{
	float l = length();
	if (l != 0) divide(l);
}

// NB: Assumes +Y is up
inline float Vector4::bearingTo(const Vector4& position)
{
	float ydiff = position.y() - y();
	float xdiff = position.x() - x();

	if (xdiff == 0 && ydiff > 0)
	{
		return 0;
	}
	else if (xdiff == 0 && ydiff < 0)
	{
		return 180;
	}

	float a = (float)toDeg(atan2f(ydiff, xdiff));

	if (ydiff > 0 && xdiff > 0) // 0-90 deg
	{
		a = 90 - a;
	}
	else if (ydiff <= 0 && xdiff > 0) //  90-180 deg
	{
		a = 90 - a;
	}
	else if (ydiff <= 0 && xdiff < 0) // 180-270 deg
	{
		a = 90 + -a;
	}
	else if (ydiff > 0 && xdiff < 0) // 270-360 deg
	{
		a = 450 - a;
	}

	return a;
}
