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

	vector<Section> sectionList;
	vector<Symbol> symbolTable;
	vector<RelocationRecord> relocationTable;
	map<string, Section> sectionMap;

	SecondPassAsm(vector<Section> sectionList1, vector<Symbol> symbolTable1)
	{
		copy(sectionList1.begin(), sectionList1.end(), back_inserter(sectionList));
		copy(symbolTable1.begin(), symbolTable1.end(), back_inserter(symbolTable));

		for (auto& i : sectionList)
		{
			RelocationRecord r;
			r.sectionName = i.getSectionName();
			relocationTable.push_back(r);
			//napuni mapu
			sectionMap[i.getSectionName()] = i;
		}




	};

	SecondPassAsm() = default;

	void printSectionMap();
	void printSectionList();
	void printSymbolTable();
	string scopePrint(Symbol s);

	void addWord(int value);
	void addWord(string value);

private:



};