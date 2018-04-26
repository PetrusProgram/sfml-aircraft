#include "TextNode.hpp"
#include "Utility.hpp"

TextNode::TextNode(const FontHolder& fonts, const std::string_view text)
{
	mText.setFont(fonts.get(Fonts::ID::Main));
	mText.setCharacterSize(20);
	setString(text);
}

void TextNode::setString(const std::string_view text)
{
	mText.setString(text.data());
	centerOrigin(mText);
}