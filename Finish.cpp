#include "Finish.h"

// --- Constructor

Finish::Finish(sf::RenderWindow& renderWindow, sf::Font& font, sf::View& view) : Screen(renderWindow, "../Screens/Finish/finish.png")
{
	// Initialisation

	m_bReturn = false;

	// Correct the view
	view.setCenter(m_renderWindow.getDefaultView().getCenter());
	m_renderWindow.setView(view);

	// Load the menu button
	if (!(m_MenuButtonTexture.loadFromFile("../Screens/Finish/menu_button.png")))
		// Error
		throw std::invalid_argument("Error: Unable to open level file.");
	m_MenuButton.setTexture(m_MenuButtonTexture);
	
	// Position and scale the menu button
	m_MenuButton.setPosition(480.f, 380.f);
	m_MenuButton.setOrigin(static_cast<sf::Vector2f>(m_MenuButtonTexture.getSize()) / 2.f);
	m_MenuButton.scale(0.5f, 0.5f);

	// Finish text
	m_FinishTime.setFillColor(sf::Color::Red);
	m_FinishTime.setFont(font);
}

// --- Methods

void Finish::draw()
{
	m_renderWindow.clear();
	sf::Sprite backgroundSprite = this->getBackgroundSprite();
	backgroundSprite.scale(0.5f, 0.5f);
	m_renderWindow.draw(backgroundSprite);
	m_renderWindow.draw(m_MenuButton);
	m_renderWindow.draw(m_FinishTime);
	m_renderWindow.display();
}

void Finish::setFinalTime(const std::string ksFinalTime)
{
	// Set the final time
	m_FinishTime.setString(ksFinalTime + " Seconds");
	const sf::FloatRect kfFinishTimeGlobalBounds = m_FinishTime.getGlobalBounds();
	m_FinishTime.setOrigin(sf::Vector2f(kfFinishTimeGlobalBounds.left + (kfFinishTimeGlobalBounds.width / 2.f), kfFinishTimeGlobalBounds.top + (kfFinishTimeGlobalBounds.height / 2.f)));
	m_FinishTime.scale(0.8f, 0.8f);
	m_FinishTime.setPosition(480.f, 340.f);
}

void Finish::show()
{
	// Draw the screen
	this->draw();

	// Detect and resolve clicks
	sf::Event event;
	while (m_renderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			// Update the mouse collider
			this->updateMouseCollider(event);
			// Get the mouse collider
			const sf::FloatRect kfMouseCollider = this->getMouseCollider();
			// Check what was clicked
			if (kfMouseCollider.intersects(m_MenuButton.getGlobalBounds()))
				// Return to menu
				m_bReturn = true;
		}
		else if (event.type == sf::Event::Closed)
		{
			m_bReturn = true;
			m_renderWindow.close();
		}
	}
}