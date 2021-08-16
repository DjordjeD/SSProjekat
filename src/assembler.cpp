#include "assembler.h"



//ovde potencijalno ima gresaka sa definicjom sekcije
// sa definicijom labele

void Assembler::addSection(string SectionName)
{
	Section temp(sidSection, SectionName); sidSection++;
	sectionList.push_back(temp);


	Symbol* symbol = getSymbol(SectionName);
	//pazi na ubacivanje sekcija 
	//tek u drugom prolazu on zna koja je sekcija
	if (symbol->isDefined() == false)
	{
		currentSection->increaseSize(locationCounter);



		for (size_t i = 0; i < sectionList.size(); i++)
		{
			if (sectionList.at(i).getSectionName() == SectionName)
				currentSection = &sectionList.at(i);
		}


		//currentSection = getSection(SectionName);
		////changeCurrentSection(SectionName);
		//Section temp = getSection(SectionName);// sta ako ne postoji
		//currentSection.setId(temp.getID());
		//currentSection.setSectionName(temp.getSectionName());
		//currentSection.setSectionSize(temp.getSectionSize());
		////ne radi promena sekcija

		symbol->setSection(currentSection->getSectionName());
		//symbol->setValue(0);
		symbol->setIsDefined(true);
		locationCounter = 0;

	}

}

Section& Assembler::getSection(string sectionName)
{
	/*for (auto& i : sectionList)
	{
		if (i.getSectionName() == sectionName) return i;
	}*/


	for (size_t i = 0; i < sectionList.size(); i++)
	{
		if (sectionList.at(i).getSectionName() == sectionName) return sectionList.at(i);
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

	for (auto& i : sectionList)
	{
		cout << i.getID() << " " << i.getSectionName() << " " << i.getSectionSize() << endl;
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
		symbol->setSection(currentSection->getSectionName());
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
	Symbol s1(name, std::stoi(tokenText));
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
	absoluteSection->increaseSize(2);
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
	currentSection->increaseSize(size);
}
