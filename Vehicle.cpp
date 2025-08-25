#include "Vehicle.h"

// --- Constructor

Vehicle::Vehicle(const sf::Vector2f& kfDragCoefficient, const float kfBrakePower, const float kfMass, VehicleType type) : MoveableObject(kfMass, RECTANGLE, sf::Texture()), m_type(type)
{
	// Initialisation

	// Set the drag
	this->setDragCoefficient(kfDragCoefficient);

	// Get the texture
	std::string sTexturePath = "../Assets/Vehicles/";
	const std::string ksFileExtension = "PNG";

	switch (type)
	{
	case CAR:
		sTexturePath = sTexturePath + "car" + "." + ksFileExtension;
		break;
	case MOTORCYCLE:
		sTexturePath = sTexturePath + "motorcycle" + "." + ksFileExtension;
		break;
	case TRUCK:
		sTexturePath = sTexturePath + "truck" + "." + ksFileExtension;
		break;
	default:
		// Error
		throw std::invalid_argument("Error: Unidentified vehicle type.");
	};

	// Set the texture
	sf::Texture texture;
	if (!(texture.loadFromFile(sTexturePath)))
		// Error
		throw std::invalid_argument("Error: Vehicle texture failed to load.");
	else
		this->setTexture(*new sf::Texture(texture), true);

	// Set the vehicle's properties
	s_vProperties.accelerationIncrement = 25.f;
	s_vProperties.brakePower = kfBrakePower;
	s_vProperties.maxAcceleration = 250.f;
	s_vProperties.maxSteeringAngle = 25.f;
	s_vProperties.steeringIncrement = 0.5f;
	s_vProperties.maxRPM = 8000;
	s_vProperties.wheelBase = static_cast<float>(texture.getSize().y) * 0.6f;

	m_currentGear = D;
	m_fSteeringAngle = 0.f;
	m_uiFuel = 100;

	// Set the vehicle's origin to its Center
	this->setOrigin(static_cast<sf::Vector2f>(this->getTexture()->getSize()) / 2.f);
}

// --- Functions

float Vehicle::getSteeringAngle() const
{
	return m_fSteeringAngle;
}

unsigned int Vehicle::getDamage() const
{
	return m_uiDamage;
}

unsigned int Vehicle::getFuelLevel() const
{
	return m_uiFuel;
}

unsigned int Vehicle::getRPM() const
{
	return m_uiRPM;
}

AutoGearBox Vehicle::getCurrentGear() const
{
	return m_currentGear;
}

VehicleType Vehicle::getType() const
{
	return m_type;
}

// --- Methods

void Vehicle::accelerate(Direction direction)
{
	// Check if the vehicle is rolling in the opposite direction
	const sf::Vector2f kfCurrentAcceleration = this->getAcceleration();
	const sf::Vector2f kfCurrentVelocity = this->getVelocity();

	if ((direction == DOWN && kfCurrentVelocity.y < 0) || (direction == UP && kfCurrentVelocity.y > 0))
	{
		if (kfCurrentAcceleration.y != 0)
			this->setAcceleration(sf::Vector2f(0, 0));
		this->brake();
	}
	else
	{
		// Check that the new acceleration doesn't exceed limits
		float fNewAccelerationY;

		if (kfCurrentAcceleration.y > 0)
		{
			fNewAccelerationY = kfCurrentAcceleration.y + s_vProperties.accelerationIncrement;
			if (!(fNewAccelerationY > s_vProperties.maxAcceleration))
				this->setAcceleration(kfCurrentAcceleration + sf::Vector2f(0, fNewAccelerationY));
			else
				this->setAcceleration(sf::Vector2f(0, s_vProperties.maxAcceleration));
		}
		else if (kfCurrentAcceleration.y < 0)
		{
			fNewAccelerationY = kfCurrentAcceleration.y - s_vProperties.accelerationIncrement;
			if (!(fNewAccelerationY < -s_vProperties.maxAcceleration))
				this->setAcceleration(kfCurrentAcceleration + sf::Vector2f(0, fNewAccelerationY));
			else
				this->setAcceleration(sf::Vector2f(0, -s_vProperties.maxAcceleration));
		}
		else
		{
			fNewAccelerationY = (direction == UP) ? -s_vProperties.accelerationIncrement : s_vProperties.accelerationIncrement;
			this->setAcceleration(sf::Vector2f(0, fNewAccelerationY));
		}
	}
}

void Vehicle::brake()
{
	const sf::Vector2f kfVelocity = this->getVelocity();

	if (kfVelocity.y > 0)
		(kfVelocity.y - s_vProperties.brakePower < 0) ? this->setVelocity(sf::Vector2f(kfVelocity.x, 0.f)) : this->setVelocity(sf::Vector2f(kfVelocity.x, kfVelocity.y - s_vProperties.brakePower));
	else if (kfVelocity.y < 0)
		(kfVelocity.y + s_vProperties.brakePower > 0) ? this->setVelocity(sf::Vector2f(kfVelocity.x, 0.f)) : this->setVelocity(sf::Vector2f(kfVelocity.x, kfVelocity.y + s_vProperties.brakePower));
}

void Vehicle::modifyDamage(const int kiDamage)
{
	m_uiDamage += kiDamage;
}

void Vehicle::refill(const unsigned int kuiAmount)
{
	// Check that the fuel level isn't more than 100
	const unsigned int kuiNewAmount = m_uiFuel + kuiAmount;
	m_uiFuel = (kuiNewAmount <= 100) ? kuiNewAmount : 100;
}

void Vehicle::resetSteering()
{
	// Reset steering
	if (m_fSteeringAngle != 0)
		if (m_fSteeringAngle < 0)
			(m_fSteeringAngle + s_vProperties.steeringIncrement < 0) ? m_fSteeringAngle += s_vProperties.steeringIncrement : m_fSteeringAngle = 0;
		else
			(m_fSteeringAngle - s_vProperties.steeringIncrement > 0) ? m_fSteeringAngle -= s_vProperties.steeringIncrement : m_fSteeringAngle = 0;
}

void Vehicle::shift(Direction direction)
{
	if (direction == UP)
		m_currentGear = D;
	else if (direction == DOWN)
		m_currentGear = R;
}

void Vehicle::steer(const Direction kDirection)
{
	// Limit the steering angle
	float fLeftLimit = -s_vProperties.maxSteeringAngle;
	float fRightLimit = s_vProperties.maxSteeringAngle;

	if (kDirection == LEFT)
		(m_fSteeringAngle - s_vProperties.steeringIncrement > fLeftLimit) ? m_fSteeringAngle -= s_vProperties.steeringIncrement : m_fSteeringAngle = fLeftLimit;
	else if (kDirection == RIGHT)
		(m_fSteeringAngle + s_vProperties.steeringIncrement < fRightLimit) ? m_fSteeringAngle += s_vProperties.steeringIncrement : m_fSteeringAngle = fRightLimit;
}

void Vehicle::update(const float kfDeltaTime)
{
	// Calculate the new velocity
	const sf::Vector2f kfAcceleration = this->getAcceleration();
	sf::Vector2f fVelocity = this->getVelocity();

	// Calculate the displacement
	const float kfSpeed = this->getSpeed();
	float fDisplacement = kfSpeed * kfDeltaTime;

	// Check the displacement
	if (fVelocity.y > 0)
		fDisplacement *= -1;

	// Calculate the new position of the car
	float fVehicleRotationRad = ang::DEGTORAD(this->getRotation() + 270.f);
	float fSteeringAngleRad = ang::DEGTORAD(m_fSteeringAngle) + fVehicleRotationRad;
	const sf::Vector2f kfVehiclePosition = this->getPosition();

	sf::Vector2f fFrontWheel, fRearWheel;

	// Calculate the positions of the theoretical front and rear wheels
	const sf::Vector2f kfVehicleOrientation = sf::Vector2f(cosf(fVehicleRotationRad), sinf(fVehicleRotationRad));
	const sf::Vector2f kfAlignedWheelRotation = kfVehicleOrientation * (s_vProperties.wheelBase / 2.f);
	fFrontWheel = kfVehiclePosition + kfAlignedWheelRotation;
	fRearWheel = kfVehiclePosition - kfAlignedWheelRotation;
	// Move the wheels
	const sf::Vector2f kfSteeringOrientation = sf::Vector2f(cosf(fSteeringAngleRad), sinf(fSteeringAngleRad));
	fFrontWheel += kfSteeringOrientation * fDisplacement;
	fRearWheel += kfVehicleOrientation * fDisplacement;
	// Update the position and angles
	this->setPosition((fFrontWheel + fRearWheel) / 2.f);
	this->setRotation(ang::RADTODEG(atan2(fFrontWheel.y - fRearWheel.y, fFrontWheel.x - fRearWheel.x)) - 270.f);
	// Update the vehicle rotation and velocity variables
	fVelocity += kfAcceleration * kfDeltaTime;
	// Calculate the drag
	const sf::Vector2f kfDrag = this->getDragCoefficient() * this->getMass() * kfSpeed * kfDeltaTime;
	// Update the velocity
	if (fVelocity.y > 0.f)
		(fVelocity.y - kfDrag.y > 0.f) ? fVelocity.y -= kfDrag.y : fVelocity.y = 0;
	else if (fVelocity.y < 0.f)
		(fVelocity.y + kfDrag.y < 0.f) ? fVelocity.y += kfDrag.y : fVelocity.y = 0;

	this->setVelocity(sf::Vector2f(0.f, fVelocity.y));
}