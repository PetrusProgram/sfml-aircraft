#include "StateStack.hpp"
#include <cassert>

void StateStack::update(sf::Time dt)
{
	for (auto itr{ mStack.rbegin() }; itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	for (const auto& state : mStack)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr{ mStack.rbegin() }; itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}
	applyPendingChanges();
}

std::unique_ptr<State> StateStack::createState(States::ID stateID)
{

	auto found{ mFactories.find(stateID) };
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (const auto& change : mPendingList)
	{
		switch (change.mAction)
		{
		case Action::Push:
			mStack.push_back(createState(change.mStateID));
			break;

		case Action::Pop:
			mStack.pop_back();
			break;

		case Action::Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}

