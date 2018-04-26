#include "Application.hpp"
#include "GameState.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"
#include "Utility.hpp"
#include "State.hpp"
#include "GameOverState.hpp"
#include <iostream>

Application::Application() : mWindow(sf::VideoMode(1024, 768), "SFML aircraft", sf::Style::Close),
	mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
{
	mWindow.setKeyRepeatEnabled(false);
	mFonts.load(Fonts::ID::Main, "Media/Sansation.ttf");

	mTextures.load(Textures::ID::TitleScreen, "Media/Textures/sky.jpg");
	mTextures.load(Textures::ID::Buttons, "Media/Textures/Buttons.png");


	mStatisticsText.setFont(mFonts.get(Fonts::ID::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::ID::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate;

	while (mWindow.isOpen())
	{
		sf::Time dt{ clock.restart() };
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);
			

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}


void Application::render()
{
	mWindow.clear();
	mStateStack.draw();
	
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(const sf::Time& dt)
{

	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{

		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::ID::Title);
	mStateStack.registerState<MenuState>(States::ID::Menu);
	mStateStack.registerState<GameState>(States::ID::Game);
	mStateStack.registerState<PauseState>(States::ID::Pause);
	mStateStack.registerState<SettingsState>(States::ID::Settings);
	mStateStack.registerState<GameOverState>(States::ID::GameOver);
}
