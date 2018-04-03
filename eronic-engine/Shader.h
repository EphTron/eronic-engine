#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Represents an abstract shader
class Shader
{
	// Data
private:

	// Structors
public:
	Shader(void);
	virtual ~Shader(void);
private: // Disable copying
	Shader(const Shader&);
	Shader operator=(const Shader&);

	// Accessors
public:

	// Mutators
public:

	// Functions
public:
};

