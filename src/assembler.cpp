#include "assembler.h"



//ovde potencijalno ima gresaka sa definicjom sekcije
// sa definicijom labele

void Assembler::addSection(string SectionName)
{
	Section temp(sidSection, SectionName); sidSection++;
	sectionList.push_back(temp);

	//Symbol* symbol = getSymbol(text);

	//pazi na ubacivanje sekcija 
	

}

Section* Assembler::getSection(string sectionName)
{
	for (auto& i : sectionList)
	{
		if (i.getSectionName() == sectionName) return &i;
	}
}

void Assembler::printSectionList()
{

	for (auto& i : sectionList)
	{
		cout << i.getID() << " " << i.getSectionName() << " " << i.getSectionSize() << endl;
	}
}


void Assembler::printSymbolTable()
{

	for (auto& i : symbolTable)
	{
		cout << i.getValue() << " " << " " << i.getSection()<<" "<<i.getSymbolName()<<" "<<i.getNumberID() << endl;
	}
}

void Assembler::addSymbol(string symbolName)
{
	Symbol s1(symbolName);
	/*
		symbolType = SymbolType::LABEL;
		value = 0;
		symbolScope = SymbolScope::LOCAL;
		numberID = sid++;
		isDefinedAlready = false;
		//default section undefined
    */
	symbolTable.push_back(s1);
}

Symbol* Assembler::getSymbol(string symbolName)
{
	for (auto& i : symbolTable)
	{
		if (i.getSymbolName() == symbolName) return &i;
	}

	addSymbol(symbolName);

	for (auto& i : symbolTable)
	{
		if (i.getSymbolName() == symbolName) return &i;
	}

}

Symbol* Assembler::getSymbolCheck(string symbolName)
{
	for (auto& i : symbolTable)
	{
		if (i.getSymbolName() == symbolName) return &i;
	}

	return NULL;

}





void Assembler::addLabel(string text)
{
	Symbol* symbol = getSymbol(text);
	//ako nije definisan definisi ga

	//if (!symbol->isDefined())
	//{
	//	symbol->setIsDefined(true);
	//	symbol.
	//}
}

void Assembler::addGlobal(string text)
{
	Symbol* symbol = getSymbol(text);

	symbol->setSymbolScope(SymbolScope::GLOBAL);
}

void Assembler::addExtern(string text)
{
	Symbol* symbol = getSymbol(text);

	symbol->setSymbolScope(SymbolScope::GLOBAL);
}

void Assembler::addEqu(string name, string tokenText)
{
	Symbol* symbol = getSymbolCheck(name);
	Symbol s1(name, std::stoi(tokenText));
	/*
	symbolType = SymbolType::EQU;
		value = _value;
		symbolScope = SymbolScope::LOCAL;
		numberID = sid++;
		isDefinedAlready = false;
		//sekcija absolute
	*/
	symbolTable.push_back(s1);
}



void Assembler::addWord(string text)
{
	Symbol* symbol = getSymbol(text);

	// u drugom prolazu mozda i relokacija i vrednost u memoriju
}
