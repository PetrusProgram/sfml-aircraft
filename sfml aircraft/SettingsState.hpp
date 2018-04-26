#pragma once
#include "Button.hpp"
#include "Label.hpp"
#include "State.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "Container.hpp"
#include <array>

class SettingsState final : public State
{
public:
	SettingsState(StateStack& stack, const Context& context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override { return true; }
	virtual bool handleEvent(const sf::Event& event) override;

private:
	void updateLabels();
	void addButtonLabel(Player::Action action, float y, const std::string_view text, const Context& context);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<std::shared_ptr<GUI::Button>, static_cast<int>(Player::Action::ActionCount)>	mBindingButtons;
	std::array<std::shared_ptr<GUI::Label>, static_cast<int>(Player::Action::ActionCount)> 	mBindingLabels;
};
