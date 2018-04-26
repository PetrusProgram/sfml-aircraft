#include "Player.hpp"
#include "Aircraft.hpp"
#include "CommandQueue.hpp"
#include <string>
#include <algorithm>


struct AircraftMover
{
	AircraftMover(float vx, float vy) : velocity(vx, vy) { }

	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.accelerate(velocity * aircraft.getMaxSpeed());
	}

	sf::Vector2f velocity;
};

Player::Player() : mCurrentMissionStatus(MissionStatus::MissionRunning)
{
	mKeyBinding[sf::Keyboard::Left] = Action::MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = Action::MoveRight;
	mKeyBinding[sf::Keyboard::Up] = Action::MoveUp;
	mKeyBinding[sf::Keyboard::Down] = Action::MoveDown;
	mKeyBinding[sf::Keyboard::Space] = Action::Fire;
	mKeyBinding[sf::Keyboard::M] = Action::LaunchMissile;

	initializeActions();

	for (auto& pair : mActionBinding)
		pair.second.category = static_cast<int>(Category::Type::PlayerAircraft);
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		auto found{ mKeyBinding.find(event.key.code) };
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	for (const auto& pair : mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	for (auto itr{ mKeyBinding.begin() }; itr != mKeyBinding.end();)
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);

		else
			++itr;
	}
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	mActionBinding[Action::MoveLeft].action = derivedAction<Aircraft, AircraftMover>(AircraftMover(-1, 0));
	mActionBinding[Action::MoveRight].action = derivedAction<Aircraft, AircraftMover>(AircraftMover(+1, 0));
	mActionBinding[Action::MoveUp].action = derivedAction<Aircraft, AircraftMover>(AircraftMover(0, -1));
	mActionBinding[Action::MoveDown].action = derivedAction<Aircraft, AircraftMover>(AircraftMover(0, +1));
	mActionBinding[Action::Fire].action = derivedAction<Aircraft>([](Aircraft& a, sf::Time) { a.fire(); });
	mActionBinding[Action::LaunchMissile].action = derivedAction<Aircraft>([](Aircraft& a, sf::Time) 
	{ a.launchMissile(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case Action::MoveLeft:
	case Action::MoveRight:
	case Action::MoveDown:
	case Action::MoveUp:
	case Action::Fire:
		return true;

	default:
		return false;
	}
}
