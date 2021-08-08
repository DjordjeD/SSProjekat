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
		Section sec(-1, "ABSOLUTE");
		sectionList.push_back(sec);
		Section sec2(0, "UNDEFINED");
		sectionList.push_back(sec2);

		Symbol section1("ABSOLUTE", "ABSOLUTE");
		Symbol section2("UNDEFINED", "UNDEFINED");

		symbolTable.push_back(section1);
		symbolTable.push_back(section2);
		
	}

	void addSection(string SectionName);
	Section* getSection(string sectionName);
	
	void printSectionList();
	void printSymbolTable();

	void addSymbol(string symbolName);
	Symbol* getSymbol(string symbolName);
	Symbol* getSymbol(string symbolName,string sectionName);
    Symbol* getSymbolCheck(string symbolName);	

		void addLabel(string text);
		void addGlobal(string text);
		void addExtern(string text);
		void addEqu(string name,string tokenText);
		void addWord(string text);	
	
		string scopePrint(Symbol s);

};