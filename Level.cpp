#include "Level.h"

// --- Constructor

Level::Level(std::string sDirectory, std::string sFileName, const std::map<std::string, ObjectID>& akObjectIDStrings, const std::map<std::string, std::shared_ptr<sf::Texture>>& akpObjectTextures, std::string sTileMapFileName) : TileMap(sDirectory, sTileMapFileName), m_ksFileName(sFileName), m_akObjectIDStrings(akObjectIDStrings), m_akpObjectTextures(akpObjectTextures)
{
	// Initialisation
	m_bExit = false;
	m_bKeyDown = false;

	// Start the clock
	m_timer.restart();

	// Sound
	if (!(m_accelerateSoundBuffer.loadFromFile("../Assets/Sounds/accelerate.ogg")) || !(m_brakeSoundBuffer.loadFromFile("../Assets/Sounds/brake.ogg")) || !(m_crashSoundBuffer.loadFromFile("../Assets/Sounds/crash.ogg")))
		sf::err() << "Error: Some sounds have failed to load." << "\n";
	else
	{
		// Set the buffers
		m_accelerateSound.setBuffer(m_accelerateSoundBuffer);
		m_brakeSound.setBuffer(m_brakeSoundBuffer);
		m_crashSound.setBuffer(m_crashSoundBuffer);

		// Set the offsets
		m_accelerateSoundOffset = sf::seconds(1);
		m_accelerateSound.setPlayingOffset(m_accelerateSoundOffset);
		m_brakeSoundOffset = sf::seconds(3);
		m_brakeSound.setPlayingOffset(m_accelerateSoundOffset);
		m_crashSoundOffset = sf::seconds(1);
		m_crashSound.setPlayingOffset(m_accelerateSoundOffset);

		m_brakeSound.setVolume(50);
		m_crashSound.setVolume(50);
	}

	// Vehicle info
	if (m_boldFont.loadFromFile("../Assets/Fonts/segoeuib.ttf"))
		sf::err() << "Error: Font failed to load." << "\n";

	m_gearText.setFillColor(sf::Color::Yellow);
	m_speedText.setFillColor(sf::Color::Blue);
	m_gearText.setFont(m_boldFont);
	m_speedText.setFont(m_boldFont);

	// Timer text
	m_timeText.setFillColor(sf::Color::Magenta);
	m_timeText.setFont(m_boldFont);

	// Read the level file
	std::ifstream levelInfo;
	std::string sLineDelimiter = ":";
	std::string sLine;
	levelInfo.open(m_ksDirectory + m_ksFileName);
	
	if (levelInfo.is_open())
	{
		while (getline(levelInfo, sLine))
		{
			// Check if the line is a blank or a comment, if not remove any whitespace
			if (gfo::isBlank(sLine) || gfo::isComment(sLine))
				continue;
			else
				gfo::removeWhiteSpace(sLine);
			
			// Split the line
			std::vector<std::string> asSplit = gfo::splitString(sLine, sLineDelimiter);
			std::string sType = asSplit.at(0);
			std::string sValue = asSplit.at(1);

			// Determine the info type and set the appropriate variable
			if (sType == "Name")
				m_sLevelName = sValue;
			else if (sType == "Objects" || sType == "MoveableObjects")
			{
				// Get the objects

				// Create the object pointer
				std::shared_ptr<GameObject> pGameObject;
				std::shared_ptr<MoveableObject> pMoveableObject;

				// Get the values
				std::vector<gfo::ObjectProperties> aObjectProperties = gfo::getObjectProperties(sValue, ",");

				// Create and store the objects
				for (gfo::ObjectProperties& op : aObjectProperties)
				{
					// Deduce the object type
					auto opIt = m_akObjectIDStrings.find(op.sIdentifier);
					auto textureIt = m_akpObjectTextures.find(op.sIdentifier);

					// Create the object, only OIL has differing properties
					if (sType == "Objects")
						pGameObject = (opIt->second == OIL) ? std::make_shared<GameObject>(false, false, CIRCLE, *textureIt->second) : std::make_shared<GameObject>(false, true, RECTANGLE, *textureIt->second);
					else
					{
						// Moveable object
						if (opIt->second == BARREL || opIt->second == CONE)
						{
							// Get the fallen texture
							auto& fallenTextureIt = m_akpObjectTextures.find(op.sIdentifier + "_DOWN");
							// Create the object
							pMoveableObject = std::make_shared<MoveableObject>(0.025f, CIRCLE, *textureIt->second, std::shared_ptr<sf::Texture>(fallenTextureIt->second));
						}
						else
						{
							// Create the object: RED_TYRE or WHITE_TYRE
							pMoveableObject = std::make_shared<MoveableObject>(0.25f, CIRCLE, *textureIt->second);
						}

						// Store the MoveableObject pointer in the GameObject pointer
						pGameObject = pMoveableObject;
					}

					// Set the object's position
					pGameObject->setPosition(op.fLocationX, op.fLocationY);
					sf::Vector2u uiNewOrigin = textureIt->second->getSize() / static_cast<unsigned int>(2);
					pGameObject->setOrigin(static_cast<float>(uiNewOrigin.x), static_cast<float>(uiNewOrigin.y));

					// Insert the object
					m_apObjects.push_back(pGameObject);
				}
			}
		}
		// Close the file stream
		levelInfo.close();
	}
	else
		// Error
		sf::err() << "Error: Unable to open level file." << "\n";

	// Mark the finish zone
	const int kiTileSize = this->getTileSize();
	const sf::Vector2i kiDimensions = this->getDimensions();
	m_fFinishZone.height = static_cast<float>(kiTileSize) * 4;
	m_fFinishZone.width = static_cast<float>(kiTileSize) * kiDimensions.x;
}

// --- Functions

std::string Level::getFilePath() const
{
	return m_ksDirectory + m_ksFileName;
}

std::string Level::getName() const
{
	return m_sLevelName;
}

std::vector<std::shared_ptr<GameObject>> Level::getObjects() const
{
	return m_apObjects;
}

// --- Methods

void Level::draw(sf::RenderWindow& renderWindow)
{
	// Draw the TileMap
	this->TileMap::draw(renderWindow);

	// Draw the objects
	const sf::View& kView = renderWindow.getView();
	for (std::shared_ptr<GameObject>& pObject : m_apObjects)
		if (Collision2D::IsViewable(*pObject, kView))
			renderWindow.draw(*pObject);

	// Draw the vehicle infomartion to the screen
	if (m_pPlayer->getCurrentGear() == 0)
		m_gearText.setString("Gear: R");

	renderWindow.draw(m_gearText);
	renderWindow.draw(m_speedText);

	// Draw the lap time to the screen
	renderWindow.draw(m_timeText);

	// Draw the player
	renderWindow.draw(*m_pPlayer);
}

void Level::playAccelerationSound()
{
	// Acceleration sound
	if (m_accelerateSound.getStatus() == sf::SoundSource::Status::Stopped)
	{
		m_accelerateSound.setPlayingOffset(m_accelerateSoundOffset);
		m_accelerateSound.play();
	}
}

void Level::playBrakeSound()
{
	// Brake sound
	if (m_brakeSound.getStatus() == sf::SoundSource::Status::Stopped && fabs(m_pPlayer->getVelocity().y) > 500.f)
	{
		m_brakeSound.setPlayingOffset(m_brakeSoundOffset);
		m_brakeSound.play();
	}
}

void Level::update(const float kfDeltaTime, sf::Event& event, sf::RenderWindow& renderWindow, sf::View& view)
{
	// Update the player's properties
	const sf::Vector2f kfCurrentAcceleration = m_pPlayer->getAcceleration();

	// Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		// Check for brake
		if (kfCurrentAcceleration.y <= 0 && m_pPlayer->getCurrentGear() == D)
		{
			// Accelerate
			playAccelerationSound();
			m_pPlayer->accelerate(UP);
		}
		else
		{
			playBrakeSound();
			m_pPlayer->brake();
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		// Reverse / Brake
		if (kfCurrentAcceleration.y >= 0 && m_pPlayer->getCurrentGear() == R)
		{
			playAccelerationSound();
			m_pPlayer->accelerate(DOWN);
		}
		else
		{
			playBrakeSound();
			m_pPlayer->brake();
		}
	}
	else
		// Reset acceleration
		if (kfCurrentAcceleration.y != 0)
			m_pPlayer->setAcceleration(sf::Vector2f(0, 0));

	// Steering
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_pPlayer->steer(LEFT);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_pPlayer->steer(RIGHT);
	else
		m_pPlayer->resetSteering();

	// Gears
	if (event.type == sf::Event::KeyPressed)
	{
		if (m_bKeyDown == false)
		{
			if (event.key.code == sf::Keyboard::LShift)
				m_pPlayer->shift(UP);
			else if (event.key.code == sf::Keyboard::RShift)
				m_pPlayer->shift(DOWN);
		}
		m_bKeyDown = true;
	}
	else if (event.type == sf::Event::KeyReleased)
		m_bKeyDown = false;

	// Update each object
	for (std::shared_ptr<GameObject>& pObject : m_apObjects)
		if (Collision2D::IsViewable(*pObject, view))
			pObject->update(kfDeltaTime);

	// Update the player
	m_pPlayer->update(kfDeltaTime);

	// Update the view
	view.setCenter(m_pPlayer->getPosition());
	renderWindow.setView(view);

	// Get the current gear as a string
	const std::string ksCurrentGear = (m_pPlayer->getCurrentGear() == 1) ? "D" : "R";

	// Update the vehicle information
	m_gearText.setString("Gear: " + ksCurrentGear);
	std::string sSpeed = std::to_string(static_cast<int>(m_pPlayer->getSpeed()) / 10);
	sSpeed.resize(3);
	m_speedText.setString(sSpeed + " MPH");

	// Update the timer text
	std::string sTime = std::to_string(m_timer.getElapsedTime().asSeconds());
	if (std::stof(sTime) < 999.f)
		sTime.resize(3);
	m_timeText.setString("Time: " + sTime + "s");

	// Store previously checked collisions
	std::vector<std::pair<std::shared_ptr<GameObject>, std::shared_ptr<GameObject>>> apCheckedObjects;

	// Check for collisions
	for (std::shared_ptr<GameObject>& pObject : m_apObjects)
	{
		// Check to see if the object is on the screen
		if (Collision2D::IsViewable(*pObject, view))
		{
			// React to collision
			CollisionInfo playerCollisionInfo = Collision2D::HaveCollided(m_pPlayer, pObject, kfDeltaTime);
			if (playerCollisionInfo.kbCollided)
			{
				if (m_crashSound.getStatus() == sf::SoundSource::Status::Stopped)
				{
					m_crashSound.setPlayingOffset(m_crashSoundOffset);
					m_crashSound.play();
				}
				Collision2D::Collide(m_pPlayer, pObject, playerCollisionInfo);
			}

			for (std::shared_ptr<GameObject>& pObject2 : m_apObjects)
			{
				// See if the check has been made before
				for (auto previousCheck : apCheckedObjects)
					if (previousCheck.first == pObject || previousCheck.first == pObject2)
						if (previousCheck.second == pObject2 || previousCheck.second == pObject)
							continue;

				// Check for a collision
				if (pObject != pObject2)
				{
					CollisionInfo objectCollisionInfo = Collision2D::HaveCollided(pObject, pObject2, kfDeltaTime);
					if (Collision2D::IsViewable(*pObject2, view) && objectCollisionInfo.kbCollided)
						Collision2D::Collide(pObject, pObject2, objectCollisionInfo);
				}

				// Add the objects to the checked list
				apCheckedObjects.push_back(std::make_pair(pObject, pObject2));
			}
		}
	}

	// Check if the player is within the Tilemap
	this->validateView(view);
	this->validateSprite(*m_pPlayer);
	renderWindow.setView(view);

	const sf::Vector2f kfCenterView = view.getCenter();
	const sf::Vector2f kfHalfViewSize = view.getSize() / 2.f;

	// Position the vehicle information
	const sf::Vector2f kfBottomRightCorner = kfCenterView + kfHalfViewSize;
	m_gearText.setPosition(kfBottomRightCorner - sf::Vector2f(135.f, 80.f));
	m_speedText.setPosition(kfBottomRightCorner - sf::Vector2f(135.f, 40.f));

	// Position the timer text
	const sf::Vector2f kfTopCenter = sf::Vector2f(kfCenterView.x, kfCenterView.y - kfHalfViewSize.y);
	const sf::FloatRect kfTimeTextBounds = m_timeText.getGlobalBounds();
	m_timeText.setPosition(kfTopCenter + sf::Vector2f(0.f - kfTimeTextBounds.width / 2.f, 20.f));

	// Check if the player has reached the finish line
	if (m_pPlayer->getGlobalBounds().intersects(m_fFinishZone))
	{
		// Get the final time
		const std::string ksFinalTime = sTime;

		// Display finish screen
		Finish finishScreen = Finish(renderWindow, m_boldFont, view);
		finishScreen.setFinalTime(ksFinalTime);

		while (finishScreen.m_bReturn == false)
			finishScreen.show();

		m_bExit = true;
	}

	// Draw the next frame
	draw(renderWindow);
}

void Level::setPlayerVehicle(const Vehicle& kVehicle)
{
	m_pPlayer = std::make_shared<Vehicle>(kVehicle);
}