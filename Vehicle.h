#ifndef VEHICLE_H
#define VEHICLE_H

/*!
\file	Vehicle.h
\brief	Used to define game objects as vehicles.
*/

#include <iostream>
#include "MoveableObject.h"
#include "VectorMaths2D.h"

/*!
\enum	AutoGearBox
\brief	Used to define the current gear of a vehicle's automatic gearbox.
*/
enum AutoGearBox { R, D };
/*!
\enum	VehicleType
\brief	used to define a type of vehicle.
*/
enum VehicleType { CAR, MOTORCYCLE, TRUCK };

/*!
\struct	VehicleProperties
\brief	Used to define key vehicle properties and limits
*/
struct VehicleProperties
{
	float accelerationIncrement;			//!< Used to define the rate at which the vehicle may accelerate.
	float brakePower;						//!< Used to define the rate at which the vehicle may slow down when braking.
	float maxAcceleration;					//!< Used to define the maximum rate of acceleration.
	float maxSteeringAngle;					//!< Used to define a maximum steering angle.
	float steeringIncrement;				//!< Used to define the rate at which a vehicle may steer.
	float wheelBase;						//!< Used to define the wheelbase of a vehicle.
	unsigned int maxRPM;					//!< Used to define the maximum RPM of a vehicle.
};

/*!
\struct	Vehicle
\brief	Extends MoveableObject to give sprites the properties of vehicles.
*/
class Vehicle : public MoveableObject 
{
public:
	// --- Constructor
	//! Constructor
	Vehicle(const sf::Vector2f& kfDragCoefficient, const float kfBrakePower, const float kfMass, VehicleType type);
	// --- Functions
	float getSteeringAngle() const;																			//!< Returns the vehicle's current steering angle.
	unsigned int getDamage() const;																			//!< Returns the amount of damage to the vehicle.
	unsigned int getFuelLevel() const;																		//!< Returns the vehicle's fuel level.
	unsigned int getRPM() const;																				//!< Returns the vehicle's current RPM.
	AutoGearBox getCurrentGear() const;																		//!< Returns the vehicle's current gear. 
	VehicleType getType() const;																		//!< Returns the vehicle's type. 
	// --- Methods
	void accelerate(Direction direction);																//!< Used to accelerate the vehicle.
	void brake();																						//!< Used to slow down the vehicle.
	void modifyDamage(const int kiDamage);																//!< Used to modify the amount of damage to the vehicle.
	void refill(const unsigned int kuiAmount);															//!< Used to refill the vehicle's fuel.
	void resetSteering();																				//!< Used to gradually reset the vehicle's steering angle to 0.
	void shift(Direction direction);																	//!< Used to change the vehicle's current gear.
	void steer(const Direction kDirection);																//!< Used to steer the vehicle in a perticular direction.
	void update(const float kfDeltaTime) override;														//!< Used to update the vehicle.
protected:
	VehicleProperties s_vProperties;																	//!< Stores the key vehicle properties and limits.
private:
	// --- Variables
	float m_fSteeringAngle;																				//!< Stores the vehicle's current steering angle.
	unsigned int m_uiDamage;																			//!< Stores the vehicle's current amount of damage (0-100).
	unsigned int m_uiFuel;																				//!< Stores the vehicle's current fuel level (0-100).
	unsigned int m_uiRPM;																				//!< Stores the vehicle's current RPM.
	AutoGearBox m_currentGear;																			//!< Stores the vehicle's current gear.
	const VehicleType m_type;																			//!< Stores the vehicle's type.
};

#endif