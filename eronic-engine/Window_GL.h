#pragma once

#define _WINSOCKAPI_ 
#include <winsock2.h>
#include <WS2tcpip.h>

#if BUILD_OPENGL
#include "Window.h"
#include "GL\GXBase\GXBase.h"

using namespace gxbase;

// OpenGL Window (GXBase)
class Window_GL :
	public Window, GLWindowEx
{

	// Structors
public:
	Window_GL(Game* game, int width, int height);
	virtual ~Window_GL();

	
	// Functions
public:
	virtual void Initialise();


	/////////////////////////
	// GXBase Window methods

	// Initialisation method
	void OnCreate();

	// Main draw method
	void OnDisplay();

	// Idle method - called when we're not busy doing anything else
	void OnIdle();

	// When the window gets resized
	void OnResize(int w, int h);

	// Keyboard input
	void OnKeyboard(int key, bool down);
};

#endif