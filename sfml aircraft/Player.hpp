#pragma once
#include "Command.hpp"
#include <unordered_map>

class CommandQueue;

class  Player
{
public:
	enum class Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		ActionCount
	};

	enum class MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};

public:
	Player();

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;

	void setMissionStatus(MissionStatus status) { mCurrentMissionStatus = status; }
	MissionStatus getMissionStatus() const { return mCurrentMissionStatus; }

private:
	void initializeActions();
	static bool	isRealtimeAction(Action action);

private:
	std::unordered_map<sf::Keyboard::Key, Action> mKeyBinding;
	std::unordered_map<Action, Command>	mActionBinding;
	MissionStatus mCurrentMissionStatus;
};