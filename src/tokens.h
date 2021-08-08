#ifndef _TOKENS_H_
#define _TOKENS_H_

#include <ostream>
#include <string>

	using namespace std;


		enum class TokenType
		{
		//public:
			GLOBAL, EXTERN, SECTION, END, SKIP, EQU, WORD, COLON, COMMA, PLUS,
			MINUS, STAR, DOLLAR,PERCENTAGE, STARTPAREN, ENDPAREN, SEMICOLN, LINE_END, LITERAL,  REGISTER,
			SYMBOL, FILE_END, RANDOM
		};

		ostream& operator<<(std::ostream& out, const TokenType tokenType);

		class Token
		{
		private:
			TokenType tokenType;
			string tokenText;
		public:
			Token(TokenType tokenType = TokenType::RANDOM, string text = "") :
				tokenType(tokenType),
				tokenText(text) {}

			TokenType getTokenType() const { return tokenType; }
			string text() const { return tokenText; }
		};


#endif
