#ifndef ANGLE_H
#define ANGLE_H

/*! 
\file	Angle.h
*/

/*!
\namespace	ang
\brief		Used to perform a range of mathematical functions using angles.
*/
namespace ang
{
	//! The first 9 digits of PI
	const float s_PI = 3.14159265f;
	//! The first 9 digits of PI multiplied by 2.
	const float s_2PI = s_PI * 2;

	// Conversion functions
	/*! Convert from degrees to radians. */
	inline float DEGTORAD(const float kfDegrees) { return kfDegrees * s_PI / 180.0f; }
	/*! Convert from radians to degrees. */
	inline float RADTODEG(const float kfRadians) { return kfRadians * 180.0f / s_PI; }

	//! Normalise an angle so that it falls within the range of 180 to -180.
	template<class T>
	void Normalise(T& angle)
	{
		while (angle < -180) 
			angle += 360;
		while (angle > 180) 
			angle -= 360;
	}

	//! Normalise an angle so that it falls within the range of PI to -PI.
	template<class T>
	void NormaliseRad(T& angle)
	{
		while (angle < -s_PI)
			angle += s_2PI;
		while (angle > s_PI)
			angle -= s_2PI;
	}

	//! Check if an angle is within a certain range.
	template<class T>
	bool IsWithinRange(T angle, T leftLimit, T rightLimit)
	{
		leftLimit -= angle;
		rightLimit -= angle;
		Normalise(leftLimit);
		Normalise(rightLimit);
		if (leftLimit * rightLimit >= 0)
			return false;
		return fabs(leftLimit - rightLimit) < 180;
	}
};

#endif