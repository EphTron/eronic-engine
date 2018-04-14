#pragma once
#include <string>

class Message
{
	// Data
protected:
	std::string _type;

	// Structors
public:
	Message(std::string type);
	virtual ~Message();


	// Gets/Sets
public:
	std::string GetMessageType()		const	{ return _type; }



	// Functions
public:


};