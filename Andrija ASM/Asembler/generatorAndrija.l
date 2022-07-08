%top{
  
    #include "tokens.h"
    using namespace std;
}

// povratna vrednost token
%option token-type=Token

// parsiranje komentara
%x COMMENT

global    ".global"
extern    ".extern"
section   ".section"
end       ".end"
skip      ".skip"
equ       ".equ"
word   	  ".word"
colon     ":"
comma     ","
plus      "+"
minus     "-"
star      "*"
dollar    "$"
startparen "("
endparen    ")"
comment   ";"
line_end  \r?\n
blanks    [\t ]
literal   ([1-9][0-9]*)|(0[0-7]*)|(0x[0-9a-fA-F]+)

register (r[0-7]?)
symbol  [a-zA-Z][a-zA-Z_0-9]*

ignore .

%%

{line_end}   { return Token(TokenType::LINE_END, str()); }
{global}     { return Token(TokenType::GLOBAL, str());      }
{extern}     { return Token(TokenType::EXTERN, str());      }
{section}    { return Token(TokenType::SECTION, str());     }
{end}        { return Token(TokenType::END, str());         }
{word}   { return Token(TokenType::WORD, str());    }
{equ}        { return Token(TokenType::EQU, str());         }
{colon}      { return Token(TokenType::COLON, str());       }
{skip}       { return Token(TokenType::SKIP, str());        }
{comma}      { return Token(TokenType::COMMA, str());       }
{plus}       { return Token(TokenType::PLUS, str());        }
{minus}      { return Token(TokenType::MINUS, str());       }
{star}       { return Token(TokenType::STAR, str());        }
{dollar}     { return Token(TokenType::DOLLAR, str());      }
{startparen}     { return Token(TokenType::STARTPAREN, str());      }
{endparen}     { return Token(TokenType::ENDPAREN, str());      }
{register}   { return Token(TokenType::REGISTER, str());    }
{literal}    { return Token(TokenType::LITERAL, str());     }
{symbol}     { return Token(TokenType::SYMBOL, str());      }
<<EOF>>      { return Token(TokenType::FILE_END, str());    }
{comment}    { start(COMMENT);                        }
{blanks}     { }
{ignore} { }


<COMMENT>  {line_end}   { start(INITIAL); }
<COMMENT>  {ignore} { }

%% 