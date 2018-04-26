#include "Button.hpp"
#include "Utility.hpp"


namespace GUI
{

	Button::Button(const FontHolder& fonts, const TextureHolder& textures) 
		: mSprite(textures.get(Textures::ID::Buttons)), mText("", fonts.get(Fonts::ID::Main), 16)
	{
		changeTexture(Type::Normal);
		auto bounds{ mSprite.getLocalBounds() };
		mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
	}

	void Button::setText(const std::string_view text)
	{
		mText.setString(text.data());
		centerOrigin(mText);
	}

	void Button::select()
	{
		Component::select();

		changeTexture(Type::Selected);
	}

	void Button::deselect()
	{
		Component::deselect();

		changeTexture(Type::Normal);
	}

	void Button::activate()
	{
		Component::activate();

		if (mIsToggle)
			changeTexture(Type::Pressed);

		if (mCallback)
			mCallback();

		if (!mIsToggle)
			deactivate();
	}

	void Button::deactivate()
	{
		Component::deactivate();

		if (mIsToggle)
		{
			if (isSelected())
				changeTexture(Type::Selected);
			else
				changeTexture(Type::Normal);
		}
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mSprite, states);
		target.draw(mText, states);
	}

	void Button::changeTexture(Type buttonType)
	{
		sf::IntRect textureRect(0, 50 * static_cast<int>(buttonType), 200, 50);
		mSprite.setTextureRect(textureRect);
	}

}
