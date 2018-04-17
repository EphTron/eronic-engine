#include "TweakBar.h"



TweakBar::TweakBar()
{
}


TweakBar::~TweakBar()
{

}

void TweakBar::add_tweak_bar(int * variable, std::string & variable_desc)
{
	TwBar *myBar;
	myBar = TwNewBar("NameOfMyTweakBar");
	TwAddVarRW(myBar, "NameOfMyVariable", TW_TYPE_UINT32, variable, variable_desc.c_str());
}

void TweakBar::draw()
{
	TwDraw();
}

void TweakBar::terminate()
{
	TwTerminate();
}
