
#include "secondPassAsm.h"

void SecondPassAsm::printSectionMap()
{
	for (auto& i : sectionMap)
	{
		cout << i.first << endl;
		cout << i.second.getSectionName() << endl;
	}
}




void SecondPassAsm::printSymbolTable()
{
	cout << "Symbol table:" << endl;
	cout << "Value\tType\tSection\t\tName\t\tId" << endl;

	for (auto& i : symbolTable)
	{
		cout << hex << i.getValue() << " " << scopePrint(i) << " " << i.getSection() << " " << i.getSymbolName() << " " << i.getNumberID() << endl;
	}
	cout << endl << " END OF SYMBOL TABLE" << endl << endl;
}



string SecondPassAsm::scopePrint(Symbol s)
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

void SecondPassAsm::addWord(string name)
{
	Symbol* symbol = getSymbol(name);
	if (symbol != NULL)
	{
		if (symbol->getSection() == "ABSOLUTE")
		{

			sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
			sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
			sectionMap[currentSection].offsets.push_back(locationCounter);
		}
		else
		{
			if (symbol->isDefined())
			{
				if (symbol->getSymbolScope() == SymbolScope::LOCAL)
				{

					sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
					sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
					sectionMap[currentSection].offsets.push_back(locationCounter);


					RelocationRecord temp;
					temp.relocationType = "R_HYP_16";
					temp.isData = true;
					temp.sectionName = currentSection;
					temp.symbolName = symbol->getSection();//ovde uzima sekciju jer je lokalan
					temp.offset = locationCounter;

					relocationTable.push_back(temp);
				}
				else
				{
					//OVDE TREBA DA SE DODAJU NULE
					sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
					sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
					sectionMap[currentSection].offsets.push_back(locationCounter);


					RelocationRecord temp;
					temp.relocationType = "R_HYP_16";
					temp.isData = true;
					temp.sectionName = currentSection;
					temp.symbolName = symbol->getSymbolName();//ovde uzima simbol
					temp.offset = locationCounter;

					relocationTable.push_back(temp);
				}
			}
			else
			{
				//symbol niej definisan, ima NULE
				sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
				sectionMap[currentSection].data.at(sectionMap[currentSection].offsets.size()).push_back(symbol->getValue());
				sectionMap[currentSection].offsets.push_back(locationCounter);


				RelocationRecord temp;
				temp.relocationType = "R_HYP_16";
				temp.isData = true;
				temp.sectionName = currentSection;
				temp.symbolName = symbol->getSymbolName();
				temp.offset = locationCounter;

				relocationTable.push_back(temp);
			}
		}
		//sectionMap[currentSection]
	}
	else {
		cout << "greska";
	}


}

void SecondPassAsm::addWord(int value)
{
}

void SecondPassAsm::addSection(string sectionName)
{
	locationCounter = 0;
	currentSection = sectionName;
}

Symbol* SecondPassAsm::getSymbol(string symbolName)
{
	for (auto& i : symbolTable)
	{
		if (i.getSymbolName() == symbolName) return &i;
	}
	return NULL;
}
