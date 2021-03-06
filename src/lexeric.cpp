// lexeric.cpp generated by reflex 2.1.4 from generator.l

#define REFLEX_VERSION "2.1.4"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_header_file         "lex.yy.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               Lexer
#define REFLEX_OPTION_outfile             "lexeric.cpp"
#define REFLEX_OPTION_token_type          Token

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "generator.l"


    #include "tokens.h"
    using namespace std;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class Lexer : public reflex::AbstractLexer<reflex::Matcher> {
 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  Lexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  static const int COMMENT = 1;
  virtual Token lex(void);
  Token lex(const reflex::Input& input)
  {
    in(input);
    return lex();
  }
  Token lex(const reflex::Input& input, std::ostream *os)
  {
    in(input);
    if (os)
      out(*os);
    return lex();
  }
};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 7 "generator.l"
// povratna vrednost token
#line 10 "generator.l"
// parsiranje komentara

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Token Lexer::lex(void)
{
  static const char *REGEX_INITIAL = "(?m)((?:\\r?\\n))|((?:(?:\\Q.global\\E)))|((?:(?:\\Q.extern\\E)))|((?:(?:\\Q.section\\E)))|((?:(?:\\Q.end\\E)))|((?:(?:\\Q.word\\E)))|((?:(?:\\Q.equ\\E)))|((?:(?:\\Q:\\E)))|((?:(?:\\Q.skip\\E)))|((?:(?:\\Q,\\E)))|((?:(?:\\Q+\\E)))|((?:(?:\\Q-\\E)))|((?:(?:\\Q*\\E)))|((?:(?:\\Q$\\E)))|((?:(?:\\Q%\\E)))|((?:(?:\\Q[\\E)))|((?:(?:\\Q]\\E)))|((?:(?:r[0-7]?)))|((?:(?:[1-9][0-9]*)|(?:0[0-7]*)|(?:0x[0-9A-Fa-f]+)))|((?:[A-Za-z][0-9A-Z_a-z]*))|((?:(?:\\Q#\\E)))|((?:[\\x09\\x20]))|((?:.))";
  static const reflex::Pattern PATTERN_INITIAL(REGEX_INITIAL);
  static const char *REGEX_COMMENT = "(?m)((?:\\r?\\n))|((?:.))";
  static const reflex::Pattern PATTERN_COMMENT(REGEX_COMMENT);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
    switch (start())
    {
      case INITIAL:
        matcher().pattern(PATTERN_INITIAL);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 61 "generator.l"
{ return Token(TokenType::FILE_END, str());    }
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule generator.l:41: {line_end} :
#line 41 "generator.l"
{ return Token(TokenType::LINE_END, str()); }
            break;
          case 2: // rule generator.l:42: {global} :
#line 42 "generator.l"
{ return Token(TokenType::GLOBAL, str());      }
            break;
          case 3: // rule generator.l:43: {extern} :
#line 43 "generator.l"
{ return Token(TokenType::EXTERN, str());      }
            break;
          case 4: // rule generator.l:44: {section} :
#line 44 "generator.l"
{ return Token(TokenType::SECTION, str());     }
            break;
          case 5: // rule generator.l:45: {end} :
#line 45 "generator.l"
{ return Token(TokenType::END, str());         }
            break;
          case 6: // rule generator.l:46: {word} :
#line 46 "generator.l"
{ return Token(TokenType::WORD, str());    }
            break;
          case 7: // rule generator.l:47: {equ} :
#line 47 "generator.l"
{ return Token(TokenType::EQU, str());         }
            break;
          case 8: // rule generator.l:48: {colon} :
#line 48 "generator.l"
{ return Token(TokenType::COLON, str());       }
            break;
          case 9: // rule generator.l:49: {skip} :
#line 49 "generator.l"
{ return Token(TokenType::SKIP, str());        }
            break;
          case 10: // rule generator.l:50: {comma} :
#line 50 "generator.l"
{ return Token(TokenType::COMMA, str());       }
            break;
          case 11: // rule generator.l:51: {plus} :
#line 51 "generator.l"
{ return Token(TokenType::PLUS, str());        }
            break;
          case 12: // rule generator.l:52: {minus} :
#line 52 "generator.l"
{ return Token(TokenType::MINUS, str());       }
            break;
          case 13: // rule generator.l:53: {star} :
#line 53 "generator.l"
{ return Token(TokenType::STAR, str());        }
            break;
          case 14: // rule generator.l:54: {dollar} :
#line 54 "generator.l"
{ return Token(TokenType::DOLLAR, str());      }
            break;
          case 15: // rule generator.l:55: {percentage} :
#line 55 "generator.l"
{return Token(TokenType::PERCENTAGE, str()); }
            break;
          case 16: // rule generator.l:56: {startparen} :
#line 56 "generator.l"
{ return Token(TokenType::STARTPAREN, str());      }
            break;
          case 17: // rule generator.l:57: {endparen} :
#line 57 "generator.l"
{ return Token(TokenType::ENDPAREN, str());      }
            break;
          case 18: // rule generator.l:58: {register} :
#line 58 "generator.l"
{ return Token(TokenType::REGISTER, str());    }
            break;
          case 19: // rule generator.l:59: {literal} :
#line 59 "generator.l"
{ return Token(TokenType::LITERAL, str());     }
            break;
          case 20: // rule generator.l:60: {symbol} :
#line 60 "generator.l"
{ return Token(TokenType::SYMBOL, str());      }
            break;
          case 21: // rule generator.l:62: {comment} :
#line 62 "generator.l"
{ start(COMMENT);                        }
            break;
          case 22: // rule generator.l:63: {blanks} :
#line 63 "generator.l"
{ }
            break;
          case 23: // rule generator.l:64: {ignore} :
#line 64 "generator.l"
{ }


            break;
        }
        break;
      case COMMENT:
        matcher().pattern(PATTERN_COMMENT);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 61 "generator.l"
{ return Token(TokenType::FILE_END, str());    }
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule generator.l:67: {line_end} :
#line 67 "generator.l"
{ start(INITIAL); return Token(TokenType::LINE_END, str()); }
            break;
          case 2: // rule generator.l:68: {ignore} :
#line 68 "generator.l"
{ }

            break;
        }
        break;
      default:
        start(0);
    }
  }
}
