#pragma once

class Message;

/// Abstract parent class for the Observer pattern
/// This class should be inherited by objects that want to observe ObserverSubjects
/// They will be notified of any messages.
class Observer
{
public:
	Observer()
	{
	}

	virtual ~Observer()
	{
	}

	/// Receive a message (called when notified of a message by the subject)
	virtual void OnMessage(Message*) = 0;
};

