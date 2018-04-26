#include "MenuState.hpp"
#include "ResourceHolder.hpp"
#include "Button.hpp"
#include "Utility.hpp"
MenuState::MenuState(StateStack& stack, const Context& context) : State(stack, context)
{
	auto& texture{ context.textures->get(Textures::ID::TitleScreen) };
	mBackgroundSprite.setTexture(texture);

	auto playButton{ std::make_shared<GUI::Button>(*context.fonts, *context.textures) };
	playButton->setPosition(100, 300);
	playButton->setText("Play");
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::ID::Game);
	});

	auto settingsButton{ std::make_shared<GUI::Button>(*context.fonts, *context.textures) };
	settingsButton->setPosition(100, 350);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()
	{
		requestStackPush(States::ID::Settings);
	});

	auto exitButton{ std::make_shared<GUI::Button>(*context.fonts, *context.textures) };
	exitButton->setPosition(100, 400);
	exitButton->setText("Exit");
	exitButton->setCallback([this]()
	{
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);
}

void MenuState::draw()
{
	auto& window{ *getContext().window };

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}


bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
