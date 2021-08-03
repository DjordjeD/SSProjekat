#include "tokens.h"


ostream& operator<<(std::ostream& out, const TokenType tokenType)
{
	switch (tokenType)
	{
	case TokenType::GLOBAL: out << "GLOBAL"; break;
	case TokenType::EXTERN: out << "EXTERN"; break;
	case TokenType::SECTION: out << "SECTION"; break;
	case TokenType::END: out << "END"; break;
	case TokenType::SKIP: out << "SKIP"; break;
	case TokenType::EQU: out << "EQU"; break;
	case TokenType::WORD: out << "WORD"; break;
	case TokenType::COLON: out << "COLON"; break;
	case TokenType::COMMA: out << "COMMA"; break;
	case TokenType::PLUS: out << "PLUS"; break;
	case TokenType::MINUS: out << "MINUS"; break;
	case TokenType::STAR: out << "STAR"; break;
	case TokenType::DOLLAR: out << "DOLLAR"; break;
	case TokenType::STARTPAREN: out << "STARTPAREN"; break;
	case TokenType::ENDPAREN: out << "ENDPAREN"; break;
	case TokenType::SEMICOLN: out << "SEMICOLN"; break;
	case TokenType::LINE_END: out << "LINE_END"; break;
	case TokenType::LITERAL: out << "LITERAL"; break;
	case TokenType::REGISTER: out << "REGISTER"; break;
	case TokenType::SYMBOL: out << "SYMBOL"; break;
	case TokenType::FILE_END: out << "FILE_END"; break;
	case TokenType::RANDOM: out << "RANDOM"; break;
	}

	return out;
}
