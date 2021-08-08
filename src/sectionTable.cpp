#include "sectionTable.h"

int Section::getID()
{
	return id;
}

std::string Section::getSectionName()
{
	return sectionName;
}

int Section::getSectionSize()
{
	return sectionSize;
}

void Section::setId(int _id)
{
	id = _id;
}

void Section::setSectionName(std::string _sectionName)
{
	sectionName = _sectionName;
}

void Section::setSectionSize(int _sectionSize)
{
	sectionSize = _sectionSize;
}

void Section::increaseSize(int size)
{
	sectionSize += size;
}

void Section::increaseSize()
{
	sectionSize++;
}
