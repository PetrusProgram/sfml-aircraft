#pragma once
#include "SceneNode.hpp"
class SpriteNode final : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture) : mSprite(texture) {}
	explicit SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect) : mSprite(texture, textureRect)
	{}

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mSprite, states);
	}
private:
	sf::Sprite	mSprite;
};
