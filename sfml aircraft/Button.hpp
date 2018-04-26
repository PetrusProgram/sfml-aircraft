#pragma once 
#include "Component.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace GUI
{
	class Button final : public Component
	{
	public:
		using Callback = std::function<void()>;

		enum class Type
		{
			Normal,
			Selected,
			Pressed,
			ButtonCount
		};


	public:
		explicit Button(const FontHolder& fonts, const TextureHolder& textures);

		void setCallback(Callback callback) { mCallback = std::move(callback); }
		void setText(const std::string_view text);
		void setToggle(bool flag) {	mIsToggle = flag; }

		virtual bool isSelectable() const override { return true; }
		virtual void select() override;
		virtual void deselect() override;

		virtual void activate() override;
		virtual void deactivate() override;

		virtual void handleEvent(const sf::Event& event) override {}


	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void changeTexture(Type buttonType);

	private:
		Callback mCallback;
		sf::Sprite mSprite;
		sf::Text mText;
		bool mIsToggle{ false };
	};

}