#pragma once
#include "ResourceIdentifiers.hpp"
#include "State.hpp"
#include <vector>
#include <utility>
#include <functional>
#include <unordered_map>

class StateStack
{
public:
	enum class Action
	{
		Push,
		Pop,
		Clear,
	};


public:
	explicit StateStack(const State::Context& context) : mContext(context)
	{}
	StateStack(const StateStack& other) = delete;
	StateStack& operator=(const StateStack& other) = delete;

	template <typename T>
	void registerState(States::ID stateID);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(States::ID stateID) { mPendingList.push_back(PendingChange(Action::Push, stateID)); }
	void popState() { mPendingList.push_back(PendingChange(Action::Pop)); }
	void clearStates() { mPendingList.push_back(PendingChange(Action::Clear)); }

	bool isEmpty() const { return mStack.empty(); }


private:
	std::unique_ptr<State> createState(States::ID stateID);
	void applyPendingChanges();


private:
	struct PendingChange
	{
		explicit PendingChange(Action action, States::ID stateID = States::ID::None) 
			: mAction(action), mStateID(stateID)
		{}

		Action mAction;
		States::ID mStateID;
	};


private:
	std::vector<std::unique_ptr<State>> mStack;
	std::vector<PendingChange> mPendingList;

	State::Context mContext;
	std::unordered_map<States::ID, std::function<std::unique_ptr<State>()>>	mFactories;
};


template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
	{
		return std::make_unique<T>(*this, mContext);
	};
}