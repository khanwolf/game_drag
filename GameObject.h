#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
/*!
\file GameObject.h
*/

#include <SFML/Graphics.hpp>
#include "Collision2D.h"

/*!
\class GameObject
\brief Extends the functionality of sf::Sprite
\details Provides bounciness, solidity and collidable properties to sprites.
*/
class GameObject : public sf::Sprite
{
public:
	// --- Constructor
	//! Constructor
	GameObject(const bool kbCollectable, const bool kbSolid, ColliderType colliderType, const sf::Texture& kTexture);
	// --- Functions
	bool isCollectible() const;																									//!< Returns whether or not the object can be collected.
	bool isSolid() const;																											//!< Returns whether or not the object is solid.
	virtual bool isStatic() const;																								//!< Returns whether or not the object is static.
	float getBounciness() const;																									//!< Returns the bounciness of the object.
	ColliderType getColliderType() const;																							//!< Returns the ColliderType of the object.
	// --- Methods
	void setBounciness(float fBounciness);																					//!< Used to set the bounciess of the object.
	void setSolidity(const bool kbSolid);																					//!< Used to set the solidity property of the object.
	virtual void update(const float kfDeltaTime);																			//!< Used to update the object.
private:
	// --- Variables
	bool m_bSolid;																											//!< Stores whether or not the object is solid.
	const bool m_kbCollectible;																								//!< Stores whether or not the object is collectible.
	float m_fBounciness;																									//!< Stores the bounciness of the object (A value between 0(No energy loss) and 1(Complete energy loss)).
	const ColliderType m_kColliderType;																						//!< Stores the object's ColliderType.
};

#endif