#include <iostream>
#include <fstream>
#include <vector>

#include "lex.yy.h"
#include "tokens.h"
#include "parseric.h"

/*
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

using namespace std;


int main(int argc, char** argv)
{

	vector<Token> tokens;

	std::ifstream file(argv[1]);
	Lexer lexer(file);
	Token token;

	
	

	while ((token.getTokenType() != TokenType::FILE_END))
	{
		token = lexer.lex();
		tokens.push_back(token);

	}
	//int i = 0;



	//potencijalan problem sa komentarima

	Parser parser(tokens);

	parser.parse();

//	for (auto const& value : tokens)
//{
//	cout << value.getTokenType() << " " << value.text() << endl;
//}


	return 0;

}