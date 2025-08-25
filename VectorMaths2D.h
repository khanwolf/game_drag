#ifndef VECTORMATHS2D_H
#define VECTORMATHS2D_H

/*!
\file VectorMaths2D.h
*/

#include <math.h>
#include <SFML/System/Vector2.hpp>
#include "Angle.h"

/*!
\namespace vm2d
\brief Used to perform mathematical functions on vectors.
*/
namespace vm2d 
{
	//! Returns the angle between two vectors
	template <class T>
	float AngleBetween(const sf::Vector2<T>& kVector1, const sf::Vector2<T>& kVector2)
	{
		return acos(Dot(kVector1, kVector2) / (Magnitude(kVector1) * Magnitude(kVector2)));
	}

	//! Returns the cross product of two vectors
	template <class T>
	float Cross(const sf::Vector2<T>& kVector1, const sf::Vector2<T>& kVector2)
	{
		return kVector1.x * kVector2.y - kVector2.x * kVector1.y;
	}

	//! Returns the dot product of two vectors
	template <class T>
	float Dot(const sf::Vector2<T>& kVector1, const sf::Vector2<T>& kVector2)
	{
		return kVector1.x * kVector2.x + kVector1.y * kVector2.y;
	}

	//! Returns the magnitude of a vector
	template <class T>
	float Magnitude(const sf::Vector2<T>& kVector)
	{ 
		return sqrtf(powf(kVector.x, 2) + powf(kVector.y, 2));
	}

	//! Multiplies two vectors
	template <class T>
	sf::Vector2<T> Multiply(const sf::Vector2<T>& kVector1, const sf::Vector2<T>& kVector2)
	{
		return sf::Vector2f(kVector1.x * kVector2.x, kVector1.y * kVector2.y);
	}

	//! Normalises a vector
	template <class T>
	sf::Vector2<T> Normalise(const sf::Vector2<T>& kVector)
	{
		// Get the vector's magnitude
		const float kfVectorMagnitude = Magnitude(kVector);
		return sf::Vector2<T>(kVector / kfVectorMagnitude);
	}

	//! Rotates a vector
	template <class T>
	sf::Vector2<T> Rotate(const sf::Vector2<T>& kVector, const float kfDegrees)
	{ 	
		// Convert degrees into radians
		const float kfRadians = ang::DEGTORAD(kfDegrees);
		const float kfCos = cosf(kfRadians);
		const float kfSin = sinf(kfRadians);
		return sf::Vector2<T>(kVector.x * kfCos - kVector.y * kfSin, kVector.x * kfSin + kVector.y * kfCos);
	}

	//! Scales a vector
	template<class T>
	sf::Vector2<T> Scale(const sf::Vector2<T>& kVector, const float kfScalar)
	{
		return sf::Vector2<T>(kfScalar * kVector.x, kfScalar * kVector.y);
	}
};

#endif