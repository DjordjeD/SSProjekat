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
	//assembler.printSectionMap();
	//assembler.createTxtFile("");
	assembler.createBinaryFile("");
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
	//cout << "usao u instruction ADD !" << endl;
	//cout << currToken.text() << endl;
	string instructionText = currToken.text();


	if (instructionText == "halt" || instructionText == "iret" || instructionText == "ret")
	{
		vector <char> dataVector;
		dataVector.push_back(assembler.instructionBinary(instructionText));
		assembler.updateSectionMap(dataVector, 1);
	}
	else if (instructionText == "int" || instructionText == "not" || instructionText == "pop" || instructionText == "push")//call fali mozda
	{
		vector <char> dataVector;
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		int registerNum;
		registerNum = currToken.text().at(1) - '0';
		if (instructionText == "int" || instructionText == "not") {
			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back((registerNum << 4) + 15);
			assembler.updateSectionMap(dataVector, 2); // uvecava za 2 , trenutnu sekciju ubacuje offset = location counter, datavector u data
		}
		else if (instructionText == "pop")
		{
			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back((registerNum << 4) + 6);
			dataVector.push_back(0x42);
			assembler.updateSectionMap(dataVector, 3);
		}
		else if (instructionText == "push")
		{
			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back((registerNum << 4) + 6);
			dataVector.push_back(0x12);
			assembler.updateSectionMap(dataVector, 3);
		}

	}
	else if (instructionText == "jmp" || instructionText == "jeq" || instructionText == "jne" || instructionText == "jgt" || instructionText == "call")
	{
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		int incLoc = -1000;

		if (currToken.getTokenType() == TokenType::SYMBOL) {
			incLoc = 5; //jmp simbol

			vector <char> dataVector;
			int regDesc = 0xFF;
			int adrMode = 0;

			int value = assembler.absoluteSymbolValue(tokens[currTokenID - 1].text());
			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back(regDesc);
			dataVector.push_back(adrMode);
			dataVector.push_back((value >> 8) & 0xff);
			dataVector.push_back(value & 0xff);

			assembler.updateSectionMap(dataVector, incLoc);

		}
		else if (currToken.getTokenType() == TokenType::LITERAL)
		{
			incLoc = 5; //jmp Label

			vector <char> dataVector;
			int regDesc = 0xFF;
			int adrMode = 0;
			int value = stoi(tokens[currTokenID - 1].text(), 0, 16);

			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back(regDesc);
			dataVector.push_back(adrMode);
			dataVector.push_back((value >> 8) & 0xff);
			dataVector.push_back(value & 0xff);

			assembler.updateSectionMap(dataVector, incLoc);
		}

		else if (checkCurrentAndSkipNext(TokenType::PERCENTAGE)) {
			//jmp %label pcrelative
			incLoc = 5;
			/*currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();*/

			vector <char> dataVector;
			int regDesc = 0xF7;
			int adrMode = 0x05;

			int value = assembler.pcRelativeSymbolValue(currToken.text());
			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back(regDesc);
			dataVector.push_back(adrMode);
			dataVector.push_back((value >> 8) & 0xff);
			dataVector.push_back(value & 0xff);

			assembler.updateSectionMap(dataVector, incLoc);

		}
		else if (checkCurrentAndSkipNext(TokenType::STAR)) { //jmp *label
			//kad je star prvi kod jmp
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
			debug({ TokenType::REGISTER, TokenType::STARTPAREN, TokenType::LITERAL, TokenType::SYMBOL });


			switch (currToken.getTokenType())
			{
			case TokenType::REGISTER: {
				//regdir
				incLoc = 3;
				vector <char> dataVector;
				int regDesc = 0xF0 + (currToken.text().at(1) - '0');
				int adrMode = 0x01;

				int value = assembler.pcRelativeSymbolValue(currToken.text());
				dataVector.push_back(assembler.instructionBinary(instructionText));
				dataVector.push_back(regDesc);
				dataVector.push_back(adrMode);

				assembler.updateSectionMap(dataVector, incLoc);
			} break;
			case TokenType::STARTPAREN: {
				//uzmi sledeci jmp *[ri]
				currToken = tokens[currTokenID++];
				nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

				if (checkCurrentAndSkipNext(TokenType::REGISTER)) {
					//zasada regind

					currToken = tokens[currTokenID++];
					nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

					if (checkCurrentAndSkipNext(TokenType::PLUS))//ako nadje plus ima i displacement
					{
						//uzmi displacement
						currToken = tokens[currTokenID++];
						nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

						debug({ TokenType::LITERAL, TokenType::SYMBOL });
						incLoc = 5;
						vector <char> dataVector;
						int regDesc = 0xF0 + (tokens[currTokenID - 1].text().at(1) - '0');
						int adrMode = 0x03;
						int value;



						if (currToken.getTokenType() == TokenType::LITERAL)
						{
							value = stoi(currToken.text(), 0, 16);
						}
						else
						{
							//symbol
							value = assembler.absoluteSymbolValue(currToken.text());
						}


						dataVector.push_back(assembler.instructionBinary(instructionText));
						dataVector.push_back(regDesc);
						dataVector.push_back(adrMode);
						dataVector.push_back((value >> 8) & 0xff);
						dataVector.push_back(value & 0xff);

						assembler.updateSectionMap(dataVector, incLoc);

						//dok ne uzmes desnu zagradu
						while (currToken.getTokenType() != TokenType::ENDPAREN)
						{
							currToken = tokens[currTokenID++];
							nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
						}


					}
					else { // ovde je zagrada onda  (regind)

						incLoc = 3;
						vector <char> dataVector;
						int regDesc = 0xF0 + (tokens[currTokenID - 1].text().at(1) - '0');
						int adrMode = 0x02;

						//int value = assembler.pcRelativeSymbolValue(currToken.text());
						dataVector.push_back(assembler.instructionBinary(instructionText));
						dataVector.push_back(regDesc);
						dataVector.push_back(adrMode);

						assembler.updateSectionMap(dataVector, incLoc);
					}

				}
				else
				{
					throw SecondParserException();
				}



			} break;
			case TokenType::LITERAL: {

				incLoc = 3; //jmp *Label

				vector <char> dataVector;
				int regDesc = 0xFF;
				int adrMode = 0x04;
				int value = stoi(currToken.text(), 0, 16);

				dataVector.push_back(assembler.instructionBinary(instructionText));
				dataVector.push_back(regDesc);
				dataVector.push_back(adrMode);
				dataVector.push_back((value >> 8) & 0xff);
				dataVector.push_back(value & 0xff);

				assembler.updateSectionMap(dataVector, incLoc);

			} break;
			case TokenType::SYMBOL: {

				incLoc = 3; //jmp *symbol

				vector <char> dataVector;
				int regDesc = 0xFF;
				int adrMode = 0x04;
				int value = assembler.absoluteSymbolValue(currToken.text());

				dataVector.push_back(assembler.instructionBinary(instructionText));
				dataVector.push_back(regDesc);
				dataVector.push_back(adrMode);
				dataVector.push_back((value >> 8) & 0xff);
				dataVector.push_back(value & 0xff);

				assembler.updateSectionMap(dataVector, incLoc);

			};  break;


			}
		}

		//assembler.updateLocationCounter(incLoc);

	}
	else if (instructionText == "ldr" || instructionText == "str")
	{
		int incLoc = -1000;// za koliko se povecava
		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		//if (!checkCurrentAndSkipNext(TokenType::REGISTER)) throw  SecondParserException(); // losa instrukcija valjda

		if (currToken.getTokenType() != TokenType::REGISTER) throw  SecondParserException();

		int regDesc = currToken.text().at(1) - '0';
		regDesc <<= 4; // ovo puni source

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		//uzima zarez
		debug(TokenType::COMMA);

		currToken = tokens[currTokenID++];
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		debug({ TokenType::REGISTER, TokenType::STARTPAREN, TokenType::LITERAL, TokenType::SYMBOL,TokenType::PERCENTAGE, TokenType::DOLLAR });

		switch (currToken.getTokenType())
		{
		case TokenType::REGISTER: {
			//regdir
			incLoc = 3;
			vector <char> dataVector;
			regDesc += (currToken.text().at(1) - '0'); //puni desni registar zbog toga sto sam siftovao gore
			int adrMode = 0x01;

			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back(regDesc);
			dataVector.push_back(adrMode);

			assembler.updateSectionMap(dataVector, incLoc);
		}break;
		case TokenType::DOLLAR: {
			//absolute ld ri, $label
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
			debug({ TokenType::LITERAL, TokenType::SYMBOL });

			incLoc = 5;
			vector <char> dataVector;
			regDesc += 0xF;
			int adrMode = 0;
			int value;



			if (currToken.getTokenType() == TokenType::LITERAL)
			{
				value = stoi(currToken.text(), 0, 16);
			}
			else
			{
				//symbol
				value = assembler.absoluteSymbolValue(currToken.text());
			}


			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back(regDesc);
			dataVector.push_back(adrMode);
			dataVector.push_back((value >> 8) & 0xff);
			dataVector.push_back(value & 0xff);


			assembler.updateSectionMap(dataVector, incLoc);

		}break;
		case TokenType::PERCENTAGE: {
			//pcrelative ld ri, %label
			incLoc = 5;
			currToken = tokens[currTokenID++];
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

			vector <char> dataVector;
			regDesc += 0x7;
			int adrMode = 0x03;

			int value = assembler.pcRelativeSymbolValue(currToken.text());
			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back(regDesc);
			dataVector.push_back(adrMode);
			dataVector.push_back((value >> 8) & 0xff);
			dataVector.push_back(value & 0xff);

			assembler.updateSectionMap(dataVector, incLoc);

		}break;

		case TokenType::STARTPAREN: {

			/*		currToken = tokens[currTokenID++];
					nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
					currToken = tokens[currTokenID++];
					nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();*/

			currToken = tokens[currTokenID++];// registar 
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

			regDesc += (currToken.text().at(1) - '0');

			currToken = tokens[currTokenID++];// plus ili endparen
			nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

			if (checkCurrentAndSkipNext(TokenType::PLUS))
			{
				//ld ri, [rj + label - 4]

				debug({ TokenType::LITERAL, TokenType::SYMBOL });
				incLoc = 5;
				vector <char> dataVector;
				int adrMode = 0x03;
				int value;



				if (currToken.getTokenType() == TokenType::LITERAL)
				{
					value = stoi(currToken.text(), 0, 16);
				}
				else
				{
					//symbol
					value = assembler.absoluteSymbolValue(currToken.text());
				}


				dataVector.push_back(assembler.instructionBinary(instructionText));
				dataVector.push_back(regDesc);
				dataVector.push_back(adrMode);
				dataVector.push_back((value >> 8) & 0xff);
				dataVector.push_back(value & 0xff);

				assembler.updateSectionMap(dataVector, incLoc);



				while (currToken.getTokenType() != TokenType::ENDPAREN)
				{
					currToken = tokens[currTokenID++];
					nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
				}
			}
			else if (currToken.getTokenType() == TokenType::ENDPAREN)
			{
				incLoc = 3; // regind  ld ri, [rj]

				currToken = tokens[currTokenID++];
				nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

				vector <char> dataVector;
				int adrMode = 0x02;

				int value = assembler.pcRelativeSymbolValue(currToken.text());
				dataVector.push_back(assembler.instructionBinary(instructionText));
				dataVector.push_back(regDesc);
				dataVector.push_back(adrMode);

				assembler.updateSectionMap(dataVector, incLoc);
			}

		}break;

		case TokenType::LITERAL:
		case TokenType::SYMBOL: {

			debug({ TokenType::LITERAL, TokenType::SYMBOL });
			incLoc = 5;
			vector <char> dataVector;
			regDesc += 0xF;
			int adrMode = 0x04;
			int value;



			if (currToken.getTokenType() == TokenType::LITERAL)
			{
				value = stoi(currToken.text(), 0, 16);
			}
			else
			{
				//symbol
				value = assembler.absoluteSymbolValue(currToken.text());
			}


			dataVector.push_back(assembler.instructionBinary(instructionText));
			dataVector.push_back(regDesc);
			dataVector.push_back(adrMode);
			dataVector.push_back((value >> 8) & 0xff);
			dataVector.push_back(value & 0xff);

			assembler.updateSectionMap(dataVector, incLoc);

		};  break; //memdir oba ova

		}

		//assembler.updateLocationCounter(incLoc);

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
		vector<char> dataVector;

		currToken = tokens[currTokenID++];//registar
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		int regDesc = currToken.text().at(1) - '0';
		//checkCurrentAndSkipNext(TokenType::REGISTER);
		// 
		//checkCurrentAndSkipNext(TokenType::COMMA);

		currToken = tokens[currTokenID++];//zarez
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		currToken = tokens[currTokenID++];//registar
		nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();

		regDesc <<= 4;

		regDesc += currToken.text().at(1) - '0';

		dataVector.push_back(assembler.instructionBinary(instructionText));
		dataVector.push_back(regDesc);

		assembler.updateSectionMap(dataVector, 2);

	}


	//da se dobije lineEnd
	currToken = tokens[currTokenID++];
	nextToken = (currTokenID < tokens.size()) ? tokens[currTokenID] : Token();
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



