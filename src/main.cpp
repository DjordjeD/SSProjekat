#include <iostream>
#include <fstream>
#include <vector>

#include "lex.yy.h"
#include "tokens.h"
#include "parseric.h"
#include "secondPassAsm.h"
#include "secondPassParser.h"


#include<tuple>


using namespace std;


int main(int argc, char** argv)
{

	vector<Token> tokens;
	string inputFile;
	string outputFile;
	string current = argv[1];

	if (current == "-o")
	{
		outputFile = argv[2];
		inputFile = argv[3];

	}
	else
	{
		cout << "Error: output doesnt exists" << endl;
	}

	std::ifstream file(inputFile);

	Lexer lexer(file);
	Token token;

	while ((token.getTokenType() != TokenType::FILE_END))
	{
		token = lexer.lex();
		tokens.push_back(token);

	}
	//int i = 0;


	Parser parser(tokens);

	SecondPassAsm secondPassAsm = parser.parse();

	SecondPassParser secondPassParser(tokens, secondPassAsm);

	secondPassParser.outputFile = outputFile;

	secondPassParser.parse();


	return 0;

}