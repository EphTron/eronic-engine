#pragma once
#if BUILD_OPENGL
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "GL\GXBase\GXBase.h"
using namespace gxbase;

/* Simple class to represent an OpenGL Shader Object
 * e.g. a fragment shader, or a vertex shader
 */
class ShaderObject_GL 
	: public glex  // Gain access to OpenGL functions
{
	// Data
private:
	GLuint		id;				// Object ID
	int			refCounter;		// How many programs we are attached to
	std::string shaderSrc;		// Source file in string form
	GLenum		shaderType;		// The shader type (e.g. GL_VERTEX_SHADER)

	// Structors
public:
	ShaderObject_GL(std::string filename, GLenum shaderType);
	virtual ~ShaderObject_GL(void);
private:
	ShaderObject_GL(const ShaderObject_GL&); // Prevent copying
	ShaderObject_GL operator=(const ShaderObject_GL&); 

	// Accessors
public:
	GLuint getID() const			{ return id; }
	GLenum getShaderType() const	{ return shaderType; }

	// Ref counter
public:
	void incRef()					{ refCounter++; }
	void decRef()					{ refCounter--; if (refCounter == 0) delete this; }
};

#endif