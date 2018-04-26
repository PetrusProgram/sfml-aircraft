#include "GameOverState.hpp"
#include "Utility.hpp"
#include "Player.hpp"
#include "ResourceHolder.hpp"

GameOverState::GameOverState(StateStack& stack, const Context& context) : State(stack, context)
{
	auto& font{ context.fonts->get(Fonts::ID::Main) };
	sf::Vector2f windowSize(context.window->getSize());

	mGameOverText.setFont(font);
	if (context.player->getMissionStatus() == Player::MissionStatus::MissionFailure)
		mGameOverText.setString("Mission failed!");

	else
		mGameOverText.setString("Mission successful!");

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	auto& window{ *getContext().window };
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(3))
	{
		requestStateClear();
		requestStackPush(States::ID::Menu);
	}
	return false;
}