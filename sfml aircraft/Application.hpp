#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "StateStack.hpp"

class Application
{
public:
	Application();
	void run();

private:
	void processInput();
	void update(sf::Time dt) { mStateStack.update(dt); }
	void render();

	void updateStatistics(const sf::Time& dt);
	void registerStates();


private:
	inline static const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	sf::RenderWindow mWindow;
	TextureHolder mTextures;
	FontHolder mFonts;
	Player mPlayer;
	StateStack mStateStack;

	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames{ 0 };
};
