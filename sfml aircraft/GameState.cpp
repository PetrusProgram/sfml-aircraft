#include "GameState.hpp"
#include "MenuState.hpp"


bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if (!mWorld.hasAlivePlayer())
	{
		mPlayer.setMissionStatus(Player::MissionStatus::MissionFailure);
		requestStackPush(States::ID::GameOver);
		return true;

	}
	else if (mWorld.hasPlayerReachedEnd())
	{
		mPlayer.setMissionStatus(Player::MissionStatus::MissionSuccess);
		requestStackPush(States::ID::GameOver);
		return true;
	}

	auto& commands{ mWorld.getCommandQueue() };
	mPlayer.handleRealtimeInput(commands);
	return false;
}

bool GameState::handleEvent(const sf::Event& event)
{
	auto& commands{ mWorld.getCommandQueue() };
	mPlayer.handleEvent(event, commands);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::ID::Pause);

	return true;
}