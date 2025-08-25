#ifndef FINISH_H
#define FINISH_H

/*!
\file Finish.h
*/

#include <SFML\Graphics\Text.hpp>
#include "Screen.h"

/*!
\class Finish
\brief Used to display a finish screen.
*/
class Finish : public Screen
{
public:
	// --- Constructor
	//! Constructor
	Finish(sf::RenderWindow& renderWindow, sf::Font& font, sf::View& view);
	// --- Methods
	void draw() override;																																				//!< Used to draw the next finish frame.
	void setFinalTime(const std::string ksFinalTime);																											//!< Used to set the final time.
	void show();																																				//!< Used to display the finish screen.
	// --- Variables
	bool m_bReturn;																																				//!< Stores a boolean value that tells the finish screen when to return to the menu.
private:
	// --- Variables
	sf::Sprite m_MenuButton;																																	//!< Stores the menu button sprite
	sf::Text m_FinishTime;																																		//!< Stores the player's lap time.
	sf::Texture m_MenuButtonTexture;																															//!< Stores the menu button sprite texture
};

#endif