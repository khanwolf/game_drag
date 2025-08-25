#include "Game.h"
#include "Vehicle.h"
#include "SFML/Graphics.hpp"

// --- Constructor

Game::Game(sf::RenderWindow& renderWindow, sf::View& view, VehicleType vehicleType, LevelType levelType) : m_renderWindow(renderWindow), m_view(view)
{
	// Initialisation
	m_iScore = 0;
	m_bExitGame = false;
	m_clock.restart();

	// Load the object textures
	loadObjectTextures();
		
	// Load the level
	if (levelType == ASPHALT)
		loadLevel(ASPHALT);
	else if (levelType == DIRT)
		loadLevel(DIRT);
	else if (levelType == SAND)
		loadLevel(SAND);

	// Load the vehicle
	std::unique_ptr<Vehicle> pVehicle;

	if (vehicleType == CAR)
		pVehicle = std::make_unique<Vehicle>(Vehicle(sf::Vector2f(0.f, 0.325f), 8.f, 0.5f, CAR));
	else if (vehicleType == TRUCK)
		pVehicle = std::make_unique<Vehicle>(Vehicle(sf::Vector2f(0.f, 0.330f), 9.f, 0.55f, TRUCK));
	else if (vehicleType == MOTORCYCLE)
		pVehicle = std::make_unique<Vehicle>(Vehicle(sf::Vector2f(0.f, 0.320f), 8.5f, 0.45f, MOTORCYCLE));

	const sf::FloatRect kfVehicleBounds = pVehicle->getGlobalBounds();
	pVehicle->setOrigin(kfVehicleBounds.left + (kfVehicleBounds.width / 2.f), kfVehicleBounds.top + (kfVehicleBounds.height / 2.f));
	pVehicle->setPosition(512, 63744);
	pCurrentLevel->setPlayerVehicle(*pVehicle);
}

// --- Functions

int Game::getScore() const
{
	return m_iScore;
}

// --- Methods

void Game::addScore(const int kiAmount)
{
	m_iScore += kiAmount;
}

void Game::loadLevel(const LevelType kLevelType)
{
	// Reset the current score
	resetScore();

	switch (kLevelType)
	{
	case ASPHALT:
		pCurrentLevel = std::make_shared<Level>("../Levels/Asphalt/", "l_Asphalt.txt", m_aObjectIDStrings, m_apObjectTextures, "tm_Asphalt.txt");
		break;
	case DIRT:
		pCurrentLevel = std::make_shared<Level>("../Levels/Dirt/", "l_Dirt.txt", m_aObjectIDStrings, m_apObjectTextures, "tm_Dirt.txt");
		break;
	case SAND:
		pCurrentLevel = std::make_shared<Level>("../Levels/Sand/", "l_Sand.txt", m_aObjectIDStrings, m_apObjectTextures, "tm_Sand.txt");
		break;
	default:
		return;
	}
}

void Game::loadObjectTextures()
{
	// Define the object ids
	m_aObjectIDStrings["BARREL"] = BARREL;
	m_aObjectIDStrings["BARREL_DOWN"] = BARREL_DOWN;
	m_aObjectIDStrings["BARRIER"] = BARRIER;
	m_aObjectIDStrings["CONE"] = CONE;
	m_aObjectIDStrings["CONE_DOWN"] = CONE_DOWN;
	m_aObjectIDStrings["OIL"] = OIL;
	m_aObjectIDStrings["RED_TYRE"] = RED_TYRE;
	m_aObjectIDStrings["WHITE_TYRE"] = WHITE_TYRE;
	m_aObjectIDStrings["TREE"] = TREE;
	m_aObjectIDStrings["TRIBUNE"] = TRIBUNE;

	// Load the object textures
	const std::string ksDirectory = "../Assets/Objects/";
	const std::string ksFileType = "PNG";

	std::map<std::string, std::string>::iterator it;
	for (auto& it : m_aObjectIDStrings)
	{
		sf::Texture texture;
		if (!(texture.loadFromFile(ksDirectory + it.first + "." + ksFileType)))
			// Error
			throw std::invalid_argument("Error: Failed to load object textures.");

		// Add the texture
		m_apObjectTextures[it.first] = std::make_shared<sf::Texture>(texture);
	}
}

void Game::resetScore()
{
	m_iScore = 0;
}

void Game::run()
{
	while (m_renderWindow.isOpen() && m_bExitGame == false)
	{
		// Get delta time
		m_fDeltaTime = m_clock.restart().asSeconds();

		// Check for events
		sf::Event event;
		while (m_renderWindow.pollEvent(event))
			if (event.type == sf::Event::Closed)
				m_renderWindow.close();

		m_renderWindow.clear();
		pCurrentLevel->update(m_fDeltaTime, event, m_renderWindow, m_view);
		
		// Check for exits
		if (pCurrentLevel->m_bExit)
			m_bExitGame = true;
		
		m_renderWindow.display();
	}
}