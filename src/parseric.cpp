
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

	try
	{
		while (currToken.getTokenType() != TokenType::FILE_END && currToken.getTokenType() != TokenType::END)
		{
			current_line++;
			parseLine();
		}
	}
	catch (ParserException parserException)
	{
		cout << "Error at line: " << current_line << ". Parser failed";
	}
		
	
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

	if(token.getTokenType() == TokenType::SECTION){

		debug(TokenType::SYMBOL);

		//dodaj sekciju

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
	
	}
	else if (token.getTokenType() == TokenType::EXTERN){

		debug(TokenType::SYMBOL);
		//dodaj global

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		//vidi sta ces sa eksternom
		while (checkNext(TokenType::COMMA))
		{
			debug(TokenType::SYMBOL);
			//_assembler.globalDef(_curr_token.text());
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}
	
	
	}
	else if (token.getTokenType() == TokenType::GLOBAL) {

		debug(TokenType::SYMBOL);
		//dodaj global

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		while (checkNext(TokenType::COMMA))
		{
			debug(TokenType::SYMBOL);
			//_assembler.globalDef(_curr_token.text());
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}

	}
	else if (token.getTokenType() == TokenType::WORD) {

		debug({ TokenType::LITERAL, TokenType::SYMBOL });
		
		defineWord();
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		while (checkNext(TokenType::COMMA))
		{
			debug({ TokenType::LITERAL, TokenType::SYMBOL });

			defineWord();
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}


	}
	else if (token.getTokenType() == TokenType::EQU) {

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		debug(TokenType::SYMBOL);

		//sacuvaj simbol

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		debug(TokenType::LITERAL);
	
		//dodaj vrednost simbolu

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		

	}
	else if (token.getTokenType() == TokenType::SKIP) {
		checkNext(TokenType::LITERAL);
		//_assembler.skipDef(std::stoi(_curr_token.text()));
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

	}
	

}

void Parser::instructionAdd()
{
	cout << "usao u instruction ADD !" << endl;

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
}
