#if BUILD_OPENGL
#include "Window_GL.h"
#include "Renderer_GL.h"
#include "Game.h"

/******************************************************************************************************************/

Window_GL::Window_GL(Game* game, int width, int height)
	: Window(game, width, height)
{
}

/******************************************************************************************************************/

Window_GL::~Window_GL()
{
	_renderer->Destroy();
}

/******************************************************************************************************************/

void Window_GL::Initialise()
{
	SetSize(512, 512);
	SetDepthBits(24);
	SetTitle("Asteroids GL");
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(512, 512);
	// wglSwapIntervalEXT(0); // Turn off frame rate
	

	// Initialise OpenGL
	_renderer = new Renderer_GL();
	_renderer->Initialise(_width, _height);

	// Setup Game
	_game->Initialise(this);
}

/******************************************************************************************************************/

// Initialisation method
void Window_GL::OnCreate()
{
	Initialise();
}

/******************************************************************************************************************/

// Main draw method
void Window_GL::OnDisplay()
{
	_game->Run();

	if (_game->GetQuitFlag())
	{
		Close();
	}

	SwapBuffers();
}

/******************************************************************************************************************/

// Idle method - called when we're not busy doing anything else
void Window_GL::OnIdle()
{
	Redraw();
}

/******************************************************************************************************************/

// When the window gets resized
void Window_GL::OnResize(int w, int h)
{
	GLWindowEx::OnResize(w, h);
	//_PM = glm::ortho(-Width() / 100.0f, +Width() / 100.0f, -Height() / 100.0f, +Height() / 100.0f, -1.0f, +1.0f);
	Redraw();
}

/******************************************************************************************************************/

// Keyboard input
void Window_GL::OnKeyboard(int key, bool down)
{
	_game->OnKeyboard(key, down);
}

/******************************************************************************************************************/

#endif