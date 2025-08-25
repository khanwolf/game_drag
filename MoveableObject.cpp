#include "MoveableObject.h"

// --- Constructor(s)

MoveableObject::MoveableObject(const float kfMass, ColliderType colliderType, const sf::Texture& kTexture) : GameObject(false, true, colliderType, kTexture), m_kbKnockable(false), m_kfMass(fabsf(kfMass)), m_kfInvMass((m_kfMass != 0) ? 1 / m_kfMass : 0.f)
{
	// Initialisation
	initialise();
}

MoveableObject::MoveableObject(const float kfMass, ColliderType colliderType, sf::Texture& texture, std::shared_ptr<sf::Texture> pFallenTexture) : GameObject(false, true, colliderType, texture), m_kbKnockable(isKnockable()), m_kfMass(fabsf(kfMass)), m_kfInvMass((m_kfMass != 0) ? 1 / m_kfMass : 0.f)
{
	// Initialisation
	initialise();
	m_pFallenTexture = pFallenTexture;
}

// --- Functions

bool MoveableObject::isKnockable() const
{
	return m_pFallenTexture != nullptr;
}

bool MoveableObject::isStatic()
{
	// MoveableObject's are never static
	return false;
}

bool MoveableObject::hasFallen() const
{
	return m_bFallen;
}

float MoveableObject::getInvMass() const
{
	return m_kfInvMass;
}

float MoveableObject::getMass() const
{
	return m_kfMass;
}

float MoveableObject::getSpeed() const
{
	return vm2d::Magnitude(this->getVelocity());
}

sf::Time MoveableObject::getDeltaTime() const
{
	return m_kDeltaTime;
}

sf::Vector2f MoveableObject::getAcceleration() const
{
	return m_fAcceleration;
}

sf::Vector2f MoveableObject::getDragCoefficient() const
{
	return m_fDragCoefficient;
}

sf::Vector2f MoveableObject::getVelocity() const
{
	return m_fVelocity;
}

// --- Methods

void MoveableObject::accelerate(const sf::Vector2f& kfAcceleration)
{
	m_fAcceleration += kfAcceleration;
	m_bAccelerating = true;
}

void MoveableObject::fall()
{
	// Check if the object is knockable
	if (m_kbKnockable == true)
	{
		m_bFallen = true;
		// Change the texture
		if (m_pFallenTexture != nullptr)
			this->setTexture(*m_pFallenTexture);
	}
}

void MoveableObject::initialise()
{
	m_bAccelerating = true;
	m_bFallen = false;
	m_fDragCoefficient = sf::Vector2f(0.5f, 0.5f);
}

void MoveableObject::setAcceleration(const sf::Vector2f& kfAcceleration)
{
	m_fAcceleration = kfAcceleration;
}

void MoveableObject::setDragCoefficient(const sf::Vector2f& kfDrag)
{
	m_fDragCoefficient = sf::Vector2f(fabs(kfDrag.x), fabs(kfDrag.y));
}

void MoveableObject::setVelocity(const sf::Vector2f& kfVelocity)
{
	m_fVelocity = kfVelocity;
}

void MoveableObject::update(const float kfDeltaTime)
{
	// Update the position of the object
	sf::Vector2f fNewPosition = this->getPosition() + (kfDeltaTime * m_fVelocity);

	// Check if acceleration needs to be reset
	if (m_bAccelerating)
		m_bAccelerating = false;
	else
		m_fAcceleration = sf::Vector2f(0.f, 0.f);

	// Calculate the new velocity for both axis
	m_fVelocity = m_fVelocity + (m_kDeltaTime.asSeconds() * m_fAcceleration);

	// Calculate the drag
	const sf::Vector2f kfDrag = m_fDragCoefficient * m_kfMass * getSpeed() * kfDeltaTime;

	// Update the velocity
	if (m_fVelocity.x > 0.f)
		(m_fVelocity.x - kfDrag.x > 0.f) ? m_fVelocity.x -= kfDrag.x : m_fVelocity.x = 0;
	else if (m_fVelocity.y < 0.f)
		(m_fVelocity.x + kfDrag.x < 0.f) ? m_fVelocity.x += kfDrag.x : m_fVelocity.x = 0;

	if (m_fVelocity.y > 0.f)
		(m_fVelocity.y - kfDrag.y > 0.f) ? m_fVelocity.y -= kfDrag.y : m_fVelocity.y = 0;
	else if (m_fVelocity.y < 0.f)
		(m_fVelocity.y + kfDrag.y < 0.f) ? m_fVelocity.y += kfDrag.y : m_fVelocity.y = 0;

	// Set the new position for the object
	this->setPosition(fNewPosition);
}