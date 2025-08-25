#ifndef MENU_H
#define MENU_H

/*!
\file Menu.h
*/

#include <memory>
#include <SFML/Audio/Music.hpp>
#include "Vehicle.h"
#include "Level.h"
#include "Screen.h"

/*!
\class Menu
\brief Used to display a menu screen.
*/
class Menu : public Screen
{
public:
	// --- Constructor
	//! Constructor
	explicit Menu(sf::RenderWindow& renderWindow);
	// --- Methods
	void draw() override;																																				//!< Used to draw the next menu frame.
	void show();																																				//!< Used to display the menu.
	// --- Variables
	bool m_bMusic;																																				//!< Stores a boolean value to toggle background music.
	bool m_bRunGame;																																			//!< Stores a boolean value that tells the menu when to run the game.
	LevelType m_ChosenLevel;																																	//!< Stores the level chosen by the player.
	VehicleType m_ChosenVehicle;																																//!< Stores the vehicle chosen by the player.
private:
	// --- Variables										
	sf::Font m_boldFont;																																		//!< Stores the bold font used for the UI.
	sf::Music m_bgMusic;																																		//!< Stores the background music for the game.
	sf::Sprite m_startButton;																																	//!< Stores the sprite for the 'start' menu option.
	sf::Sprite m_asphaltOption;																																	//!< Stores the sprite for the 'asphalt level' menu option.
	sf::Sprite m_dirtOption;																																	//!< Stores the sprite for the 'dirt level' menu option.
	sf::Sprite m_sandOption;																																	//!< Stores the sprite for the 'sand level' menu option.
	sf::Sprite m_carOption;																																		//!< Stores the sprite for the 'car' menu option.
	sf::Sprite m_motorcycleOption;																																//!< Stores the sprite for the 'motorcycle' menu option.
	sf::Sprite m_truckOption;																																	//!< Stores the sprite for the 'truck' menu option.				
	sf::Text m_MusicText;																																		//!< Stores the text for the music menu option.
	sf::Texture m_startButtonTexture;																															//!< Stores the texture for the 'start' menu option.
	sf::Texture m_asphaltTexture;																																//!< Stores the texture for the 'asphalt level' menu option.
	sf::Texture	m_dirtTexture;																																	//!< Stores the texture for the 'dirt level' menu option.
	sf::Texture m_sandTexture;																																	//!< Stores the texture for the 'sand level' menu option.
	sf::Texture m_carTexture;																																	//!< Stores the texture for the 'car' menu option.
	sf::Texture m_motorcycleTexture;																															//!< Stores the texture for the 'motorcycle' menu option.
	sf::Texture m_truckTexture;																																	//!< Stores the texture for the 'truck' menu option.
};

#endif
