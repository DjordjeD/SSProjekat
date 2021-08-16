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
		Section absoluteSection1(-1, "ABSOLUTE");
		absoluteSection = &absoluteSection1;
		sectionList.push_back(absoluteSection1);
		//absoluteSection = &sectionList.at(0);
		Section undefinedSection1(0, "UNDEFINED");
		currentSection = &undefinedSection1;
		undefinedSection = &undefinedSection1;
		sectionList.push_back(undefinedSection1);

		Symbol section1("ABSOLUTE", "ABSOLUTE", -1);
		Symbol section2("UNDEFINED", "UNDEFINED", 0);

		symbolTable.push_back(section1);
		symbolTable.push_back(section2);

		locationCounter = 0;
	}

	void addSection(string SectionName);
	Section& getSection(string sectionName);
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


	Section* currentSection;
	Section* undefinedSection;
	Section* absoluteSection;


private:

	int locationCounter;


};