#pragma once
#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

class TextNode final : public SceneNode
{
public:
	explicit TextNode(const FontHolder& fonts, const std::string_view text);
	void setString(const std::string_view text);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(mText, states);
	}

private:
	sf::Text			mText;
};