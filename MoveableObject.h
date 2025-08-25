#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

/*!
\file MoveableObject.h
*/

#include <memory>
#include "GameObject.h"

/*!
\enum Direction
\brief Used to refer to a direction on a 2D plane.
*/
enum Direction { UP, DOWN, LEFT, RIGHT };

/*!
\class MoveableObject
\brief Extends upon GameObject to give sprites physics properties.
*/
class MoveableObject : public GameObject
{
public:
	// --- Constructor(s)
	//! Default constructor
	MoveableObject(const float kfMass, ColliderType colliderType, const sf::Texture& kTexture);
	//! Overridden constructor, used for objects that are capable of falling.
	MoveableObject(const float kfMass, ColliderType colliderType, sf::Texture& texture, std::shared_ptr<sf::Texture> pFallenTexture);
	// --- Functions
	bool isKnockable() const;														//!< Returns whether or not the object is knockable.
	static bool isStatic();														//!< Returns whether or not the object is static.
	bool hasFallen() const;														//!< Returns whether or not the object has fallen.
	float getInvMass() const;														//!< Returns the object's inverted mass.
	float getMass() const;														//!< Returns the object's mass.
	float getSpeed() const;														//!< Returns the object's current speed.
	sf::Time getDeltaTime() const;												//!< Returns delta time in seconds.
	sf::Vector2f getAcceleration() const;											//!< Returns the object's current acceleration.
	sf::Vector2f getDragCoefficient() const;										//!< Returns the object's drag coefficent.
	sf::Vector2f getVelocity() const;												//!< Returns the object's current velocity.
	// --- Methods
	virtual void accelerate(const sf::Vector2f& kfAcceleration);			//!< Used to accelerate the object.
	void fall();															//!< Marks the object as having fallen.
	void setAcceleration(const sf::Vector2f& kfAcceleration);				//!< Used to set the acceleration of the object.
	void setDragCoefficient(const sf::Vector2f& kfDrag);					//!< Used to set the drag coefficent of the object.
	void setVelocity(const sf::Vector2f& kfVelocity);						//!< Used to set the velocity of the object.
	void update(const float kfDeltaTime) override;							//!< Used to update the object.
	// --- Variables
	const sf::Time m_kDeltaTime;											//!< Stores delta time in seconds.
private:
	// --- Methods
	void initialise();														//!< Used to initialise the object.
	// --- Variables
	bool m_bAccelerating;													//!< Stores whether or not the object is currently being accelerated.
	bool m_bFallen;															//!< Stores whether or not the object has fallen.
	const bool  m_kbKnockable;												//!< Stores whether or not the object can be knocked over.
	const float m_kfMass;													//!< Stores the object's mass.
	const float m_kfInvMass;												//!< Stores the objects inverted mass.
	std::shared_ptr<sf::Texture> m_pFallenTexture;							//!< Stores the texture of the object when it has fallen.
	sf::Vector2f m_fAcceleration;											//!< Stores the object's current acceleration.
	sf::Vector2f m_fDragCoefficient;										//!< Stores the object's drag coefficient.
	sf::Vector2f m_fVelocity;												//!< Stores the object's current velocity.
};

#endif