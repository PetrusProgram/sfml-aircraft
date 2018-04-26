#pragma once
#include "State.hpp"
#include "Container.hpp"

class PauseState final : public State
{
public:
	explicit PauseState(StateStack& stack, const Context& context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override { return false; }
	virtual bool handleEvent(const sf::Event& event) override;

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mPausedText;
	GUI::Container mGUIContainer;
};
