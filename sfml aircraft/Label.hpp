#pragma once
#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

namespace GUI
{
	class Label final : public Component
	{
	public:
		explicit Label(const std::string_view text, const FontHolder& fonts) : mText(text.data(), 
			fonts.get(Fonts::ID::Main), 16)
		{}

		virtual bool isSelectable() const override { return false; }
		void setText(const std::string_view text) {	mText.setString(text.data()); }

		virtual void handleEvent(const sf::Event& event) override {};

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Text mText;
	};

}
