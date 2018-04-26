#include "TitleState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

TitleState::TitleState(StateStack& stack, const Context& context) : State(stack, context)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::TitleScreen));

	mText.setFont(context.fonts->get(Fonts::ID::Main));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	mText.setPosition(sf::Vector2f(context.window->getSize() / 2u));
}

void TitleState::draw()
{
	auto& window{ *getContext().window };
	window.draw(mBackgroundSprite);

	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(States::ID::Menu);
	}

	return false;
}