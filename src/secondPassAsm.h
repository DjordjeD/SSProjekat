#pragma once
#include "assembler.h"


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

	SecondPassAsm(vector<Section> sectionList1, vector<Symbol> symbolTable1)
	{
		copy(sectionList1.begin(), sectionList1.end(), back_inserter(sectionList));
		copy(symbolTable1.begin(), symbolTable1.end(), back_inserter(symbolTable));

		for (auto& i : sectionList)
		{
			RelocationRecord r;
			r.sectionName = i.getSectionName();
			relocationTable.push_back(r);
		}

	};

	SecondPassAsm() = default;

	void printSectionList();
	void printSymbolTable();
	string scopePrint(Symbol s);


private:



};