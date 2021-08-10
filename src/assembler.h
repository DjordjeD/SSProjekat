#pragma once
#include "symbol.h"
#include "sectionTable.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

static int sidSection = 1;

class Assembler {

public:

	vector<Section> sectionList;
	vector<Symbol> symbolTable;

	Assembler()
	{
		Section absoluteSection(-1, "ABSOLUTE");
		sectionList.push_back(absoluteSection);
		Section undefinedSection(0, "UNDEFINED");
		currentSection = undefinedSection;
		sectionList.push_back(undefinedSection);

		Symbol section1("ABSOLUTE", "ABSOLUTE", -1);
		Symbol section2("UNDEFINED", "UNDEFINED", 0);

		symbolTable.push_back(section1);
		symbolTable.push_back(section2);

		locationCounter = 0;
	}

	void addSection(string SectionName);
	Section getSection(string sectionName);
	void changeCurrentSection(string sectionName);


	void printSectionList();
	void printSymbolTable();

	void addSymbol(string symbolName);
	Symbol* getSymbol(string symbolName);
	Symbol* getSymbol(string symbolName, string sectionName);
	Symbol* getSymbolCheck(string symbolName);

	void addLabel(string text);
	void addGlobal(string text);
	void addExtern(string text);
	void addEqu(string name, string tokenText);
	void addWord(string text);
	void skipDef(int size);

	string scopePrint(Symbol s);

	void updateLocationCounter(int size);

private:

	int locationCounter;

	Section currentSection;
	Section undefinedSection;
	Section absoluteSection;
};