#include "AsteroidsGame.h"

static const int		SCREEN_WIDTH  = 512;
static const int		SCREEN_HEIGHT = 512;


#if BUILD_DIRECTX

/**********************************/
// http://www.directxtutorial.com //
/**********************************/

// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "Window_DX.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// a struct to define a single vertex
struct VERTEX{ FLOAT X, Y, Z; D3DXCOLOR Color; };

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Create the Game object
	AsteroidsGame game;

	// Create a Window object
	Window_DX application(&game, SCREEN_WIDTH, SCREEN_HEIGHT, hInstance, nCmdShow);

	application.Initialise();
}

#else

#include "GL\GXBase\GXBase.h"
#include "Window_GL.h"
using namespace gxbase;

// Simple application class (all it does is contain our main window)
class AsteroidsApp
	: public App
{
private:
	Window_GL win;
	Game* game;

public:
	AsteroidsApp()
		: win(new AsteroidsGame(), SCREEN_WIDTH, SCREEN_HEIGHT)
	{
		game = win.GetGame();
	}

	~AsteroidsApp()
	{
		delete game;
	}
};

// This is the single instance of our application
static AsteroidsApp asteroids;

#endif