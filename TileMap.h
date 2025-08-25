#ifndef TILEMAP_H
#define TILEMAP_H

/*!
\file TileMap.h
*/

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <string>
#include <vector>
#include "GameFileOperations.h"

/*!
\class TileMap
\brief Used to store and draw tilemaps.
*/
class TileMap
{
public:
	// --- Constructor
	//! Constructor
	TileMap(std::string sDirectory, std::string sFileName);
	// --- Functions
	int getTileSize() const;															//!< Returns the size of the tile in pixels.
	std::string getDirectory() const;													//!< Returns the tilemap's directory.
	std::string getPath() const;														//!< Returns the path to the tilemap's file.
	const std::vector<std::shared_ptr<sf::Sprite>>& getTiles() const;					//!< Returns pointers to the tiles used within the tilemap.
	sf::FloatRect getBounds() const;													//!< Returns the bounds of the tilemap.
	sf::Vector2i getDimensions() const;												//!< Returns the dimensions of the tilemap in tiles.
	// --- Methods
	void draw(sf::RenderWindow& renderWindow);									//!< Draws the tilemap to the screen.
	void validateSprite(sf::Sprite& sprite) const;									//!< Keeps a sprite within the boundries of the tilemap.
	void validateView(sf::View& view);											//!< Kepps the view wihin the boundries of the tilemap.
protected:
	// --- Variables
	const std::string m_ksDirectory;											//!< Stores the tilemap's directory.
private:
	// --- Variables
	int m_iTileSize;															//!< Stores the size of each tile in pixels.
	std::string m_sAssetPrefix;													//!< Stores the prefix of each tile's filename.
	const std::string m_ksFileName;												//!< Stores the filename of the tilemap.
	std::string m_sFileType;													//!< Stores the type of file each tile is.
	std::array<sf::Vector2f, 4> m_afTileMapCorners;								//!< Stores the four corners of the tilemap in the window's global space.
	std::map<std::string, std::shared_ptr<sf::Texture>> m_apTextures;			//!< Stores pointers to the textures of the tiles used within the tilemap.
	std::vector<std::shared_ptr<sf::Sprite>> m_apTiles;							//!< Stores pointers to the tiles used within the tilemap.
	sf::FloatRect m_fBounds;													//!< Stores the bounds of the tilemap in the window's global space.
	sf::Vector2i m_iDimensions;													//!< Stores the dimensions of the tilemap in tiles.
};

#endif