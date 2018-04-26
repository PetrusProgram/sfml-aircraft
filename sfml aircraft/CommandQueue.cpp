#include "CommandQueue.hpp"

Command CommandQueue::pop()
{
	Command command = mQueue.front();
	mQueue.pop();
	return command;
}


