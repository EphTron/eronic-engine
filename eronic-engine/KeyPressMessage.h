#pragma once
#include <string>
#include "Message.h"

class KeyPressMessage
	: public Message
{
	// Data
protected:
	int		_key;
	bool	_down;

	// Structors
public:
	KeyPressMessage(int key, bool down);
	virtual ~KeyPressMessage();


	// Gets/Sets
public:
	int GetKey()				const	{ return _key; }
	bool GetDown()				const	{ return _down; }


	// Functions
public:


};