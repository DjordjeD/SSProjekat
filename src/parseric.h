#pragma once

#include "tokens.h"
#include "assembler.h"
#include "secondPassAsm.h"

#include <iostream>
#include <vector>
using namespace std;


class Parser {
public:
	SecondPassAsm parse();
	Parser(vector<Token>& _tokens)
	{
		tokens = _tokens;
		currTokenID = 0;
	}
	int currTokenID;
	Token currToken;
	Token nextToken;
	vector<Token> tokens;
	static bool isDirective(const Token& token);

private:

	Assembler assembler;
	void parseLine();
	void directiveAdd();
	void instructionAdd();

	void debug(TokenType tokenType);
	void debug(vector<TokenType> list);
	bool checkCurrentAndSkipNext(TokenType tokenType);
	void defineWord();



	//proveri kad se pomeraju tokeni da li odmah ili pre


};