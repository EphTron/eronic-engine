#pragma once
#include <math.h>

/////////////////////////////////
//
// Useful maths functions and constants
//
/////////////////////////////////

// CONSTANTS
static const float PI		= 3.1415926535898f;
static const float EPSILON	= 1.0E-08f;			// error value
static const float TORAD	= 0.01745329254f;   // = PI / 180
static const float COS30	= 0.8660254038f;	// cos(30) aka sin(60)
static const float COS45	= 0.7071067812f;	// cos(45) aka sin(45)
static const float SIN60	= 0.8660254038f;	// cos(30) aka sin(60)
static const float SIN45	= 0.7071067812f;	// cos(45) aka sin(45)

// TRIGONOMETRY - sin/cos etc take degrees and floats
inline double toRad(float angle)												{ return angle * TORAD; }
inline double toDeg(float angle)												{ return angle / TORAD; }
inline float sinDeg(float angle)												{ return (float)sin(toRad(angle)); }
inline float cosDeg(float angle)												{ return (float)cos(toRad(angle)); }
inline float tanDeg(float angle)												{ return (float)tan(toRad(angle)); }
inline float sinDeg(int angle)													{ return (float)sin(toRad((float)angle)); }
inline float cosDeg(int angle)													{ return (float)cos(toRad((float)angle)); }
inline float tanDeg(int angle)													{ return (float)tan(toRad((float)angle)); }

// PYTHAGORAS
inline float dist2(float x, float y)											{ return (float)(x*x)+(y*y); }
inline float dist(float x, float y)												{ return (float)sqrt(dist2(x,y)); }
inline float dist2(float x, float y, float z)									{ return (float)(x*x)+(y*y)+(z*z); }
inline float dist(float x, float y, float z)									{ return (float)sqrt(dist2(x,y,z)); }
inline float dist2(float x1, float y1, float x2, float y2)						{ float dx = x2-x1; float dy = y2-y1; return dist2(dx, dy); }
inline float dist(float x1, float y1, float x2, float y2)						{ float dx = x2-x1; float dy = y2-y1; return dist(dx, dy); }
inline float dist2(float x1, float y1, float z1, float x2, float y2, float z2)	{ float dx = x2-x1; float dy = y2-y1; float dz = z2-z1; return dist2(dx, dy, dz); }
inline float dist(float x1, float y1, float z1, float x2, float y2, float z2)	{ float dx = x2-x1; float dy = y2-y1; float dz = z2-z1; return dist(dx, dy, dz); }

// OTHER
inline float absf(float x)														{ return (float)fabs(x); }
inline float maxf(float a, float b)												{ if (a > b) return a; return b; }
inline float minf(float a, float b)												{ if (a < b) return a; return b; }
inline float mid(float a, float b)												{ return (a + b) / 2; }

// INTERPOLATE
inline float lerp(float a, float b, float f)									{ return ((1-f)*a) + (f*b); }

// GEOGRAPHIC COORDINATES
inline float xpos(float lat, float longt, float alt)							{ return alt * cosf(longt) * sinf(lat); }
inline float ypos(float lat)													{ return cosf(lat); }
inline float zpos(float lat, float longt, float alt)							{ return alt * sinf(longt) * sinf(lat); }
