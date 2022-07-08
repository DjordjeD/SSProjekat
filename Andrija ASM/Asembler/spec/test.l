%top{
  
    #include "tokens.h"
    using namespace std;
    using namespace ss::lexer;
}

// dodajemo opciju da nam je povratna vrednost token
%option token-type=Token

// definisemo stanje za parsiranje komentara
%x STATE_COMMENT

global    ".global"
extern    ".extern"
section   ".section"
end       ".end"
skip      ".skip"
equ       ".equ"
data_def  (\.byte)|(\.word)
colon     ":"
comma     ","
plus      "+"
minus     "-"
star      "*"
dollar    "$"
lparen    "("
rparen    ")"
comment   ";"
line_end  \r?\n
blanks    [\t ]
literal   ([1-9][0-9]*)|(0[0-7]*)|(0x[0-9a-fA-F]+)

register \%(pc|(r[0-7](l|h)?))
symbol  [a-zA-Z][a-zA-Z_0-9]*

irrelevant .

%%

{line_end}   { return Token(TokenType::LINE_END, str()); }
{global}     { return Token(TokenType::GLOBAL, str());      }
{extern}     { return Token(TokenType::EXTERN, str());      }
{section}    { return Token(TokenType::SECTION, str());     }
{end}        { return Token(TokenType::END, str());         }
{data_def}   { return Token(TokenType::DATA_DEF, str());    }
{equ}        { return Token(TokenType::EQU, str());         }
{colon}      { return Token(TokenType::COLON, str());       }
{skip}       { return Token(TokenType::SKIP, str());        }
{comma}      { return Token(TokenType::COMMA, str());       }
{plus}       { return Token(TokenType::PLUS, str());        }
{minus}      { return Token(TokenType::MINUS, str());       }
{star}       { return Token(TokenType::STAR, str());        }
{dollar}     { return Token(TokenType::DOLLAR, str());      }
{lparen}     { return Token(TokenType::LPAREN, str());      }
{rparen}     { return Token(TokenType::RPAREN, str());      }
{register}   { return Token(TokenType::REGISTER, str());    }
{literal}    { return Token(TokenType::LITERAL, str());     }
{symbol}     { return Token(TokenType::SYMBOL, str());      }
<<EOF>>      { return Token(TokenType::FILE_END, str());    }
{comment}    { start(STATE_COMMENT);                        }
{blanks}     { }
{irrelevant} { }


<STATE_COMMENT>  {line_end}   { start(INITIAL); }
<STATE_COMMENT>  {irrelevant} { }

%% // Kraj definicije akcija