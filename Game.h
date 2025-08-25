#ifndef GAME_H
#define GAME_H

/*!
\file	Game.h
*/

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>

#include "Level.h"
#include "TileMap.h"
#include "Vehicle.h"

/*!
\class	Game
\brief	Manages Levels, scoring, GameObjects and the main game loop.
*/
class Game
{
public:
	// --- Constructor
	//! Constructor
	Game(sf::RenderWindow& renderWindow, sf::View& view, VehicleType vehicleType, LevelType levelType);							 
	// --- Functions
	int getScore() const;																												 //!< Returns the player's current score.
	// --- Methods
	void addScore(const int kiAmount);																							 //!< Used to increase the player's current score.
	void loadLevel(const LevelType kLevelType);																					 //!< Used to load a level.
	void resetScore();																											 //!< Used to reset the score back to 0.
	void run();																													 //!< Used to begin the game loop.
	// -- Variables
	bool m_bExitGame;																											 //!< Used to determine whether or not to return to the main menu.
private:
	// --- Methods
	void loadObjectTextures();																									 //!< Used to load all possible object textures into memory.
	// --- Variables
	int m_iScore;																												 //!< Used to store the player's current score.
	float m_fDeltaTime;																											 //!< Used to store delta time in seconds.
	std::map<std::string, ObjectID> m_aObjectIDStrings;																			 //!< Stores the enum values of ObjectID as strings.
	std::map<std::string, std::shared_ptr<sf::Texture>> m_apObjectTextures;														 //!< Stores pointers to object textures.
	std::shared_ptr<Level> pCurrentLevel;																						 //!< A pointer used to refer to the current level.
	sf::Clock m_clock;																											 //!< Used to retrieve delta time.
	sf::RenderWindow& m_renderWindow;																							 //!< The RenderWindow used for all game activites.
	sf::View m_view;																											 //!< Stores the view.
};

#endif