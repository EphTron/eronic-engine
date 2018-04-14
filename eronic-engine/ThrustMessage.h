#pragma once
#include <string>
#include "Message.h"
#include "Vector4.h"

class ThrustMessage
	: public Message
{
	// Data
protected:
	Vector4 _thrust;

	// Structors
public:
	ThrustMessage(Vector4 thrustAmt);
	virtual ~ThrustMessage();


	// Gets/Sets
public:
	Vector4 GetThrust()		const	{ return _thrust; }



	// Functions
public:


};