#ifndef COLLISION2D_H
#define COLLISION2D_H

/*!
\file	Collision2D.h
*/

/*! 
\enum	ColliderType
\brief	Used to state the type of collider a GameObject has. 
*/
enum ColliderType { CIRCLE, RECTANGLE };

#include <iostream>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <vector>
#include "VectorMaths2D.h"

class GameObject;
class MoveableObject;

/*! 
\struct CollisionInfo
\brief	Stores collision information, e.g. (Penetration depth and the Collision normal). 
*/
struct CollisionInfo {
	const bool kbCollided;																																									 //!< Used to determine whether or not a collision has occured.
	const float kfPenetration;																																								 //!< Stores the penetration depth of the collision.
	const sf::Vector2f kfCollisionNormal;																																					 //!< Stores the normal of the collision.
	//! Default constructor, used when no collision has occured.
	CollisionInfo() : kbCollided(false), kfPenetration(0.f), kfCollisionNormal(sf::Vector2f(0, 0)) {};																	
	//! Overloaded constructor, can be used to store collision information.
	CollisionInfo(bool bCollided, float fPenetration, sf::Vector2f& fCollisionNormal) : kbCollided(bCollided), kfPenetration(fPenetration), kfCollisionNormal(fCollisionNormal) {}
};

/*!
\class		Collision2D
\brief		Used to detect and resolve collisions between Sprites or GameObjects.
\details	<ul><li>Reference: <a href="http://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331">tutsplus</a></li><li>Reference: <a href="http://www.dyn4j.org/2010/01/sat/">dyn4j</a></li></ul>
*/
class Collision2D
{
public:
	// --- Functions
	static bool IsViewable(const sf::Sprite& kSprite, const sf::View& kView);																												 //!< Returns a boolean stating whether or not a sprite is within the bounds of the view.
	static CollisionInfo AACollisionTest(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, const float kfDeltaTime);												 //!< Used to detect a collision between axis aligned rectangles and circles.
	static CollisionInfo CircleCollisionTest(const sf::Sprite& kSprite1, const sf::Sprite& kSprite2);																						 //!< Used to detect a collision between two circles.
	static CollisionInfo HaveCollided(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, const float kfDeltaTime);													 //!< Used to detect a collision between two GameObjects.
	static CollisionInfo OBBTest(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, const float kfDeltaTime);														 //!< Used to detect a collision between an oriented bounding box and a circle.
	static CollisionInfo SATCollisionBBTest(const sf::Sprite& kSprite1, const sf::Sprite& kSprite2);																						 //!< Used to detect a collision between any two sprites.
	// --- Methods
	static void Collide(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, CollisionInfo& collisionInfo);															 //!< Used to resolve collisions.
private:
	// --- Functions
	static float GetSpriteRadius(const sf::Sprite& kSprite);																																 //!< Returns the radius of a sprite.
	static std::vector<sf::Vector2f> GetAxes(const sf::Sprite& kSprite, const std::vector<sf::Vector2f>& akfSpriteVertices);																 //!< Returns the axes of a sprite; this function is used in the SAT collision test.
	static std::vector<sf::Vector2f> GetBoundingBoxVertices(const sf::Sprite& kSprite);																										 //!< Returns the vertices of a sprite's bounding box.
	static sf::Vector2f GetSpriteCenter(const sf::Sprite& kSprite);																															 //!< Returns a vertex that contains the location of a sprite's center.
	static sf::Vector2f GetSpriteSize(const sf::Sprite& kSprite);																															 //!< Returns the dimensions of a sprite with scaling taken into account.
	// --- Methods
	static void PositionalCorrection(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, CollisionInfo& collisionInfo);												 //!< Corrects the positions of sprites when resolving collisions.
	static void Project(const std::vector<sf::Vector2f>& akfAxes, const sf::Vector2f& fAxis, float& fMin, float& fMax);																		 //!< Projects points onto axes; this function is used in the SAT collision test.
};

#endif