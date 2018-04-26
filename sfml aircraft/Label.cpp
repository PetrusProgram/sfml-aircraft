#include "Label.hpp"
#include "Utility.hpp"

namespace GUI
{
	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mText, states);
	}
}