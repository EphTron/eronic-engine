#pragma once

#include "Vector4.h"
#include "WordDisplay.h"

class MenuPoint {
	MenuPoint();
	MenuPoint(Vector4 pos, float scl, std::string txt) :
		selectable(true),
		selected(false),
		position(pos),
		scale(scl),
		text_display(new WordDisplay())
	{
		text_display->SetPosition(position);
		text_display->SetColour(Colour::Grey());
		text_display->SetWord("JOIN AND PLAY");
	}
	~MenuPoint();



	bool selectable;
	bool selected;
	Vector4 position;
	float scale;
	WordDisplay* text_display;
};