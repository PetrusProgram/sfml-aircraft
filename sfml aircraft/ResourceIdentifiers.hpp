#pragma once

namespace sf
{
	class Texture;
	class Font;
	class Shader;
}

namespace Textures
{
	enum class ID
	{
		Entities,
		Jungle,
		TitleScreen,
		Buttons,
		Explosion,
		Particle,
		FinishLine,
	};
}

namespace Shaders
{
	enum class ID
	{
		BrightnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass,
	};
}

namespace Fonts
{
	enum class ID
	{
		Main,
	};
}
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID>	ShaderHolder;