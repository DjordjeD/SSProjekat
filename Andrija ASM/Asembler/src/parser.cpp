#include <iostream>

#include "parser.h"

using namespace ss::parser;
using namespace ss::lexer;


void Parser::parse()
{
	next_token();

	int current_line = 0;

	try {
		while (_curr_token.tokenType() != TokenType::FILE_END &&
			   _curr_token.tokenType() != TokenType::END)
		{
			current_line++;
			start();
		}
	}
	catch (ParserException parserException)
	{
		std::cout << "Error at line: " << current_line << ". Parser failed";
	}
	catch (AssemblerException assemblerException)
	{
		std::cout << "Error at line: " << current_line << "." << assemblerException.what();
	}
}

void Parser::start()
{
	// Ovo je za definiciju labele.
	if (_curr_token.tokenType() == TokenType::SYMBOL)
	{
		labelDef();
	}

	if (_curr_token.tokenType() == TokenType::SYMBOL)
	{
		instructionDef();
	}
	else if (isDirectiveDef(_curr_token))
	{
		directiveDef();
	}

	expect({ TokenType::LINE_END, TokenType::FILE_END });
	next_token();
}

void Parser::labelDef()
{
	// SYMBOL COLON STAGOD
	// ---^
	// _curr_token.tokentype == SYMBOL
	if (_lookahead_token.tokenType() == TokenType::COLON)
	{
		// pozivanje akcije asemblera
		_assembler.labelDef(_curr_token.text());

		next_token();
		next_token();
	}
}

void Parser::directiveDef()
{
	const Token token = _curr_token;

	// SECTION NESTO
	// ----^
	next_token();
	switch (token.tokenType())
	{
		case TokenType::SECTION: directiveDefSection();       break;
		case TokenType::GLOBAL: directiveDefGlobal();	      break;
		case TokenType::EXTERN: directiveDefExtern();         break;
		case TokenType::DATA_DEF: directiveDefDataDef(token); break;
		case TokenType::SKIP: directiveDefSkip();             break;
		case TokenType::EQU: directiveDefEqu();               break;
	}
}

void ss::parser::Parser::directiveDefSection()
{
	expect(TokenType::SYMBOL);

	_assembler.sectionDef(_curr_token.text());

	next_token();
}

void ss::parser::Parser::directiveDefGlobal()
{
	expect(TokenType::SYMBOL);
	_assembler.globalDef(_curr_token.text());
	
	next_token();
	// potencijalno je _curr_token.tokenType() == TokenType::COMMA
	while (accept(TokenType::COMMA))
	{
		expect(TokenType::SYMBOL);
		_assembler.globalDef(_curr_token.text());

		next_token();
	}
}

void ss::parser::Parser::directiveDefExtern()
{
	expect(TokenType::SYMBOL);
	_assembler.globalDef(_curr_token.text());

	next_token();
	// potencijalno je _curr_token.tokenType() == TokenType::COMMA
	while (accept(TokenType::COMMA))
	{
		expect(TokenType::SYMBOL);
		_assembler.globalDef(_curr_token.text());

		next_token();
	}
}

void ss::parser::Parser::directiveDefDataDef(Token data_token)
{
	int size = (data_token.text() == ".word") ? 2 : 1;
	expect({ TokenType::SYMBOL, TokenType::LITERAL });
	dataDefAction(size);

	next_token();
	while (accept(TokenType::COMMA))
	{
		expect({ TokenType::SYMBOL, TokenType::LITERAL });
		dataDefAction(size);
		next_token();
	}
}

void ss::parser::Parser::directiveDefSkip()
{
	expect(TokenType::LITERAL);

	_assembler.skipDef(std::stoi(_curr_token.text()));

	next_token();
}

void ss::parser::Parser::directiveDefEqu()
{
	next_token();
	expect(TokenType::COMMA);
	next_token();

	expr();
}

void ss::parser::Parser::instructionDef()
{
	expect(TokenType::SYMBOL);

	next_token();

	if (isArg(_curr_token))
	{
		arg();

		if (accept(TokenType::COMMA))
		{
			if (!isArg(_curr_token))
			{
				// TODO: greska
			}

			arg();

		}
	}
}

void ss::parser::Parser::expr()
{
	expect({ TokenType::SYMBOL, TokenType::LITERAL });
	
	next_token();
	while (accept(TokenType::PLUS) || accept(TokenType::MINUS))
	{
		expect({ TokenType::SYMBOL, TokenType::LITERAL });
		next_token();
	}
}

void ss::parser::Parser::arg()
{
	if (accept(TokenType::DOLLAR))
	{
		// parsiranje dolara
		argImmediate();
	}
	else if (accept(TokenType::STAR))
	{
		// TODO: obeleziti da je samo u pianju STAR/ da se javio star
	}

	expect({ TokenType::REGISTER, TokenType::LPAREN, TokenType::LITERAL, TokenType::SYMBOL });
	switch (_curr_token.tokenType())
	{
		case TokenType::REGISTER: argRegisterDirect();   break;
		case TokenType::LPAREN:   argRegisterIndirect(); break;
		case TokenType::LITERAL:  
		case TokenType::SYMBOL:   argMem();              break;
	}
}

void ss::parser::Parser::argRegisterDirect()
{
	next_token();
}

void ss::parser::Parser::argRegisterIndirect()
{
	next_token();
	expect(TokenType::REGISTER);

	next_token();
	expect(TokenType::RPAREN);
}

void ss::parser::Parser::argRegisterIndirectDispl()
{
	Token displ = _curr_token;

	next_token();
	expect(TokenType::LPAREN);

	next_token();
	expect(TokenType::REGISTER);

	next_token();
	expect(TokenType::RPAREN);

	next_token();
}

void ss::parser::Parser::argMem()
{
	if (_lookahead_token.tokenType() == TokenType::LPAREN)
	{
		argRegisterIndirectDispl();
	}
	else
	{
		// Memorijsko direktno
		next_token();
	}
}

void ss::parser::Parser::argImmediate()
{
	expect({ TokenType::SYMBOL, TokenType::LITERAL });
	next_token();
}

void ss::parser::Parser::next_token()
{
	_curr_token = _tokens[_curr_idx++];
	_lookahead_token = (_curr_idx < _tokens.size()) ? _tokens[_curr_idx] : Token();
}

bool ss::parser::Parser::accept(TokenType token_type)
{
	if (_curr_token.tokenType() == token_type)
	{
		next_token();
		return true;
	}

	return false;
}

void ss::parser::Parser::expect(std::vector<TokenType> token_types)
{
	for (auto token_type : token_types)
	{
		if (token_type == _curr_token.tokenType())
			return;
	}

	throw ParserException();
}

void ss::parser::Parser::expect(TokenType token_type)
{
	if (token_type != _curr_token.tokenType())
	{
		throw ParserException();
	}
}

bool Parser::isDirectiveDef(const Token& token)
{
	TokenType token_type = token.tokenType();

	return token_type == TokenType::SECTION || token_type == TokenType::GLOBAL || token_type == TokenType::EXTERN || 
		   token_type == TokenType::DATA_DEF || token_type == TokenType::SKIP || token_type == TokenType::EQU;
}

bool ss::parser::Parser::isArg(const Token& token)
{
	TokenType token_type = token.tokenType();
	return token_type == TokenType::DOLLAR || token_type == TokenType::REGISTER ||
		   token_type == TokenType::LPAREN || token_type == TokenType::LITERAL ||
		   token_type == TokenType::SYMBOL || token_type == TokenType::STAR;
}

void ss::parser::Parser::dataDefAction(int size)
{
	if (_curr_token.tokenType() == TokenType::SYMBOL)
		_assembler.dataDef(size, _curr_token.text());
	else
		_assembler.dataDef(size, std::stoi(_curr_token.text()));
}
