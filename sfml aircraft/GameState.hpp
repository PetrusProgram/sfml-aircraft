#pragma once
#include "State.hpp"
#include "Player.hpp"
#include "World.hpp"

class GameState final : public State
{
public:
	explicit GameState(StateStack& stack, const Context& context) : State(stack, context)
		, mWorld(*context.window, *context.fonts)
		, mPlayer(*context.player)
	{
		mPlayer.setMissionStatus(Player::MissionStatus::MissionRunning);
	}

	virtual void draw() override { mWorld.draw(); }
	virtual bool update(sf::Time dt) override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	World mWorld;
	Player& mPlayer;
};

