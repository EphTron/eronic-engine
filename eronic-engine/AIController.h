#pragma once

#include "Scene.h"
class AIController
{
public:
	AIController();
	~AIController();

	void initialize();

	void make_move();
	void analyse_scene(Scene *s);

private:
	int _difficulty;

};

