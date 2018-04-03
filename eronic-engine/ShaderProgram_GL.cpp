#if BUILD_OPENGL
#include "ShaderProgram_GL.h"
#include "ShaderObject_GL.h"
using namespace std;

/*****************************************************************************/

ShaderProgram_GL::ShaderProgram_GL(void)
{
	glex::Load();

	id = glCreateProgram();
	assert(id);
	if (id == 0)
	{
		cerr << "Error creating shader program" << endl;
		throw exception("Error creating shader program");
	}
}

/*****************************************************************************/

ShaderProgram_GL::~ShaderProgram_GL(void)
{
	// Detach all shader objects
	for (int i = 0; i < (int)attachedShaders.size(); i++)
	{
		ShaderObject_GL* obj = attachedShaders[i];
		glDetachShader(id, obj->getID());

		// Decrement reference counter (will delete shader object if == 0)
		obj->decRef();
	}
	attachedShaders.clear();

	// Delete this shader program
	glDeleteProgram(id);
}

/*****************************************************************************/

GLint ShaderProgram_GL::getUniformLocationFromShader(std::string name)
{
	return glGetUniformLocation(id, name.c_str());
}

/*****************************************************************************/

GLint ShaderProgram_GL::getUniformLocation(std::string name)
{
	UniformIterator uid = uniforms.find(name);
	if (uid != uniforms.end())
	{
		return uid->second;
	}
	else
	{
		// Query shader for it and cache the result
		GLint id = getUniformLocationFromShader(name);
		if (id >= 0)
		{
			uniforms[name] = id;
		}
		return id;
	}
}

/*****************************************************************************/

GLint ShaderProgram_GL::getAttribLocationFromShader(std::string name)
{
	return glGetAttribLocation(id, name.c_str());
}

/*****************************************************************************/
// Uniform setters
/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLint val)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform1i(id, val);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLint val1, GLint val2)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform2i(id, val1, val2);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLint val1, GLint val2, GLint val3)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform3i(id, val1, val2, val3);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLint val1, GLint val2, GLint val3, GLint val4)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform4i(id, val1, val2, val3, val4);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLfloat val)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform1f(id, val);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLfloat val1, GLfloat val2)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform2f(id, val1, val2);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLfloat val1, GLfloat val2, GLfloat val3)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform3f(id, val1, val2, val3);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		glUniform4f(id, val1, val2, val3, val4);
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLint vectorSize, GLint count, const GLint* val)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		switch (vectorSize)
		{
			case 1: glUniform1iv(id, count, val); break;
			case 2: glUniform2iv(id, count, val); break;
			case 3: glUniform3iv(id, count, val); break;
			case 4: glUniform4iv(id, count, val); break;
		}
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniform(std::string name, GLint vectorSize, GLint count, const GLfloat* val)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		switch (vectorSize)
		{
			case 1: glUniform1fv(id, count, val); break;
			case 2: glUniform2fv(id, count, val); break;
			case 3: glUniform3fv(id, count, val); break;
			case 4: glUniform4fv(id, count, val); break;
		}
	}
}

/*****************************************************************************/

void ShaderProgram_GL::setUniformMatrix(std::string name, GLint matrixSize, GLint numMatrices, bool transpose, const GLfloat* val)
{
	GLint id = getUniformLocation(name);

	if (id >= 0)
	{
		switch (matrixSize)
		{
			// Second parameter is number of matrices being passed - we assume only 1
			case 2: glUniformMatrix2fv(id, numMatrices, transpose, val); break;
			case 3: glUniformMatrix3fv(id, numMatrices, transpose, val); break;
			case 4: glUniformMatrix4fv(id, numMatrices, transpose, val); break;
		}
	}
}

/*****************************************************************************/
// Functions
/*****************************************************************************/

void ShaderProgram_GL::attachShader(ShaderObject_GL* shader)
{
	// Attach shader (and remember the attached shader)
	attachedShaders.push_back(shader);

	glAttachShader(id, shader->getID());

	// Increment reference counter
	shader->incRef();
}

/*****************************************************************************/

void ShaderProgram_GL::link()
{
	// Attempt to link the program
	glLinkProgram(id);

	// Check result:
	GLint result = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		cerr << "Error linking shader program" << endl;

		char error[1024];
		GLint errorLength=0;
		glGetProgramInfoLog(id, 1024, &errorLength, error);
		cerr << error << endl;

		// Output error to log file
		ofstream errorLog("errors.log");
		errorLog << error << endl;
		errorLog.close();

		throw exception("Error linking shader program");
	}
	else
	{
		cout << "Shader link OK" << endl;
	}
}

/*****************************************************************************/

void ShaderProgram_GL::use()
{
	glUseProgram(id);
}

/*****************************************************************************/

#endif