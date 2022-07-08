#include "tokens.h"

using namespace ss::lexer;

std::ostream& ss::lexer::operator<<(std::ostream& out, const TokenType tokenType)
{
	switch (tokenType)
	{
	case TokenType::GLOBAL: out << "GLOBAL"; break;
	case TokenType::EXTERN: out << "EXTERN"; break;
	case TokenType::SECTION: out << "SECTION"; break;
	case TokenType::END: out << "END"; break;
	case TokenType::SKIP: out << "SKIP"; break;
	case TokenType::EQU: out << "EQU"; break;
	case TokenType::DATA_DEF: out << "DATA_DEF"; break;
	case TokenType::COLON: out << "COLON"; break;
	case TokenType::COMMA: out << "COMMA"; break;
	case TokenType::PLUS: out << "PLUS"; break;
	case TokenType::MINUS: out << "MINUS"; break;
	case TokenType::STAR: out << "STAR"; break;
	case TokenType::DOLLAR: out << "DOLLAR"; break;
	case TokenType::LPAREN: out << "LPAREN"; break;
	case TokenType::RPAREN: out << "RPAREN"; break;
	case TokenType::SEMICOLN: out << "SEMICOLN"; break;
	case TokenType::LINE_END: out << "LINE_END"; break;
	case TokenType::LITERAL: out << "LITERAL"; break;
	case TokenType::REGISTER: out << "REGISTER"; break;
	case TokenType::SYMBOL: out << "SYMBOL"; break;
	case TokenType::FILE_END: out << "FILE_END"; break;
	case TokenType::DUMMY: out << "DUMMY"; break;
	}

	return out;
}