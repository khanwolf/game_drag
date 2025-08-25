/*!
\mainpage  Geared | Developer Documentation

\brief     A top-down 2D racing game.
\author    Harish Bhagat
\version   1.0.0
\date      2016
*/

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Menu.h"

int main()
{
	// Create and configure the window settings
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;

	// Create the RenderWindow
	const sf::Vector2u kuiRenderWindowSize = sf::Vector2u(960, 540);
	sf::RenderWindow renderWindow(sf::VideoMode(kuiRenderWindowSize.x, kuiRenderWindowSize.y), "Geared: Version 1.0.0", sf::Style::Titlebar | sf::Style::Close, contextSettings);
	// Set the RenderWindow's properties
	renderWindow.setFramerateLimit(60);

	// Create the view
	sf::View view = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(kuiRenderWindowSize.x), static_cast<float>(kuiRenderWindowSize.y)));
	// Initialise the menu
	Menu menu(renderWindow);

	while (renderWindow.isOpen())
	{
		// Update the menu
		while (menu.m_bRunGame == false)
			menu.show();

		// Initialise the game
		Game game(renderWindow, view, menu.m_ChosenVehicle, menu.m_ChosenLevel);

		// Update the game
		game.run();

		// Show the menu
		menu.m_bRunGame = false;
	}

	return 0;
}