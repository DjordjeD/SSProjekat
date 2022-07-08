#ifndef _TOKENS_H_
#define _TOKENS_H_

#include <ostream>
#include <string>

namespace ss {
	namespace lexer {

		enum class TokenType
		{
			GLOBAL, EXTERN, SECTION, END, SKIP, EQU, DATA_DEF, COLON, COMMA, PLUS,
			MINUS, STAR, DOLLAR, LPAREN, RPAREN, SEMICOLN, LINE_END, LITERAL,REGISTER,
			SYMBOL, FILE_END, DUMMY
		};

		std::ostream& operator<<(std::ostream& out, const TokenType tokenType);

		class Token
		{
		private:
			TokenType _tokenType;
			std::string _text;
		public:
			Token(TokenType tokenType = TokenType::DUMMY, std::string text = "") :
				_tokenType(tokenType),
				_text(text) {}

			TokenType tokenType() const { return _tokenType; }
			std::string text() const { return _text; }
		};
	}
}


#endif
