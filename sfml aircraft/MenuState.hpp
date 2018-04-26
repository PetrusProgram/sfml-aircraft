#pragma once
#include "State.hpp"
#include "Container.hpp"
class MenuState final : public State
{
public:
	explicit MenuState(StateStack& stack, const Context& context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override {	return false; }
	virtual bool handleEvent(const sf::Event& event) override;

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
};