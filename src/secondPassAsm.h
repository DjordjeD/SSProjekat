#pragma once
#include "assembler.h"
#include <map>

struct RelocationRecord
{
	bool isData;
	int offset;
	string sectionName;
	string relocationType;
	string symbolName;
};


class SecondPassAsm
{


public:

	//vector<Section> sectionList;
	vector<Symbol> symbolTable;
	vector<RelocationRecord> relocationTable;
	map<string, Section> sectionMap;

	SecondPassAsm(map<string, Section> sectionList1, vector<Symbol> symbolTable1)
	{

		for (auto& i : sectionList1)
		{
			sectionMap[i.first] = i.second;
		}
		//copy(sectionList1.begin(), sectionList1.end(), back_inserter(sectionMap));
		copy(symbolTable1.begin(), symbolTable1.end(), back_inserter(symbolTable));


	};

	SecondPassAsm() = default;

	void printSectionMap();
	//void printSectionList();
	void printSymbolTable();
	string scopePrint(Symbol s);

	void addWord(int value);
	void addWord(string name);
	void addSection(string sectionName);
	Symbol* getSymbol(string symbolName);


	string currentSection;
	int locationCounter;

private:



};