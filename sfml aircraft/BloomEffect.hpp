#pragma once
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "PostEffect.hpp"
#include <array>


class BloomEffect final : public PostEffect
{
public:
	BloomEffect();
	virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) override;

private:
	using RenderTextureArray = std::array<sf::RenderTexture, 2>;

private:
	void prepareTextures(sf::Vector2u size);

	void filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
	void blurMultipass(RenderTextureArray& renderTextures);
	void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
	void downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
	void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);


private:
	ShaderHolder mShaders;
	sf::RenderTexture mBrightnessTexture;
	RenderTextureArray mFirstPassTextures;
	RenderTextureArray mSecondPassTextures;
};
