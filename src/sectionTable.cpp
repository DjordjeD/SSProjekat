#include "sectionTable.h"

int Sections::getID()
{
	return id;
}

std::string Sections::getSectionName()
{
	return sectionName;
}

int Sections::getSectionSize()
{
	return sectionSize;
}

void Sections::setId(int _id)
{
	id = _id;
}

void Sections::setSectionName(std::string _sectionName)
{
	sectionName = _sectionName;
}

void Sections::setSectionSize(int _sectionSize)
{
	sectionSize = _sectionSize;
}
