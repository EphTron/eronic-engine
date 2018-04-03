#include "Window.h"
#include <stdlib.h>
#include <time.h>

Window*	Window::TheWindow = 0;

/******************************************************************************************************************/

Window::Window(Game* game, int width, int height)
	:	_renderer(0),
		_game(game),
		_width(width),
		_height(height)
{
	// Set static singleton
	TheWindow = this;

	// Reset RNG
	srand(time(NULL));

}

/******************************************************************************************************************/

Window::~Window()
{
}

/******************************************************************************************************************/
