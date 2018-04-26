#include "SettingsState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <string>
SettingsState::SettingsState(StateStack& stack, const Context& context) : State(stack, context)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::TitleScreen));

	addButtonLabel(Player::Action::MoveLeft, 300.f, "Move Left", context);
	addButtonLabel(Player::Action::MoveRight, 350.f, "Move Right", context);
	addButtonLabel(Player::Action::MoveUp, 400.f, "Move Up", context);
	addButtonLabel(Player::Action::MoveDown, 450.f, "Move Down", context);
	addButtonLabel(Player::Action::Fire, 500.f, "Fire", context);
	addButtonLabel(Player::Action::LaunchMissile, 550.f, "Missile", context);

	updateLabels();

	auto backButton{ std::make_shared<GUI::Button>(*context.fonts, *context.textures) };
	backButton->setPosition(80.f, 620.f);
	backButton->setText("Back");
	backButton->setCallback([this]() {requestStackPop(); });

	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	auto& window{ *getContext().window };

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding{ false };

	for (std::size_t action{ 0 }; action < static_cast<int>(Player::Action::ActionCount); ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	if (isKeyBinding)
		updateLabels();

	else
		mGUIContainer.handleEvent(event);

	return false;
}

void SettingsState::updateLabels()
{
	auto& player{ *getContext().player };

	for (std::size_t i{ 0 }; i < static_cast<int>(Player::Action::ActionCount); ++i)
	{
		auto key{ player.getAssignedKey(static_cast<Player::Action>(i)) };
		mBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string_view text, const Context& context)
{
	mBindingButtons[static_cast<int>(action)] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[static_cast<int>(action)]->setPosition(80.f, y);
	mBindingButtons[static_cast<int>(action)]->setText(text);
	mBindingButtons[static_cast<int>(action)]->setToggle(true);

	mBindingLabels[static_cast<int>(action)] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[static_cast<int>(action)]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[static_cast<int>(action)]);
	mGUIContainer.pack(mBindingLabels[static_cast<int>(action)]);
}
