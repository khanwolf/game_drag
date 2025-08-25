#include "Menu.h"

// --- Constructor

Menu::Menu(sf::RenderWindow& renderWindow) : Screen(renderWindow, "../Screens/Menu/menu.png")
{
	// Initialisation

	// Variables
	m_bMusic = true;
	m_bRunGame = false;

	// Load the font
	if (m_boldFont.loadFromFile("../Assets/Fonts/segoeuib.ttf"))
		sf::err() << "Error: Font failed to load." << "\n";

	// Start button
	if (!(m_startButtonTexture.loadFromFile("../Screens/Menu/start_button.png")))
		throw std::invalid_argument("Error: Start button failed to load.");
	m_startButton.setTexture(m_startButtonTexture);
	m_startButton.setPosition(442.5, 362.5);
	m_startButton.scale(0.5, 0.5);

	// Level options
	if (!(m_asphaltTexture.loadFromFile("../Screens/Menu/asphalt_icon.png")) || !(m_dirtTexture.loadFromFile("../Screens/Menu/dirt_icon.png")) || !(m_sandTexture.loadFromFile("../Screens/Menu/sand_icon.png")))
		throw std::invalid_argument("Error: Level option failed to load.");

	// Set the textures
	m_asphaltOption.setTexture(m_asphaltTexture);
	m_dirtOption.setTexture(m_dirtTexture);
	m_sandOption.setTexture(m_sandTexture);

	// Position the options
	m_asphaltOption.setPosition(422, 212);
	m_dirtOption.setPosition(480, 212);
	m_sandOption.setPosition(538, 212);

	// Scale the options
	m_asphaltOption.scale(0.5, 0.5);
	m_dirtOption.scale(0.5, 0.5);
	m_sandOption.scale(0.5, 0.5);

	// Set the origins of the options
	m_asphaltOption.setOrigin(static_cast<sf::Vector2f>(m_asphaltTexture.getSize()) / 2.f);
	m_dirtOption.setOrigin(static_cast<sf::Vector2f>(m_dirtTexture.getSize()) / 2.f);
	m_sandOption.setOrigin(static_cast<sf::Vector2f>(m_sandTexture.getSize()) / 2.f);

	// Vehicle options
	if (!(m_carTexture.loadFromFile("../Screens/Menu/car_icon.png")) || !(m_motorcycleTexture.loadFromFile("../Screens/Menu/motorcycle_icon.png")) || !(m_truckTexture.loadFromFile("../Screens/Menu/truck_icon.png")))
		throw std::invalid_argument("Error: Vehicle option failed to load.");

	// Set the textures
	m_carOption.setTexture(m_carTexture);
	m_motorcycleOption.setTexture(m_motorcycleTexture);
	m_truckOption.setTexture(m_truckTexture);

	// Position the options
	m_carOption.setPosition(422, 318);
	m_motorcycleOption.setPosition(480, 318);
	m_truckOption.setPosition(538, 318);

	// Scale the options
	m_carOption.scale(0.5, 0.5);
	m_motorcycleOption.scale(0.5, 0.5);
	m_truckOption.scale(0.5, 0.5);

	// Set the origins of the options
	m_carOption.setOrigin(static_cast<sf::Vector2f>(m_carTexture.getSize()) / 2.f);
	m_motorcycleOption.setOrigin(static_cast<sf::Vector2f>(m_motorcycleTexture.getSize()) / 2.f);
	m_truckOption.setOrigin(static_cast<sf::Vector2f>(m_truckTexture.getSize()) / 2.f);

	// Load and play the music
	if (!(m_bgMusic.openFromFile("../Assets/Music/VoiceOverUnder.ogg")))
		// Error
		throw std::invalid_argument("Error: Failed to load music file.");
	else
	{
		m_bgMusic.setLoop(true);
		m_bgMusic.setVolume(25);
		m_bgMusic.play();
	}

	// Music option
	m_MusicText.setFillColor(sf::Color::White);
	m_MusicText.setFont(m_boldFont);
	m_MusicText.setString("Music");
	const sf::FloatRect kfMusicTextGlobalBounds = m_MusicText.getGlobalBounds();
	m_MusicText.setOrigin(kfMusicTextGlobalBounds.left + (kfMusicTextGlobalBounds.width / 2.f), kfMusicTextGlobalBounds.top + (kfMusicTextGlobalBounds.height / 2.f));
	m_MusicText.setScale(0.8f, 0.8f);
	m_MusicText.setPosition(480, 450);

	// Default options
	m_ChosenLevel = ASPHALT;
	m_asphaltOption.setScale(0.55f, 0.55f);
	m_ChosenVehicle = CAR;
	m_carOption.setScale(0.55f, 0.55f);
}

// --- Methods

void Menu::draw()
{
	m_renderWindow.clear();
	sf::Sprite backgroundSprite = this->getBackgroundSprite();
	backgroundSprite.scale(0.5f, 0.5f);
	m_renderWindow.draw(backgroundSprite);
	m_renderWindow.draw(m_startButton);
	m_renderWindow.draw(m_asphaltOption);
	m_renderWindow.draw(m_dirtOption);
	m_renderWindow.draw(m_sandOption);
	m_renderWindow.draw(m_carOption);
	m_renderWindow.draw(m_motorcycleOption);
	m_renderWindow.draw(m_truckOption);
	m_renderWindow.draw(m_MusicText);
	m_renderWindow.display();
}

void Menu::show()
{
	// Draw the menu
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
			if (m_asphaltOption.getGlobalBounds().intersects(kfMouseCollider))
			{
				// Prevent double-scaling
				m_asphaltOption.setScale(0.55f, 0.55f);
				m_dirtOption.setScale(0.5f, 0.5f);
				m_sandOption.setScale(0.5f, 0.5f);
				m_ChosenLevel = ASPHALT;
			}
			else if (m_dirtOption.getGlobalBounds().intersects(kfMouseCollider))
			{
				m_dirtOption.setScale(0.55f, 0.55f);
				m_asphaltOption.setScale(0.5f, 0.5f);
				m_sandOption.setScale(0.5f, 0.5f);
				m_ChosenLevel = DIRT;
			}
			else if (m_sandOption.getGlobalBounds().intersects(kfMouseCollider))
			{
				m_sandOption.setScale(0.55f, 0.55f);
				m_asphaltOption.setScale(0.5f, 0.5f);
				m_dirtOption.setScale(0.5f, 0.5f);
				m_ChosenLevel = SAND;
			}
			else if (m_carOption.getGlobalBounds().intersects(kfMouseCollider))
			{
				m_carOption.setScale(0.55f, 0.55f);
				m_motorcycleOption.setScale(0.5f, 0.5f);
				m_truckOption.setScale(0.5f, 0.5f);
				m_ChosenVehicle = CAR;
			}
			else if (m_motorcycleOption.getGlobalBounds().intersects(kfMouseCollider))
			{
				m_motorcycleOption.setScale(0.55f, 0.55f);
				m_carOption.setScale(0.5f, 0.5f);
				m_truckOption.setScale(0.5f, 0.5f);
				m_ChosenVehicle = MOTORCYCLE;
			}
			else if (m_truckOption.getGlobalBounds().intersects(kfMouseCollider))
			{
				m_truckOption.setScale(0.55f, 0.55f);
				m_motorcycleOption.setScale(0.5f, 0.5f);
				m_carOption.setScale(0.5f, 0.5f);
				m_ChosenVehicle = TRUCK;
			}
			else if (m_startButton.getGlobalBounds().intersects(kfMouseCollider))
				m_bRunGame = true;
			else if (m_MusicText.getGlobalBounds().intersects(kfMouseCollider))
			{
				if (m_bMusic)
				{
					m_bMusic = false;
					m_MusicText.setFillColor(sf::Color::Color(170, 170, 170, 255));
				}
				else
				{
					m_bMusic = true;
					m_MusicText.setFillColor(sf::Color::White);
				}
			}
		}
		else if (event.type == sf::Event::Closed)
		{
			m_bRunGame = true;
			m_renderWindow.close();
		}
	}

	// Check for music toggle
	if (m_bMusic)
	{
		if (m_bgMusic.getStatus() == sf::SoundSource::Stopped)
			m_bgMusic.play();
	}
	else
		m_bgMusic.stop();
}