#pragma once
#include "symbol.h"
#include "sectionTable.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include<iomanip>

using namespace std;

static int sidSection = 1;

class Assembler {

public:

	map<string, Section> sectionList;
	vector<Symbol> symbolTable;

	Assembler()
	{
		Section absoluteSection1(-1, "ABSOLUTE");
		//absoluteSection = &absoluteSection1;
		sectionList["ABSOLUTE"] = absoluteSection1;
		//absoluteSection = &sectionList.at(0);
		Section undefinedSection1(0, "UNDEFINED");
		sectionList["UNDEFINED"] = undefinedSection1;

		currentSection = "UNDEFINED";

		Symbol section1("ABSOLUTE", "ABSOLUTE", -1);
		Symbol section2("UNDEFINED", "UNDEFINED", 0);

		section1.setSymbolType(SymbolType::SECTION);
		section2.setSymbolType(SymbolType::SECTION);

		symbolTable.push_back(section1);
		symbolTable.push_back(section2);

		locationCounter = 0;
	}

	void addSection(string SectionName);
	//Section& getSection(string sectionName);
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

	void addLastSection();
	string scopePrint(Symbol s);

	void updateLocationCounter(int size);


	string currentSection;
	Section* undefinedSection;
	Section* absoluteSection;


private:

	int locationCounter;


};