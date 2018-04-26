#pragma once
#include <memory>
#include "sfml\Graphics.hpp"

namespace sf
{
	class Event;
}
namespace GUI
{
	class Component : public sf::Drawable, public sf::Transformable
	{
	public:
		Component() = default;
		virtual	~Component() = default;
		Component(const Component& other) = delete;
		Component& operator=(const Component& other) = delete;

		virtual bool isSelectable() const = 0;
		bool isSelected() const { return mIsSelected; }
		virtual void select() {	mIsSelected = true;	}
		virtual void deselect() { mIsSelected = false; }

		virtual bool isActive() const { return mIsActive; }
		virtual void activate() { mIsActive = true; }
		virtual void deactivate() {	mIsActive = false; }

		virtual void handleEvent(const sf::Event& event) = 0;

	private:
		bool mIsSelected{ false };
		bool mIsActive{ false };
	};

}