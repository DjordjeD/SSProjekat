#include "secondPassParser.h"

void SecondPassParser::parse()
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
	catch (SecondParserException parserException)
	{
		cout << "Error at line: " << current_line << ". Parser failed" << endl;
	}
	catch (AssemblerException assemblerException)
	{
		cout << "Error at line: " << current_line << ". ASM failed  " << assemblerException.what() << endl;
	}

	assembler.printSectionMap();
	//assembler.printSectionList();
	assembler.printSymbolTable();

	cout << "sectionMAP" << endl;
	assembler.printSectionMap();

}


bool isDirective(const Token& token)
{
	TokenType currType = token.getTokenType();

	return currType == TokenType::SECTION || currType == TokenType::EXTERN || currType == TokenType::GLOBAL || currType == TokenType::SKIP ||
		currType == TokenType::WORD || currType == TokenType::EQU;
}



void SecondPassParser::parseLine()
{
	if (currToken.getTokenType() == TokenType::SYMBOL)
	{
		//LABELA ADD
		if (nextToken.getTokenType() == TokenType::COLON)
		{
			//dodaj u asm LABELU
			//assembler.addLabel(currToken.text());
			//valjda nista 
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

void SecondPassParser::directiveAdd()
{
	const Token token = currToken;
	//pre ovoga je bio section

	currToken = tokens[currTokenID++];
	nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

	if (token.getTokenType() == TokenType::SECTION) {

		debug(TokenType::SYMBOL);

		//preksace se sekcija
		assembler.addSection(currToken.text());

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

	}
	else if (token.getTokenType() == TokenType::EXTERN) {

		debug(TokenType::SYMBOL);
		//dodaj global
		//assembler.addExtern(currToken.text());//mozda moze da se odmah zna sekcija tako sto ce asm da pamti u kojoj je sekciji
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		//vidi sta ces sa eksternom
		while (checkCurrentAndSkipNext(TokenType::COMMA))
		{
			debug(TokenType::SYMBOL);
			//assembler.addExtern(currToken.text());

			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}


	}
	else if (token.getTokenType() == TokenType::GLOBAL) {

		debug(TokenType::SYMBOL);
		//preksace
		//assembler.addGlobal(currToken.text());

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		while (checkCurrentAndSkipNext(TokenType::COMMA))
		{
			debug(TokenType::SYMBOL);
			//assembler.addGlobal(currToken.text());
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		}

	}
	else if (token.getTokenType() == TokenType::WORD) {

		debug({ TokenType::LITERAL, TokenType::SYMBOL });
		//ubaci token
		defineWord(currToken.text());


		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		while (checkCurrentAndSkipNext(TokenType::COMMA))
		{
			debug({ TokenType::LITERAL, TokenType::SYMBOL });
			//ubaci currtoken
			defineWord(currToken.text());
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

void SecondPassParser::instructionAdd()
{
	while (currToken.getTokenType() != TokenType::LINE_END)
	{
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
	}
}

void SecondPassParser::debug(TokenType tokenType)
{
	if (tokenType != currToken.getTokenType())
	{
		throw SecondParserException();
	}
}

void SecondPassParser::debug(vector<TokenType> list)
{
	for (auto& i : list)
	{
		if (i == currToken.getTokenType())
			return;
	}

	throw SecondParserException();
}

bool SecondPassParser::checkCurrentAndSkipNext(TokenType tokenType)
{
	if (tokenType == currToken.getTokenType()) {
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
		return 1;
	}
	return 0;
}

void SecondPassParser::defineWord(string name)
{
	if (currToken.getTokenType() == TokenType::SYMBOL)
		assembler.addWord(currToken.text());
	else
		assembler.addWord(std::stoi(currToken.text()));

}



