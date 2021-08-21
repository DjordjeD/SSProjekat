#include "assembler.h"



//ovde potencijalno ima gresaka sa definicjom sekcije
// sa definicijom labele

void Assembler::addSection(string SectionName)
{
	Section temp(sidSection, SectionName); sidSection++;
	sectionList[SectionName] = temp;


	Symbol* symbol = getSymbol(SectionName);
	//pazi na ubacivanje sekcija 
	//tek u drugom prolazu on zna koja je sekcija
	if (symbol->isDefined() == false)
	{
		sectionList[currentSection].increaseSize(locationCounter);


		currentSection = SectionName;

		symbol->setSection(currentSection);
		//symbol->setValue(0);
		symbol->setIsDefined(true);
		locationCounter = 0;

	}

}



void Assembler::changeCurrentSection(string sectionName)
{
	//sectionList.push_back(currentSection);
	//for (auto& i : sectionList)
	//{
	//	if (i.getSectionName() == sectionName) {
	//		currentSection = i;
	//	}
	//}
}

void Assembler::printSectionList()
{
	cout << "Section LIST " << endl;
	cout << "ID\t Section\t Size" << endl;
	for (auto& i : sectionList)
	{
		cout << hex << i.second.getID() << "\t" << i.second.getSectionName() << "\t" << i.second.getSectionSize() << endl;
	}
}



void Assembler::printSymbolTable()
{
	cout << "Symbol table:" << endl;
	cout << "Value\tType\tSection\t\tName\t\tId" << endl;

	for (auto& i : symbolTable)
	{
		cout << hex << i.getValue() << " " << scopePrint(i) << " " << i.getSection() << " " << i.getSymbolName() << " " << i.getNumberID() << endl;
	}
	cout << endl << " END OF SYMBOL TABLE" << endl << endl;
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

Symbol* Assembler::getSymbol(string symbolName, string sectionName)
{
	// zasad visak
	return nullptr;
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

	if (symbol->isDefined() == false)
	{
		symbol->setIsDefined(true);
		symbol->setValue(locationCounter);
		symbol->setSection(currentSection);
		//symbol->setSymbolType(SymbolType::LABEL);
	}
}

void Assembler::addGlobal(string text)
{
	Symbol* symbol = getSymbol(text);

	symbol->setSymbolScope(SymbolScope::GLOBAL);
}

void Assembler::addExtern(string text)
{
	Symbol* symbol = getSymbol(text);

	symbol->setSymbolScope(SymbolScope::EXTERN);
}

void Assembler::addEqu(string name, string tokenText)
{
	Symbol* symbol = getSymbolCheck(name);
	int value;
	if (tokenText.at(0) == '0') value = std::stoi(tokenText, 0, 16);
	else value = std::stoi(tokenText);

	Symbol s1(name, value);
	/*
	symbolType = SymbolType::EQU;
		value = _value;
		symbolScope = SymbolScope::LOCAL;
		numberID = sid++;
		isDefinedAlready = false;
		//sekcija absolute
	*/
	//dodaje lokaciju to fali
	// ne radi dizanje sekcije JEBEM TI SEKCIJE 
	sectionList["ABSOLUTE"].increaseSize(2);

	symbolTable.push_back(s1);
}



void Assembler::addWord(string text)
{
	Symbol* symbol = getSymbol(text);

	// u drugom prolazu mozda i relokacija i vrednost u memoriju
	locationCounter += 2;
}

void Assembler::skipDef(int size)
{
	locationCounter += size;
}

string Assembler::scopePrint(Symbol s)
{
	if (s.getSymbolScope() == SymbolScope::GLOBAL)
	{
		return "g";
	}
	else if (s.getSymbolScope() == SymbolScope::EXTERN)
	{
		return "e";
	}
	else 	if (s.getSymbolScope() == SymbolScope::LOCAL) return "l";
}

void Assembler::updateLocationCounter(int size)
{
	locationCounter += size;
	sectionList[currentSection].increaseSize(size);
}
