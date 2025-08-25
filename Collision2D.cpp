#include "Collision2D.h"
#include "MoveableObject.h"

// --- Methods

void Collision2D::Collide(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, CollisionInfo& collisionInfo)
{
	// Respond to a collision between two objects 

	// Check if both objects are solids
	if (pObject1->isSolid() == false || pObject2->isSolid() == false)
		return;

	// Check which objects are moving
	std::shared_ptr<MoveableObject> pMovingObject1, pMovingObject2;

	if (pObject1->isStatic() && pObject2->isStatic())
		return;

	pMovingObject1 = (pObject1->isStatic() || pObject2->isStatic()) ? std::static_pointer_cast<MoveableObject>(pObject2) : std::static_pointer_cast<MoveableObject>(pObject1);
	pMovingObject2 = (pMovingObject1 != pObject2) ? std::static_pointer_cast<MoveableObject>(pObject2) : nullptr;

	// Calculate the resultant velocity
	if (pMovingObject2 == nullptr)
	{
		// A moving object colliding with an immoveable object

		// Check if pMovingObject1 is knockable
		if (pMovingObject1->isKnockable())
			pMovingObject1->fall();

		// Get the properties of the moving object
		sf::Vector2f fVelocity = pMovingObject1->getVelocity();
		const float fVNDot = vm2d::Dot(fVelocity, collisionInfo.kfCollisionNormal);
		const float kfRestitution = std::min(pMovingObject1->getBounciness(), pObject2->getBounciness());

		// Calculate and set the new velocity
		fVelocity -= (1 + kfRestitution) * collisionInfo.kfCollisionNormal * fVNDot;
		pMovingObject1->setVelocity(fVelocity);
	}
	else
	{
		// Two moving objects colliding

		// Check if pMovingObject1 or pMovingObject2 is knockable
		if (pMovingObject1->isKnockable())
			pMovingObject1->fall();
		if (pMovingObject2->isKnockable())
			pMovingObject2->fall();

		// Get the velocities of the objects
		sf::Vector2f fVelocity1 = pMovingObject1->getVelocity();
		sf::Vector2f fVelocity2 = pMovingObject2->getVelocity();

		// Caluclate the normal
		const float kfVelNormal = vm2d::Dot(fVelocity2 - fVelocity1, collisionInfo.kfCollisionNormal);
		
		// Don't resolve the collision if the velocities are already seperating the two objects
		if (kfVelNormal > 0) return;

		// get the restitution
		const float kfRestitution = std::min(pMovingObject1->getBounciness(), pObject2->getBounciness());

		// Get the inverse mass's of both objects
		const float kfInvMass1 = pMovingObject1->getInvMass();
		const float kfInvMass2 = pMovingObject2->getInvMass();

		// Calculate impulse scalar
		const float kfJ = (-(1 + kfRestitution) * kfVelNormal) / (kfInvMass1 + kfInvMass2);

		// Calculate the collision impulse
		const sf::Vector2f kfImpulse = (kfJ * collisionInfo.kfCollisionNormal);
		// Apply the collision impulse
		pMovingObject1->setVelocity(fVelocity1 - (kfInvMass1 * kfImpulse));
		pMovingObject2->setVelocity(fVelocity2 + (kfInvMass2 * kfImpulse));
	}
}

void Collision2D::PositionalCorrection(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, CollisionInfo& collisionInfo)
{
	// Check which objects are moving
	std::shared_ptr<MoveableObject> pMovingObject1, pMovingObject2;

	if (pObject1->isStatic() && pObject2->isStatic())
		return;

	pMovingObject1 = (pObject1->isStatic() || pObject2->isStatic()) ? std::static_pointer_cast<MoveableObject>(pObject2) : std::static_pointer_cast<MoveableObject>(pObject1);
	pMovingObject2 = (pMovingObject1 != pObject2) ? std::static_pointer_cast<MoveableObject>(pObject2) : nullptr;

	const float kfInvMass1 = pMovingObject1->getInvMass();
	const float kfInvMass2 = (pMovingObject2 != nullptr) ? pMovingObject2->getInvMass() : 0.f;

	// Calculate the correction
	const float kfPercent = 0.8f; // Usually between 20% - 80%
	const sf::Vector2f kfCorrection = collisionInfo.kfPenetration / (kfInvMass1 + kfInvMass2) * kfPercent * collisionInfo.kfCollisionNormal;
	// Apply the correction
	pObject1->setPosition(pObject1->getPosition() - kfInvMass1 * kfCorrection);
	pObject2->setPosition(pObject2->getPosition() + kfInvMass2 * kfCorrection);
}

void Collision2D::Project(const std::vector<sf::Vector2f>& akfVertices, const sf::Vector2f& fAxis, float& fMin, float& fMax)
{
	// Calculate the min and max values
	fMin = vm2d::Dot(akfVertices.at(0), fAxis);
	fMax = fMin;

	// Project
	for (const sf::Vector2f& Vertex : akfVertices)
	{
		const float kfProjection = vm2d::Dot(Vertex, fAxis);

		if (kfProjection < fMin)
			fMin = kfProjection;
		else if (kfProjection > fMax)
			fMax = kfProjection;
	}
}

// --- Functions

bool Collision2D::IsViewable(const sf::Sprite& kSprite, const sf::View& kView)
{
	// View properties
	const sf::Vector2f kfHalfSize = kView.getSize() / 2.f;
	const sf::Vector2f kfCenter = kView.getCenter();
	const sf::Vector2f kfTopLeftBounds = kfCenter - kfHalfSize;
	const sf::Vector2f kfBottomRightBounds = kfCenter + kfHalfSize;

	// Get the sprite's bounding box vertices
	const std::vector<sf::Vector2f> kfVertices = GetBoundingBoxVertices(kSprite);

	// Check to see if any of the vertices are within the bounds of the view
	for (const sf::Vector2f& kfVertex : kfVertices)
	{
		if (kfVertex.x > kfTopLeftBounds.x && kfVertex.x < kfBottomRightBounds.x && kfVertex.y > kfTopLeftBounds.y && kfVertex.y < kfBottomRightBounds.y)
			// Vertex visible
			return true;
	}

	return false;
}

CollisionInfo Collision2D::AACollisionTest(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, const float kfDeltaTime)
{
	ColliderType GOColliderType1 = pObject1->getColliderType();
	ColliderType GOColliderType2 = pObject2->getColliderType();

	if (GOColliderType1 == RECTANGLE && GOColliderType2 == RECTANGLE)
	{
		// Get the bounding box's of both objects
		sf::FloatRect kfBoundingBox1 = pObject1->getGlobalBounds();
		sf::FloatRect kfBoundingBox2 = pObject2->getGlobalBounds();

		// Check for a collision
		bool bCollided = kfBoundingBox1.intersects(kfBoundingBox2);

		if (bCollided == false)
			// No collision
			return CollisionInfo();

		// Collision detected

		// Get the moving objects
		std::shared_ptr<MoveableObject> pMovingObject1 = (pObject1->isStatic()) ? nullptr : std::static_pointer_cast<MoveableObject>(pObject1);
		std::shared_ptr<MoveableObject> pMovingObject2 = (pObject2->isStatic()) ? nullptr : std::static_pointer_cast<MoveableObject>(pObject2);

		// Get the velocities of both objects
		const sf::Vector2f kfObject1Velocity = (pMovingObject1 == nullptr) ? sf::Vector2f(0, 0) : pMovingObject1->getVelocity();
		const sf::Vector2f kfObject2Velocity = (pMovingObject2 == nullptr) ? sf::Vector2f(0, 0) : pMovingObject2->getVelocity();

		// Find the collision normal
		sf::Vector2f fCollisionNormal = (kfObject1Velocity - kfObject2Velocity) * kfDeltaTime;

		// Return the collision info
		return CollisionInfo(bCollided, vm2d::Magnitude(fCollisionNormal), fCollisionNormal);
	}
	else
	{
		// Check the collider type
		std::shared_ptr<GameObject> pBoundingBox, pCircle;
		pBoundingBox = (GOColliderType1 == RECTANGLE) ? pObject1 : pObject2;
		pCircle = (pBoundingBox == pObject1) ? pObject2 : pObject1;

		// Find the vector distance between the centers of the two shapes
		const sf::Vector2f kfCenterDistance = pCircle->getPosition() - pBoundingBox->getPosition();
		
		// Clamp the distance
		sf::Vector2f fClamp;
		const sf::Vector2f kfExtents = GetSpriteSize(*pBoundingBox) / 2.f;

		if (kfCenterDistance.x >= 0)
			fClamp.x = (kfCenterDistance.x > kfExtents.x) ? kfExtents.x : kfCenterDistance.x;
		else
			fClamp.x = (kfCenterDistance.x > -kfExtents.x) ? kfCenterDistance.x : -kfExtents.x;

		if (kfCenterDistance.y >= 0)
			fClamp.y = (kfCenterDistance.y > kfExtents.y) ? kfExtents.y : kfCenterDistance.y;
		else
			fClamp.y = (kfCenterDistance.y > -kfExtents.y) ? kfCenterDistance.y : -kfExtents.y;

		// Get the difference
		const sf::Vector2f kfDifference = kfCenterDistance - fClamp;
	
		// Find the distance
		float fDistance = vm2d::Magnitude(kfDifference) - GetSpriteRadius(*pCircle);

		// Return the info
		return CollisionInfo(fDistance < 0.f, fDistance, vm2d::Normalise(fClamp));
	}
}

CollisionInfo Collision2D::CircleCollisionTest(const sf::Sprite& kSprite1, const sf::Sprite& kSprite2)
{
	// Check if the magnitude between the Centers of the two circles is less than the sum of the radii, if so a collision has occured.
	const sf::Vector2f kfCenterSubtraction = GetSpriteCenter(kSprite2) - GetSpriteCenter(kSprite1);
	float radiiSumSquared = GetSpriteRadius(kSprite1) + GetSpriteRadius(kSprite2);
	radiiSumSquared *= radiiSumSquared;

	if ((kfCenterSubtraction.x * kfCenterSubtraction.x) + (kfCenterSubtraction.y * kfCenterSubtraction.y) > radiiSumSquared)
		// No collision
		return CollisionInfo();

	// Collision
	const float kfMagnitude = vm2d::Magnitude(kfCenterSubtraction);

	if (kfMagnitude != 0)
		return CollisionInfo(true, radiiSumSquared - kfMagnitude, kfCenterSubtraction / kfMagnitude);

	// Circles are at the same position
	return CollisionInfo(true, GetSpriteRadius(kSprite1), sf::Vector2f(1, 0));
}

CollisionInfo Collision2D::HaveCollided(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, const float kfDeltaTime)
{
	// Perform the correct test and return the result
	const ColliderType kColliderType1 = pObject1->getColliderType();
	const ColliderType kColliderType2 = pObject2->getColliderType();

	const float kfObjectRotation1 = pObject1->getRotation();
	const float kfObjectRotation2 = pObject2->getRotation();

	if (kColliderType1 == CIRCLE && kColliderType2 == CIRCLE)
		return CircleCollisionTest(*pObject1, *pObject2);
	else if (kColliderType1 == RECTANGLE && kColliderType2 == RECTANGLE)
		return ((kfObjectRotation1 == 0 && kfObjectRotation2 == 0)) ? AACollisionTest(pObject1, pObject2, kfDeltaTime) : SATCollisionBBTest(*pObject1, *pObject2);
	else
		return OBBTest(pObject1, pObject2, kfDeltaTime);
}

CollisionInfo Collision2D::OBBTest(std::shared_ptr<GameObject> pObject1, std::shared_ptr<GameObject> pObject2, const float kfDeltaTime)
{
	// Get the collider shapes of the first object
	ColliderType GOColliderType1 = pObject1->getColliderType();

	std::shared_ptr<GameObject> pBoundingBox, pCircle;

	pBoundingBox = (GOColliderType1 == CIRCLE) ? pObject2 : pObject1;
	pCircle = (pBoundingBox == pObject2) ? pObject1 : pObject2;

	// Store the original values
	const float kfOriginalBBRotation = pBoundingBox->getRotation();
	const sf::Vector2f kfOriginalBBPosition = pBoundingBox->getPosition();
	const sf::Vector2f kfOriginalCirclePosition = pCircle->getPosition();
	
	// Translate the objects
	pBoundingBox->setPosition(0, 0);
	pCircle->setPosition(kfOriginalCirclePosition - kfOriginalBBPosition);
	// Rotate the objects
	pBoundingBox->setRotation(0);
	pCircle->setPosition(vm2d::Rotate(pCircle->getPosition(), -kfOriginalBBRotation));

	// Check for a collision
	const CollisionInfo kCollisionInfo = AACollisionTest(pObject1, pObject2, kfDeltaTime);

	// Reset the object positions and rotations
	pBoundingBox->setRotation(kfOriginalBBRotation);
	pBoundingBox->setPosition(kfOriginalBBPosition);
	pCircle->setPosition(kfOriginalCirclePosition);

	// Return the collision info
	return kCollisionInfo;
}

CollisionInfo Collision2D::SATCollisionBBTest(const sf::Sprite& kSprite1, const sf::Sprite& kSprite2)
{
	// Get the vertices for both sprites
	const std::vector<sf::Vector2f>& akfSprite1Vertices = GetBoundingBoxVertices(kSprite1);
	const std::vector<sf::Vector2f>& akfSprite2Vertices = GetBoundingBoxVertices(kSprite2);
	// Get the axes for both bounding boxes
	std::vector<std::vector<sf::Vector2f>> afSpriteAxes;
	afSpriteAxes.push_back(GetAxes(kSprite1, akfSprite1Vertices));
	afSpriteAxes.push_back(GetAxes(kSprite2, akfSprite2Vertices));

	sf::Vector2f fMinAxis;
	float fMinOverlap = 0;
	bool bFirstRun = true;
	
	// Check each axis for both shapes
	for (const std::vector<sf::Vector2f>& akfAxes : afSpriteAxes)
	{
		for (const sf::Vector2f& kfAxis : akfAxes)
		{
			float fMax1, fMax2, fMin1, fMin2;
			// Project the points of both bounding boxes onto the axis
			Project(akfSprite1Vertices, kfAxis, fMin1, fMax1);
			Project(akfSprite2Vertices, kfAxis, fMin2, fMax2);
			// Check for overlap
			if (!((fMin2 <= fMax1) && (fMax2 >= fMin1)))
				return CollisionInfo();

			const float kfOverlap = std::min(fMax1, fMax2) - std::max(fMin1, fMin2);

			if (bFirstRun)
			{
				fMinOverlap = kfOverlap;
				fMinAxis = kfAxis;
				bFirstRun = false;
			}
			else if (kfOverlap < fMinOverlap)
			{
				fMinOverlap = kfOverlap;
				fMinAxis = kfAxis;
			}
		}
	}
	
	// Collision
	return CollisionInfo(true, fMinOverlap, vm2d::Normalise(fMinAxis));
}

float Collision2D::GetSpriteRadius(const sf::Sprite& kSprite)
{
	const sf::Vector2f kfSpriteSize = GetSpriteSize(kSprite);
	return (kfSpriteSize.x + kfSpriteSize.y) / 4;
}

std::vector<sf::Vector2f> Collision2D::GetAxes(const sf::Sprite& kSprite, const std::vector<sf::Vector2f>& akfSpriteVertices)
{
	// Iterate through the vector and add the axes
	std::vector<sf::Vector2f>::iterator it;
	std::vector<sf::Vector2f> afAxes;

	for (auto& it = akfSpriteVertices.begin(); it != akfSpriteVertices.end(); ++it)
	{
		// Get the edge
		sf::Vector2f fEdge = (std::next(it) == akfSpriteVertices.end()) ? *it - akfSpriteVertices.front() : *it - *std::next(it);
		// Get the normal
		fEdge = sf::Vector2f(-fEdge.y, fEdge.x);
		// Add the axis
		afAxes.push_back(fEdge);
	}

	return afAxes;
}

std::vector<sf::Vector2f> Collision2D::GetBoundingBoxVertices(const sf::Sprite& kSprite)
{
	std::vector<sf::Vector2f> afSpriteVertices;
	sf::Transform transform = kSprite.getTransform();
	sf::FloatRect fSpriteBounds = kSprite.getGlobalBounds();

	// Store and return the vertices
	afSpriteVertices.push_back(transform.transformPoint(0.f, 0.f));
	afSpriteVertices.push_back(transform.transformPoint(fSpriteBounds.width, 0.f));
	afSpriteVertices.push_back(transform.transformPoint(fSpriteBounds.width, fSpriteBounds.height));
	afSpriteVertices.push_back(transform.transformPoint(0.f, fSpriteBounds.height));

	return afSpriteVertices;
}

sf::Vector2f Collision2D::GetSpriteCenter(const sf::Sprite& kSprite)
{
	sf::FloatRect fAABB = kSprite.getGlobalBounds();
	return sf::Vector2f(fAABB.left + fAABB.width / 2.0f, fAABB.top + fAABB.height / 2.0f);
}

sf::Vector2f Collision2D::GetSpriteSize(const sf::Sprite& kSprite)
{
	const sf::IntRect kiOriginalSpriteSize = kSprite.getTextureRect();
	const sf::Vector2f kfScale = kSprite.getScale();
	return sf::Vector2f(kiOriginalSpriteSize.width * kfScale.x, kiOriginalSpriteSize.height * kfScale.y);
}