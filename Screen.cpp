#include "Screen.h"

// --- Constructor

Screen::Screen(sf::RenderWindow& renderWindow, std::string sBackgroundPath) : m_renderWindow(renderWindow)
{
	// Initialisation

	// Mouse
	m_fMouseCollider = sf::FloatRect(0, 0, 5, 5);

	// Load the background
	if (!(m_backgroundTexture.loadFromFile(sBackgroundPath)))
		// Error
		throw std::invalid_argument("Error: Invalid background path.");
	m_backgroundSprite.setTexture(m_backgroundTexture);
}

// --- Functions

sf::Sprite Screen::getBackgroundSprite() const
{
	return m_backgroundSprite;
}

sf::FloatRect Screen::getMouseCollider() const
{
	return m_fMouseCollider;
}

// --- Methods

void Screen::draw()
{
	m_renderWindow.draw(m_backgroundSprite);
}

void Screen::updateMouseCollider(sf::Event event)
{
	m_fMouseCollider.left = event.mouseButton.x + (m_fMouseCollider.width / 2.f);
	m_fMouseCollider.top = event.mouseButton.y + (m_fMouseCollider.height / 2.f);
}