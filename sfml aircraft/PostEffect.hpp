#pragma once
#include "sfml\Graphics.hpp"

namespace sf
{
	class RenderTarget;
	class RenderTexture;
	class Shader;
}

class PostEffect
{
public:
	PostEffect() = default;
	virtual	~PostEffect() = default;
	PostEffect(const PostEffect& other) = delete;
	PostEffect& operator=(const PostEffect& other) = delete;

	virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;
	static bool	isSupported() {	return sf::Shader::isAvailable(); }

protected:
	static void	applyShader(const sf::Shader& shader, sf::RenderTarget& output);
};