#pragma once
#if BUILD_OPENGL

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "GL\GXBase\GXBase.h"
using namespace gxbase;

// Forward declare the ShaderObject_GL class
class ShaderObject_GL;

typedef std::map<std::string, GLint>::iterator UniformIterator;

/* Simple class to represent an OpenGL Shader Program
 * i.e. containing one or more shader objects
 */
class ShaderProgram_GL 
	: public glex // Gain access to OpenGL functions
{
	// Data
private:
	std::vector<ShaderObject_GL*>		attachedShaders;	// The list of attached shaders
	GLuint							id;					// The program ID
	std::map<std::string, GLint>	uniforms;			// Cache uniform IDs;

	// Structors
public:
	ShaderProgram_GL(void);
	virtual ~ShaderProgram_GL(void);
private: // Disable copying
	ShaderProgram_GL(const ShaderProgram_GL&);
	ShaderProgram_GL operator=(const ShaderProgram_GL&);

	// Accessors
public:
	GLuint getID() const								{ return id; }
	GLint getUniformLocation(std::string name)			;
	GLint getUniformLocationFromShader(std::string name);
	GLint getAttribLocationFromShader(std::string name)	;

	// Mutators
public:

	// Uniform setters
	void setUniform(std::string name, GLint val);
	void setUniform(std::string name, GLint val1, GLint val2);
	void setUniform(std::string name, GLint val1, GLint val2, GLint val3);
	void setUniform(std::string name, GLint val1, GLint val2, GLint val3, GLint val4);
	void setUniform(std::string name, GLfloat val);
	void setUniform(std::string name, GLfloat val1, GLfloat val2);
	void setUniform(std::string name, GLfloat val1, GLfloat val2, GLfloat val3);
	void setUniform(std::string name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4);
	void setUniform(std::string name, GLint count, GLint vectorSize, const GLint* val);
	void setUniform(std::string name, GLint vectorSize, GLint count, const GLfloat* val);
	void setUniformMatrix(std::string name, GLint matrixSize, GLint numMatrices, bool transpose, const GLfloat* val);

	// Functions
public:
	void attachShader(ShaderObject_GL* shader);
	void link();
	void use();
};

#endif