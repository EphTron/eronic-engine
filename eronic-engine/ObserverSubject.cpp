#include "ObserverSubject.h"
#include "Observer.h"
#include "Message.h"

/******************************************************************************************************************/

void ObserverSubject::RegisterListener(std::string msg, Observer* o)
{
	MessageListenerMapIterator i = _messageListeners.find(msg);

	// Make entry and add listener
	if (i == _messageListeners.end())
	{
		_messageListeners[msg] = std::vector<Observer*>();
		_messageListeners[msg].push_back(o);
	}
	else
	{
		// Already have list; just add
		_messageListeners[msg].push_back(o);
	}
}

/******************************************************************************************************************/

void ObserverSubject::UnregisterListener(std::string msg, Observer* o)
{
	MessageListenerMapIterator i = _messageListeners.find(msg);

	// Exists?
	if (i != _messageListeners.end())
	{
		std::vector<Observer*>& list = i->second;

		for (ObserverListIterator j = list.begin();
			j != list.end();
			++j)
		{
			if (*j == o)
			{
				// Found it - so remove the listener
				list.erase(j);
				break;
			}
		}
	}
}

/******************************************************************************************************************/

// Broadcast message to listeners
void ObserverSubject::BroadcastMessage(Message* msg)
{
	// Dispatch message to all registered listeners
	MessageListenerMapIterator i = _messageListeners.find(msg->GetMessageType());

	// If we have registered listeners for this message
	if (i != _messageListeners.end())
	{
		std::vector<Observer*>& list = i->second;

		for (ObserverListIterator j = list.begin();
			j != list.end();
			++j)
		{
			// Send listener the message
			(*j)->OnMessage(msg);
		}
	}
}

/******************************************************************************************************************/
