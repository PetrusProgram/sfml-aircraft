#pragma once
#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "Command.hpp"

class Aircraft;

class Pickup final : public Entity
{
public:
	enum class Type
	{
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		TypeCount
	};

public:
	explicit Pickup(Type type, const TextureHolder& textures);

	virtual unsigned int getCategory() const override 
	{
		return static_cast<unsigned int>(Category::Type::Pickup);
	}
	virtual sf::FloatRect getBoundingRect() const override 
	{
		return getWorldTransform().transformRect(mSprite.getGlobalBounds());
	}
	void apply(Aircraft& player) const;

protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(mSprite, states);
	}

private:
	Type mType;
	sf::Sprite mSprite;
};