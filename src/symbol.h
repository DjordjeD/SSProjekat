#pragma once

using namespace std;
#include <iostream>;

enum class SymbolType { EQU, LABEL, SECTION };
enum class SymbolScope { GLOBAL, LOCAL };

class Symbol {

	string getSymbolName();
	SymbolType getSymbolType();
	int getValue();
	SymbolScope getSymbolScope();
	int getNumberID();
	bool isDefined();

	void setSymbolName(string name);
	void setSymbolType(SymbolType type);
	void setValue(int newValue);
	void setSymbolScope(SymbolScope scope);
	void setNumberID(int number);
	void setIsDefined(bool b);


	private:

	string symbolName;
	SymbolType symbolType;
	int value;
	SymbolScope symbolScope;
	int numberID;
	bool isDefinedAlready;

};