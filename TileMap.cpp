#include "TileMap.h"

// --- Constructor

TileMap::TileMap(std::string sDirectory, std::string sFileName) : m_ksDirectory(sDirectory), m_ksFileName(sFileName)
{
	// Initialisation

	// Read the level file
	std::ifstream levelInfo;
	std::string sLineDelimiter = ":";
	std::string sLine;
	levelInfo.open(m_ksDirectory + m_ksFileName);

	if (levelInfo.is_open())
	{
		while (getline(levelInfo, sLine))
		{
			// Remove whitespace
			gfo::removeWhiteSpace(sLine);

			// Split the line
			std::vector<std::string> asSplit = gfo::splitString(sLine, sLineDelimiter);
			std::string sType = asSplit.at(0);
			std::string sValue = asSplit.at(1);

			// Determine the info type and set the appropriate variable
			if (sType == "Dimensions")
			{
				// Split the string
				std::vector<std::string> asDimensionsSplit = gfo::splitString(sValue, "x");
				std::string sWidth = asDimensionsSplit.at(0);
				std::string sHeight = asDimensionsSplit.at(1);
				// Convert and store the dimensions
				m_iDimensions = sf::Vector2i(std::stoi(sWidth), std::stoi(sHeight));
			}
			else if (sType == "Tile_Size")
				m_iTileSize = std::stoi(sValue);
			else if (sType == "Asset_Prefix")
				m_sAssetPrefix = sValue;
			else if (sType == "File_Type")
				m_sFileType = sValue;
			else if (sType == "Layout")
			{
				std::string sTileDelimiter = ",";
				std::vector<std::string> asTiles;

				// Split the string and store the individual values
				std::vector<std::string> asTextureSplit = gfo::splitString(sValue, sTileDelimiter);
				for (std::string& sCurrentValue : asTextureSplit)
					asTiles.push_back(sCurrentValue);

				// Create each texture
				std::vector<std::string> asTextureInfo = asTiles;
				std::sort(asTextureInfo.begin(), asTextureInfo.end());
				asTextureInfo.erase(std::unique(asTextureInfo.begin(), asTextureInfo.end()), asTextureInfo.end());

				for (std::string& sTextureName : asTextureInfo)
				{
					sf::Texture texture;
					if (!texture.loadFromFile(m_ksDirectory + m_sAssetPrefix + sTextureName + "." + m_sFileType))
						// Error
						throw std::invalid_argument("Error: Texture failed to load.");
					else
						// Create and store the texture
						m_apTextures[sTextureName] = std::make_shared<sf::Texture>(texture);
				}

				// Create and position the tiles
				std::map<std::string, std::shared_ptr<sf::Texture>>::iterator it;
				for (int y = 0; y < m_iDimensions.y; ++y)
					for (int x = 0; x < m_iDimensions.x; ++x)
					{
						// Create and position a new tile
						it = m_apTextures.find(asTiles.at(x + y * m_iDimensions.x));
						sf::Sprite tile = sf::Sprite(*it->second);
						tile.setPosition(static_cast<float>(m_iTileSize * x), static_cast<float>(m_iTileSize * y));
						m_apTiles.push_back(std::make_shared<sf::Sprite>(tile));
					}

				// Store the bounds
				sf::Vector2f fTopLeft = m_apTiles.front()->getPosition();
				sf::Vector2f fSize = sf::Vector2f(m_iDimensions * m_iTileSize);
				m_fBounds = sf::FloatRect(fTopLeft, fSize);
			}
			else
				// Error
				throw std::invalid_argument("Error: Invalid tilemap file.");
		}
		// Close the file stream
		levelInfo.close();
	}
	else
	{
		// Error
		throw std::invalid_argument("Error: Unable to open tilemap file.");
	}

	// Get the four corners of the tile map
	m_afTileMapCorners[0] = sf::Vector2f(m_fBounds.left, m_fBounds.top);
	m_afTileMapCorners[1] = sf::Vector2f(m_fBounds.left + m_fBounds.width, m_fBounds.top);
	m_afTileMapCorners[2] = sf::Vector2f(m_fBounds.left + m_fBounds.width, m_fBounds.top + m_fBounds.height);
	m_afTileMapCorners[3] = sf::Vector2f(m_fBounds.left, m_fBounds.top + m_fBounds.height);
}

// --- Functions

int TileMap::getTileSize() const
{
	return m_iTileSize;
}

std::string TileMap::getDirectory() const
{
	return m_ksDirectory;
}

std::string TileMap::getPath() const
{
	return m_ksDirectory + "/" + m_ksFileName;
}

const std::vector<std::shared_ptr<sf::Sprite>>& TileMap::getTiles() const
{
	return m_apTiles;
}

sf::FloatRect TileMap::getBounds() const
{
	return m_fBounds;
}

sf::Vector2i TileMap::getDimensions() const
{
	return m_iDimensions;
}

// --- Methods

void TileMap::draw(sf::RenderWindow& renderWindow)
{
	// Draw each tile to the render window
	for (const std::shared_ptr<sf::Sprite>& pkTile : m_apTiles)
		renderWindow.draw(*pkTile);
}

void TileMap::validateSprite(sf::Sprite& sprite) const
{	
	// Get the viewport coordinates and size
	sf::FloatRect fTileMapBounds = m_fBounds;

	// Get the sprite's texture rect
	const sf::IntRect kiTextureRect = sprite.getTextureRect();

	// Modify the viewport
	fTileMapBounds.width -= kiTextureRect.width;
	fTileMapBounds.height -= kiTextureRect.height;
	fTileMapBounds.left += kiTextureRect.width / 2;
	fTileMapBounds.top += kiTextureRect.height / 2;

	// Get the sprite's position
	sf::Vector2f fSpritePos = sprite.getPosition();

	// Get the X and Y limits
	const float kfMinX = fTileMapBounds.left;
	const float kfMinY = fTileMapBounds.top;
	const float kfMaxX = fTileMapBounds.left + fTileMapBounds.width;
	const float kfMaxY = fTileMapBounds.top + fTileMapBounds.height;

	// Correct the position
	if (fSpritePos.x < kfMinX)
		fSpritePos.x = kfMinX;
	else if (fSpritePos.x > kfMaxX)
		fSpritePos.x = kfMaxX;

	if (fSpritePos.y < kfMinY)
		fSpritePos.y = kfMinY;
	else if (fSpritePos.y > kfMaxY)
		fSpritePos.y = kfMaxY;
	
	// Set the position
	sprite.setPosition(fSpritePos);
}

void TileMap::validateView(sf::View& view)
{
	// Check if the view can be validated
	const sf::Vector2f kfViewSize = view.getSize();
	if (m_fBounds.width < kfViewSize.x && m_fBounds.height < kfViewSize.y)
		return;

	// Get the center of the tile map and view
	const sf::Vector2f kfViewCenter = view.getCenter();

	// Get the min and max values for X and Y
	const sf::Vector2f kfHalfViewSize = kfViewSize / 2.f;

	const float kfMinX = m_afTileMapCorners[0].x + kfHalfViewSize.x;
	const float kfMaxX = m_afTileMapCorners[1].x - kfHalfViewSize.x;
	const float kfMinY = m_afTileMapCorners[0].y + kfHalfViewSize.y;
	const float kfMaxY = m_afTileMapCorners[2].y - kfHalfViewSize.y;

	// Check the view center against the ranges
	sf::Vector2f fNewCenter;

	if (kfViewCenter.x < kfMinX)
		fNewCenter.x = kfMinX;
	else if (kfViewCenter.x > kfMaxX)
		fNewCenter.x = kfMaxX;
	else
		fNewCenter.x = kfViewCenter.x;

	if (kfViewCenter.y < kfMinY)
		fNewCenter.y = kfMinY;
	else if (kfViewCenter.y > kfMaxY)
		fNewCenter.y = kfMaxY;
	else
		fNewCenter.y = kfViewCenter.y;

	// Set the new view center
	view.setCenter(fNewCenter);
}