#include "GameFileOperations.h"

// --- Functions

bool gfo::isBlank(std::string sString)
{
	removeWhiteSpace(sString);
	return sString.length() == 0;
}

bool gfo::isComment(std::string sString)
{
	removeWhiteSpace(sString);
	return sString.substr(0, 1) == "#";
}

std::vector<std::string> gfo::splitString(std::string sString, const std::string& ksDelimiter)
{
	size_t pos;
	std::vector<std::string> asValues;
	
	// Get and store each value
	while ((pos = sString.find(ksDelimiter)) != std::string::npos)
	{
		asValues.push_back(sString.substr(0, pos));
		sString.erase(0, pos + ksDelimiter.length());
	}
	asValues.push_back(sString.substr(0, pos));

	return asValues;
}

std::vector<gfo::ObjectProperties> gfo::getObjectProperties(const std::string& ksString, const std::string& ksDelimiter)
{
	std::vector<std::string> asSections;

	// Split the string into individual sections
	std::vector<std::string> asSectionSplit = gfo::splitString(ksString, ksDelimiter);
	for (std::string& sValue : asSectionSplit)
		asSections.push_back(sValue);

	// Split the section string and store the properties
	const std::string ksObjectDelimiter = "|";
	std::vector<ObjectProperties> aObjectProperties;

	for (std::string& sSection : asSections)
	{
		ObjectProperties objectProperties;
		// Split the string
		std::vector<std::string> asPropertySplit = gfo::splitString(sSection, ksObjectDelimiter);
		// Check the validity of the array
		if (asPropertySplit.size() < 3 || asPropertySplit.size() > 3)
			// Invalid properties
			continue;
		// Assign the properties
		objectProperties.sIdentifier = asPropertySplit.front();
		objectProperties.fLocationX = std::stof(asPropertySplit.at(1));
		objectProperties.fLocationY = std::stof(asPropertySplit.back());
		aObjectProperties.push_back(objectProperties);
	}

	return aObjectProperties;
}

// --- Methods

void gfo::removeWhiteSpace(std::string& sString)
{
	sString.erase(std::remove_if(sString.begin(), sString.end(), isspace), sString.end());
}