#pragma once
#include "Command.hpp"
#include <queue>

class CommandQueue
{
public:
	void push(const Command& command) { mQueue.push(command); }
	Command pop();
	bool isEmpty() const { return mQueue.empty(); }

private:
	std::queue<Command>	mQueue;
};
