#pragma once
#include "State.hpp"

class GameOverState final : public State
{
public:
	explicit GameOverState(StateStack& stack, const Context& context);
	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event&) override { return false; }

private:
	sf::Text mGameOverText;
	sf::Time mElapsedTime;
};