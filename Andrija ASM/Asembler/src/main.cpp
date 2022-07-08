#include <iostream>
#include <fstream>
#include <vector>

#include "lexer.h"
#include "parser.h"
#include "tokens.h"


using namespace ss::lexer;
using namespace ss::parser;

/*
* ================================ GRAMATIKA ===================================
* Start ::= [[LabelaDef] DirectiveDef|InstructionDef] (LINE_END|FILE_END)
* LabelaDef ::= SYMBOL COLON
* DrectiveDef ::= SECTION SYMBOL 
                | GLOBAL SYMBOL{,SYMBOL} 
				| EXTERN SYMBOL{,SYMBOL}
				| DATA_DEF (SYMBOL|LITERAL){,(SYMBOL|LITERAL)}
				| SKIP LITERAL
				| EQU SYMBOL COMMA Expr
* Expr ::= (SYMBOL|LITERAL) {(PLUS|MINUS) (SYMBOL|LITERAL)}
* InstructionDef ::= SYMBOL [Arg{,Arg}{1}]
* Arg ::= DOLLAR (SYMBOL | LITERAL)
*       | REGISTER
*		| LPAREN REGISTER RPAREN
*		| (SYMBOL|LTERAL) LPAREN REGISTER RPAREN
*		| LITERAL
*		| SYMBOL
*		| STAR REGISTER
*		| STAR LPAREN REGISTER RPAREN
*		| STAR (SYMBOL|LITERAL) REGISTER
*		| STAR LITERAL
*		| STAR SYMBOL
*/
int main(int argc, char** argv)
{
	std::ifstream file(argv[1]);
	Lexer lexer(file);
	Token token = lexer.lex();

	std::vector<Token> tokens;
	for(;token.tokenType() != TokenType::FILE_END; token = lexer.lex())
	{
		//std::cout << token.tokenType() << std::endl;
		tokens.push_back(token);
	}

	Parser parser(tokens);

	parser.parse();

	return 0;
}