#pragma once
#include  "sfml\include\SFML\Graphics.hpp"

class Animation final : public sf::Drawable, public sf::Transformable
{
public:
	Animation() = default;
	explicit Animation(const sf::Texture& texture) : mSprite(texture) {}

	void setTexture(const sf::Texture& texture) { mSprite.setTexture(texture); }
	const sf::Texture* getTexture() const {	return mSprite.getTexture(); }

	void setFrameSize(sf::Vector2i frameSize) {	mFrameSize = frameSize;	}
	sf::Vector2i getFrameSize() const {	return mFrameSize; }


	void setNumFrames(std::size_t numFrames) { mNumFrames = numFrames; }

	std::size_t getNumFrames() const { return mNumFrames; }

	void setDuration(sf::Time duration) { mDuration = duration; }
	sf::Time getDuration() const { return mDuration; }

	void setRepeating(bool flag) { mRepeat = flag; }
	bool isRepeating() const { return mRepeat; }

	void restart() { mCurrentFrame = 0;	}
	bool isFinished() const	{ return mCurrentFrame >= mNumFrames; }

	sf::FloatRect getLocalBounds() const 
	{ 
		return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
	}

	sf::FloatRect getGlobalBounds() const { return getTransform().transformRect(getLocalBounds()); }

	void update(sf::Time dt);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Sprite mSprite;
	sf::Vector2i mFrameSize;
	std::size_t mNumFrames{ 0 };
	std::size_t mCurrentFrame{ 0 };
	sf::Time mDuration;
	sf::Time mElapsedTime;
	bool mRepeat{ false };
};