#pragma once

#include "tokens.h"
#include "secondPassAsm.h"
#include <iostream>
#include <vector>
using namespace std;


class SecondParserException : public exception {


	const char* whichError() const noexcept {
		return "Parsing Error";
	}
};



class SecondPassParser {
public:
	void parse();

	SecondPassParser(vector<Token>& _tokens, SecondPassAsm& assembler1)
	{
		tokens = _tokens;
		currTokenID = 0;
		assembler = assembler1;
	}
	int currTokenID;
	Token currToken;
	Token nextToken;
	vector<Token> tokens;

private:

	SecondPassAsm assembler;
	void parseLine();
	void directiveAdd();
	void instructionAdd();

	void debug(TokenType tokenType);
	void debug(vector<TokenType> list);
	bool checkCurrentAndSkipNext(TokenType tokenType);
	void defineWord();



	//proveri kad se pomeraju tokeni da li odmah ili pre


};