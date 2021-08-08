#include "symbol.h"

string Symbol::getSymbolName()
{
	return symbolName;
}

SymbolType Symbol::getSymbolType()
{
	return symbolType;
}

int Symbol::getValue()
{
	return value;
}

SymbolScope Symbol::getSymbolScope()
{
	return symbolScope;
}

int Symbol::getNumberID()
{
	return numberID;
}

bool Symbol::isDefined()
{
	return isDefinedAlready;
}

string Symbol::getSection()
{
	return symbolSection;
}

void Symbol::setSection(string section)
{
	symbolSection = section;
}

void Symbol::setSymbolName(string name)
{
	symbolName = name;
}

void Symbol::setSymbolType(SymbolType type)
{
	symbolType = type;
}

void Symbol::setValue(int newValue)
{
	value = newValue;
}

void Symbol::setSymbolScope(SymbolScope scope)
{
	symbolScope = scope;
}

void Symbol::setNumberID(int number)
{
	numberID = number;
}

void Symbol::setIsDefined(bool b)
{
	isDefinedAlready = b;
}
