
#pragma once

#include "parseric.h"

class ParserException : public std::exception {
	const char* whichError() const noexcept {
		return "Parsing Error";
	}
};


void Parser::parse()
{
	currToken = tokens[currTokenID++];
	nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
	int current_line = 0;


	while (currToken.getTokenType() != TokenType::FILE_END && currToken.getTokenType() != TokenType::END)
	{
		try
		{
			current_line++;
			parseLine();
		}
		catch (ParserException parserException)
		{
			cout << "Error at line: " << current_line << ". Parser failed" << endl;
		}
		
	}

	
	assembler.printSymbolTable();
	assembler.printSectionList();
	// 
	
}

bool isDirective(const Token& token) {
	TokenType currType = token.getTokenType();

	return currType == TokenType::SECTION || currType == TokenType::EXTERN || currType == TokenType::GLOBAL || currType == TokenType::SKIP ||
		currType == TokenType::WORD || currType == TokenType::EQU;

}

void Parser::parseLine()
{

	if (currToken.getTokenType() == TokenType::SYMBOL)
	{
		//LABELA ADD
		if (nextToken.getTokenType() == TokenType::COLON)
		{
			//dodaj u asm LABELU
			assembler.addLabel(currToken.text());
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		}
		//INSTRUKCIJA ADD
		else instructionAdd();

	}
	else if (isDirective(currToken))
	{
		directiveAdd();
	}

	debug({ TokenType::LINE_END, TokenType::FILE_END });
	currToken = tokens[currTokenID++];
	nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
}



void Parser::directiveAdd()
{
	const Token token = currToken;
	//pre ovoga je bio section

	currToken = tokens[currTokenID++];
	nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

	if (token.getTokenType() == TokenType::SECTION) {

		debug(TokenType::SYMBOL);

		//dodaj sekciju
		assembler.addSection(currToken.text());

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

	}
	else if (token.getTokenType() == TokenType::EXTERN) {

		debug(TokenType::SYMBOL);
		//dodaj global
		assembler.addExtern(currToken.text());//mozda moze da se odmah zna sekcija tako sto ce asm da pamti u kojoj je sekciji
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		//vidi sta ces sa eksternom
		while (checkNext(TokenType::COMMA))
		{
			debug(TokenType::SYMBOL);
			assembler.addExtern(currToken.text());

			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}


	}
	else if (token.getTokenType() == TokenType::GLOBAL) {

		debug(TokenType::SYMBOL);
		//dodaj global
		assembler.addExtern(currToken.text());

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		while (checkNext(TokenType::COMMA))
		{
			debug(TokenType::SYMBOL);
			assembler.addExtern(currToken.text());
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}

	}
	else if (token.getTokenType() == TokenType::WORD) {

		debug({ TokenType::LITERAL, TokenType::SYMBOL });
		//ubaci token
		defineWord();
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		while (checkNext(TokenType::COMMA))
		{
			debug({ TokenType::LITERAL, TokenType::SYMBOL });
			//ubaci currtoken
			defineWord();
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}


	}
	else if (token.getTokenType() == TokenType::EQU) {
		/*
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		*/


		debug(TokenType::SYMBOL);

		//sacuvaj simbol

		string symbolNameTemp = currToken.text();

		//nova fja koja odmah ubacuje value uvek je value, Local, absolute, ime, id dodat.

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		debug(TokenType::LITERAL);

		assembler.addEqu(symbolNameTemp, currToken.text());//nekako da zna absolute sekciju
		//dodaj vrednost simbolu
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();


	}
	else if (token.getTokenType() == TokenType::SKIP) {
		debug(TokenType::LITERAL);
		assembler.skipDef(std::stoi(currToken.text()));
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

	}


}

void Parser::instructionAdd()
{
	//cout << "usao u instruction ADD !" << endl;
	cout << currToken.text();
	string instructionText = currToken.text();
	
	if (instructionText == "halt" || instructionText == "iret" || instructionText == "ret")
	{
		assembler.updateLocationCounter(2);
	}
	else if (instructionText == "int" || instructionText == "pop" || instructionText == "push" || instructionText == "not")//call fali mozda
	{   
		if (instructionText == "pop" || instructionText == "push") assembler.updateLocationCounter(3);
		else assembler.updateLocationCounter(2);

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
	}
	else if (instructionText == "jmp" || instructionText == "jeq" || instructionText == "jne" || instructionText == "jgt" || instructionText == "call")
	{
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		int incLoc = -1;

		if (checkNext(TokenType::LITERAL)) incLoc = 5;//jmp 1413
		else if (checkNext(TokenType::SYMBOL)) incLoc = 5; //jmp Label
		else if (checkNext(TokenType::PERCENTAGE)) { 
			//jmp %label pcrelative
		incLoc = 5; 
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		} 
		else if (checkNext(TokenType::STAR)){
			//kad je star prvi kod jmp
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
			debug({ TokenType::REGISTER, TokenType::STARTPAREN, TokenType::LITERAL, TokenType::SYMBOL });


			switch (currToken.getTokenType())
			{
			case TokenType::REGISTER: {
				//regdir
				incLoc = 3; 
			} break;
			case TokenType::STARTPAREN: {
				//uzmi sledeci
				currToken = tokens[currTokenID++];
				nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

				if (checkNext(TokenType::REGISTER)) {
					//zasada regind

					currToken = tokens[currTokenID++];
					nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

					if (checkNext(TokenType::PLUS))//ako nadje plus ima i displacement
					{
						//uzmi displacement
						currToken = tokens[currTokenID++];
						nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
						
						debug({ TokenType::LITERAL, TokenType::SYMBOL });
						incLoc=5;

						//uzmi desnu zagradu
						currToken = tokens[currTokenID++];
						nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

					}
					else {
						incLoc = 3;
					}

				}
				else
				{
					cout << "greska nije bio registar posle startparena";
				}

				
			
			} break;
				case TokenType::LITERAL:
				case TokenType::SYMBOL:  incLoc = 3;  break;
			}
		}

		assembler.updateLocationCounter(incLoc);

	}

	else if (instructionText == "ldr" || instructionText == "str")
	{


	}
	else {
		/*
		xchg regD, regS temp <= regD; regD <= regS; regS <= temp; -
		add regD, regS regD <= regD + regS; -
		sub regD, regS regD <= regD - regS; -
		mul regD, regS regD <= regD * regS; -
		div regD, regS regD <= regD / regS; -
		cmp regD, regS temp <= regD - regS; Z O C N
		not regD regD <= ~regD; -
		and regD, regS regD <= regD & regS; -
		or regD, regS regD <= regD | regS -
		xor regD, regS regD <= regD ^ regS; -
		test regD, regS temp <= regD & regS; Z N
		shl regD, regS regD <= regD << regS; Z C N
		shr regD, regS regD <= regD >> regS; Z C N*/

		checkNext(TokenType::REGISTER);
		checkNext(TokenType::COMMA);
		checkNext(TokenType::REGISTER);

		assembler.updateLocationCounter(3);

	}




	//da se dobije lineEnd
	currToken = tokens[currTokenID++];
	nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
}

void Parser::debug(TokenType tokenType)
{

	if (tokenType != currToken.getTokenType())
	{
		throw ParserException();
	}
}

void Parser::debug(vector<TokenType> list)
{
	for (auto& i : list)
	{
		if (i == currToken.getTokenType())
			return;
	}

	throw ParserException();
}

bool Parser::checkNext(TokenType tokenType)
{
	if (tokenType == currToken.getTokenType()){
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		return 1;
	}
	return 0;
}

void Parser::defineWord()
{
	//ovde si stao treba da se upise wrod
	/*if (currToken.getTokenType() == TokenType::SYMBOL)
		assembler.addWord(currToken.text());
	else
		assembler.addWord(std::stoi(currToken.text());*/

	assembler.addWord(currToken.text());
}
