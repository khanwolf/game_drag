#ifndef GAMEFILEOPERATIONS_H
#define GAMEFILEOPERATIONS_H

/*!
\file	GameFileOperations.h
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*!
\namespace	gfo
\brief		Used to read game files.
*/
namespace gfo
{
	/*!
	\struct	ObjectProperties
	\brief	Used to store properties on an object read from a game file.
	*/
	struct ObjectProperties
	{
		float fLocationX;																								//!< Stores the X axis location of the object.
		float fLocationY;																								//!< Stores the Y axis location of the object.
		std::string sIdentifier;																						//!< Stores the identifier of the object.
	};

	// --- Functions
	bool isBlank(std::string sString);																					//!< Check if the line is blank
	bool isComment(std::string sString);																				//!< Check if the string is a comment
	std::vector<std::string> splitString(std::string sString, const std::string& ksDelimiter);							//!< Split the string using the delimiter given
	std::vector<ObjectProperties> getObjectProperties(const std::string& ksString, const std::string& ksDelimiter);		//!< Return a vector of objects using the string and delimiter given
	// --- Methods
	void removeWhiteSpace(std::string& sString);																		//!< Remove any whitespace(s) in the string
};

#endif