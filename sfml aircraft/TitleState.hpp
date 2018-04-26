#pragma once
#include "State.hpp"
class TitleState final : public State
{
public:
	explicit TitleState(StateStack& stack, const Context& context);

	virtual void draw() override;
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mText;
	bool mShowText{ true };
	sf::Time mTextEffectTime;
};
