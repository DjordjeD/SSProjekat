#ifndef _PARSER_H_
#define _PARSER_H_

#include <algorithm>
#include <vector>

#include "tokens.h"
#include "assembler.h"

namespace ss {
	namespace parser {

		class ParserException : public std::exception {
			const char* what() const noexcept {
				return "Error during parsing!";
			}
		};

		using namespace ss::lexer;
		using namespace ss::assembler;
		class Parser
		{
		public:
			void parse();
			Parser(std::vector<Token>& tokens) :_tokens(tokens), _curr_idx(0) {}
		private:
			int   _curr_idx;
			Token _curr_token;
			Token _lookahead_token;

			std::vector<Token> _tokens;
			Assembler _assembler;

			void start();
			void labelDef();

			void directiveDef();
			void directiveDefSection();
			void directiveDefGlobal();
			void directiveDefExtern();
			void directiveDefDataDef(Token data_token);
			void directiveDefSkip();
			void directiveDefEqu();

			void instructionDef();

			void expr();
			void arg();
			void argRegisterDirect();
			void argRegisterIndirect();
			void argRegisterIndirectDispl();
			void argMem();
			void argImmediate();


			void next_token();
			bool accept(TokenType);
			void expect(std::vector<TokenType>);
			void expect(TokenType);

			bool isDirectiveDef(const Token& token);
			bool isArg(const Token& token);


			void dataDefAction(int size);
		};
	}
}

#endif