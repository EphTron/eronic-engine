#if BUILD_OPENGL
#include "ShaderObject_GL.h"
using namespace std;

/*****************************************************************************/

ShaderObject_GL::ShaderObject_GL(string filename, GLenum shaderType)
{
	// Initialise OpenGL functions and our own variables
	bool loadedGL = glex::Load();
	refCounter = 0;
	this->shaderType = shaderType;

	// Attempt to load file
	ifstream file;
	file.open(filename, ios::in);
	if (!file)
	{
		cerr << "Error loading " << filename << " - file not found" << endl;
		throw exception("Error loading shader");
	}

	// Read in strings and store them in a stringstream
	stringstream srcBuilder;
	while (!file.eof())
	{
		unsigned char in = file.get();
		srcBuilder << in;
	}
	file.close();

	// Obtain the file in a single string
	shaderSrc = srcBuilder.str();

	// Remove EOF char
	shaderSrc = shaderSrc.substr(0, shaderSrc.size()-1);

	// Check length?
	if (shaderSrc.size() == 0)
	{
		cerr << "Error loading " << filename << " - empty file" << endl;
		throw exception("Error loading shader - empty file");
	}

	file.close();

	////////////////////////
	// Try to build shader

	// Create shader object
	id = glCreateShader(shaderType);
	if (id == 0)
	{
		cerr << "Could not create shader object" << endl;
		throw exception("Could not create shader object");
	}
	assert(id);

	// Assign source to a GLchar** (array of char[])
	// We also need a separate array of string lengths
	// We will be using a single string, so we have one entry in each array
	GLint len[1];
	len[0] = shaderSrc.size();
	GLchar** src = new GLchar*[1];
	src[0] = new GLchar[len[0]];
	for (int i = 0; i < len[0]; i++) src[0][i] = shaderSrc[i]; // Copy string into char[]
	glShaderSource(id, 1, (const GLchar**) src, len); // Set source of shader

	// Compile shader
	glCompileShader(id);

	// Check compile status
	GLint result = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result) // If there was a problem...
	{
		cerr << "Compile for " << filename << " failed!" << endl;

		// Get error message
		result = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &result);

		char* error = new char[result];
		GLint errorLength=0;
		glGetShaderInfoLog(id, result, &errorLength, error);
		cerr << error << endl;

		// Output error to log file
		ofstream errorLog("errors.log");
		errorLog << error << endl;
		errorLog.close();
		
		delete [] error;

		throw exception("Error loading shader - compile failed");
	}
	else
	{
		cout << "Shader " << filename << " compile OK" << endl;
	}


	loadedGL= false;
}

/*****************************************************************************/

ShaderObject_GL::~ShaderObject_GL(void)
{
	// Delete this shader object
	glDeleteShader(id);
}

/*****************************************************************************/

#endif