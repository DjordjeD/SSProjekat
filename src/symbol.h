#pragma once
#include <iostream>;
#include "sectionTable.h"
using namespace std;


enum class SymbolType { EQU, LABEL, SECTION };
enum class SymbolScope { GLOBAL, LOCAL,EXTERN };

static int sid = 0;

class Symbol {
public:
	string getSymbolName();
	SymbolType getSymbolType();
	int getValue();
	SymbolScope getSymbolScope();
	int getNumberID();
	bool isDefined();
	//get i set section;
	string getSection();

	void setSymbolName(string name);
	void setSymbolType(SymbolType type);
	void setValue(int newValue);
	void setSymbolScope(SymbolScope scope);
	void setNumberID(int number);
	void setIsDefined(bool b);

	Symbol(string _symbolName):symbolName(_symbolName){//pazi na ovo moguce greske zbog defaulta
		symbolType = SymbolType::LABEL;
		value = 0;
		symbolScope = SymbolScope::LOCAL;
		numberID = sid++;
		isDefinedAlready = false;
		symbolSection = "UNDEFINED";
		//default section undefined
	}

	Symbol(string _symbolName,int _value) :symbolName(_symbolName) {//pazi na ovo moguce greske zbog defaulta
		symbolType = SymbolType::EQU;
		value = _value;
		symbolScope = SymbolScope::LOCAL;
		numberID = sid++;
		isDefinedAlready = false;
		//sekcija absolute
		symbolSection = "ABSOLUTE";
	}

	Symbol(string _symbolName, string _section) :symbolName(_symbolName) {//pazi na ovo moguce greske zbog defaulta
		symbolType = SymbolType::LABEL;
		value = 0;
		symbolScope = SymbolScope::LOCAL;
		numberID = sid++;
		isDefinedAlready = false;
		//sekcija zadata
		symbolSection = _section;
	}


	private:

	string symbolName;
	SymbolType symbolType;
	int value;
	SymbolScope symbolScope;
	int numberID;
	bool isDefinedAlready;
	string symbolSection;

};