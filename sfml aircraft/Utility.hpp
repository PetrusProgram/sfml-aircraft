#pragma once
#include "sfml\Graphics.hpp"
namespace sf
{
	class Sprite;
	class Text;
}
class Animation;

std::string	toString(sf::Keyboard::Key key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

float toDegree(float radian);
float toRadian(float degree);

 int randomInt(int exclusiveMax);

float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

