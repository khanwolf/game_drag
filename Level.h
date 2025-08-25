#ifndef LEVEL_H
#define LEVEL_H

/*!
\file Level.h
*/

#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\System\Time.hpp>
#include "Collision2D.h"
#include "Finish.h"
#include "TileMap.h"
#include "Vehicle.h"

/*!
\enum LevelType
\brief Stores the type of the level.
*/
enum LevelType { ASPHALT, DIRT, SAND };
/*!
\enum ObjectID
\brief Stores the type of the object.
*/
enum ObjectID { BARREL, BARREL_DOWN, BARRIER, CONE, CONE_DOWN, OIL, RED_TYRE, WHITE_TYRE, TREE, TRIBUNE };

/*!
\class Level
\brief Stores and manages level data.
*/
class Level : public TileMap
{
public:
	// --- Constructor
	//! Constructor
	Level(std::string sDirectory, std::string sFileName, const std::map<std::string, ObjectID>& akObjectIDStrings, const std::map<std::string, std::shared_ptr<sf::Texture>>& akpObjectTextures, std::string sTileMapFileName);
	// --- Functions
	std::string getFilePath() const;																					//!< Returns the level's file path.
	std::string getName() const;																						//!< Returns the name of the level.
	std::vector<std::shared_ptr<GameObject>> getObjects() const;																//!< Returns the level's GameObjects and MoveableObjects.
	// --- Methods
	void draw(sf::RenderWindow& renderWindow);																			//!< Draws the next frame.
	void playAccelerationSound();																						//!< Used to prepare and play the acceleration sound.
	void playBrakeSound();																								//!< Used to prepare and play the brake sound.
	void update(const float kfDeltaTime, sf::Event& event, sf::RenderWindow& renderWindow, sf::View& view);				//!< Updates the player, view and level data.
	void setPlayerVehicle(const Vehicle& kVehicle);																		//!< Used to set the player's vehicle.
	// --- Variables
	bool m_bExit;																										//!< Used to determine when to exit the level.
private:
	// --- Variables
	bool m_bKeyDown;																									//!< Used to prevent multiple key presses.
	const std::string m_ksFileName;																						//!< Stores the level's file name.
	std::string m_sLevelName;																							//!< Stores the level name
	std::shared_ptr<Vehicle> m_pPlayer;																					//!< A pointer to the player's vehicle.
	const std::map<std::string, ObjectID>& m_akObjectIDStrings;															//!< Stores the enum values of ObjectID as strings.
	const std::map<std::string, std::shared_ptr<sf::Texture>>& m_akpObjectTextures;										//!< Stores pointers to object textures.
	std::vector<std::shared_ptr<GameObject>> m_apObjects;																//!< Stores pointers to the level's GameObject's and MoveableObjects.
	sf::Clock m_timer;																									//!< Used to time the player.
	sf::FloatRect m_fFinishZone;																						//!< A zone to mark the finish line.
	sf::Font m_boldFont;																								//!< Stores the bold font used for the UI.
	sf::Sound m_accelerateSound;																						//!< Stores the acceleration sound effect.
	sf::Sound m_brakeSound;																								//!< Stores the braking sound effect.
	sf::Sound m_crashSound;																								//!< Stores the crashing sound effect.
	sf::SoundBuffer m_accelerateSoundBuffer;																			//!< Stores the acceleration sound buffer.
	sf::SoundBuffer m_brakeSoundBuffer;																					//!< Stores the braking sound buffer.
	sf::SoundBuffer m_crashSoundBuffer;																					//!< Stores the crashing sound buffer.
	sf::Text m_gearText;																								//!< Stores text relating to the current gear.
	sf::Text m_speedText;																								//!< Stores text relating to the current speed of the player's vehicle.
	sf::Text m_timeText;																								//!< Stores text relating to the player's current lap time.
	sf::Time m_accelerateSoundOffset;																					//!< Stores the offset to be used for acceleration sound effect.
	sf::Time m_brakeSoundOffset;																						//!< Stores the offset to be used for brake sound effect.
	sf::Time m_crashSoundOffset;																						//!< Stores the offset to be used for crash sound effect.
};

#endif