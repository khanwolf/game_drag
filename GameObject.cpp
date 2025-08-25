 #include "GameObject.h"

// --- Constructor

GameObject::GameObject(const bool bCollectable, const bool kbSolid, ColliderType colliderType, const sf::Texture& kTexture) : sf::Sprite(kTexture), m_kbCollectible(bCollectable), m_kColliderType(colliderType)
{
	// Initialisation
	setSolidity(kbSolid);
	m_fBounciness = 0.5f;
}

// --- Functions

bool GameObject::isCollectible() const
{ 
	return m_kbCollectible;
}

bool GameObject::isSolid() const
{ 
	return m_bSolid;
}

bool GameObject::isStatic() const
{
	// GameObject's are static by default
	return true;
}

float GameObject::getBounciness() const
{
	return m_fBounciness;
}

ColliderType GameObject::getColliderType() const
{
	return m_kColliderType;
}

// --- Methods

void GameObject::setBounciness(float fBounciness)
{
	// Ensure the value is between 0 and 1
	if (fabs(fBounciness) > 1)
		fBounciness = 1.f;
	
	m_fBounciness = fBounciness;
}

void GameObject::setSolidity(const bool kbSolid)
{
	 // Check if the object is a collectible
	m_bSolid = (m_kbCollectible) ? false : kbSolid;
}

void GameObject::update(const float kfDeltaTime) {}