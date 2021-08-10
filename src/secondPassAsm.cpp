
#include "secondPassAsm.h"

void SecondPassAsm::printSectionMap()
{
	for (auto& i : sectionMap)
	{
		cout << i.first << endl;
		cout << i.second.getSectionName() << endl;
	}
}

void SecondPassAsm::printSectionList()
{

	for (auto& i : sectionList)
	{
		cout << i.getID() << " " << i.getSectionName() << " " << i.getSectionSize() << endl;
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

void SecondPassAsm::addWord(int value)
{
}

void SecondPassAsm::addWord(string value)
{
}
