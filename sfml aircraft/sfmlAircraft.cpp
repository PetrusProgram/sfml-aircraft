#include "Application.hpp"
#include <stdexcept>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

int main()
{
	{
		try
		{
			Application app;
			app.run();
			std::cin.get();
		}
		catch (std::exception& e)
		{
			std::cout << "\nexception: " << e.what() << std::endl;
		}
	}
}
