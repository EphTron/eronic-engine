#pragma once

#include <vector>


class TweakBar
{
public:
	TweakBar();
	~TweakBar();
	virtual void init(void * device, int width, int height) = 0;
	void add_tweak_bar(int* variable, std::string& variable_desc);
	void draw();
	void terminate();

private:
	//std::vector<TwBar *> tweak_bars;
	
};

