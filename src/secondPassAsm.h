#pragma once
#include "assembler.h"
#include <map>
#include <fstream>

class AssemblerException : public std::exception
{
public:
	AssemblerException(std::string msg);
	const char* what() const noexcept;
private:
	std::string _msg;
};


struct RelocationRecord
{
	bool isData;
	int offset; //gde krece offset
	string sectionName; //trenutna sekcija, gde da trazi relokaciju
	string relocationType;
	string symbolName;
	//bitno je da se razlikuje da li je offset u odnosu na sekciju ili u odnosu na sve sekcije (kod linkera)
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

	void addEqu(string symbolName, string tokenText);
	void skipDef(int value);
	string currentSection;
	int locationCounter;
	void updateSectionMap(vector<char> data, int locationCounterInc);
	void updateSectionMap(int offset, vector<char> data, int locationCounterInc);
	void updateSectionMap(int offset, vector<char> data, int locationCounterInc, string section);
	int instructionBinary(string insTxt);

	int absoluteSymbolValue(string symbolName);
	int pcRelativeSymbolValue(string symbolName);

	void createTxtFile(string path);
	void createBinaryFile(string path);


};