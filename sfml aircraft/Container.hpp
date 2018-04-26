#pragma once
#include "Component.hpp"
#include <vector>
#include <memory>

namespace GUI
{
	class Container final: public Component
	{
	public:
		void pack(std::shared_ptr<GUI::Component> component);

		virtual bool isSelectable() const override { return false; }
		virtual void handleEvent(const sf::Event& event) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		bool hasSelection() const {	return mSelectedChild >= 0;	}
		void select(std::size_t index);
		void selectNext();
		void selectPrevious();

	private:
		std::vector<std::shared_ptr<GUI::Component>> mChildren;
		int	mSelectedChild{ -1 };
	};

}