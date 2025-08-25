#ifndef SCREEN_H
#define SCREEN_H

/*!
\file Screen.h
*/

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Window\Event.hpp>

/*!
\class Screen
\brief Used to create screens for various game states.
*/
class Screen
{
public:
	virtual ~Screen() = default;
	// --- Constructor
	//! Constructor
	Screen(sf::RenderWindow& renderWindow, std::string sBackgroundPath);
	// --- Functions
	sf::FloatRect getMouseCollider() const;						//!< Returns the mouse collider.
	sf::Sprite getBackgroundSprite() const;						//!< Returns a copy of the background sprite.
	// --- Methods
	virtual void draw();									//!< Draws the background image to the screen.
	void updateMouseCollider(sf::Event event);				//!< Updates the position of the mouse collider.
	// --- Variables
	sf::RenderWindow& m_renderWindow;						//!< A reference to the game's renderwindow.
private:
	// --- Variables
	sf::FloatRect m_fMouseCollider;							//!< Stores the dimensions and location of the mouse collider.
	sf::Sprite m_backgroundSprite;							//!< Stores the background image.
	sf::Texture m_backgroundTexture;						//!< Stores the background texture.
};

#endif