#pragma once
#include "ResourceIdentifiers.hpp"
#include "StateIdentifiers.hpp"
#include "sfml\Graphics.hpp"
#include <memory>

namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;

class State
{
public:

	struct Context
	{
		explicit Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player)
			: window(&window), textures(&textures), fonts(&fonts), player(&player)
		{}

		sf::RenderWindow* window{ nullptr };
		TextureHolder* textures{ nullptr };
		FontHolder*	fonts{ nullptr };
		Player*	player{ nullptr };
	};


public:
	State(StateStack& stack, const Context& context) : mStack(&stack), mContext(context) {}
	State(const State& other) = delete;
	State& operator=(const State& other) = delete;
	virtual	~State() = default;

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();
	Context	getContext() const;

private:
	StateStack* mStack{ nullptr };
	Context	mContext;
};