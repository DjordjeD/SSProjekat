#include <iostream>
#include <fstream>
#include <vector>

#include "lex.yy.h"
#include "tokens.h"
#include "parseric.h"
#include "secondPassAsm.h"
#include "secondPassParser.h"


#include<tuple>


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

	//Parser parser(tokens);

	//SecondPassAsm secondPassAsm = parser.parse();

	//SecondPassParser secondPassParser(tokens, secondPassAsm);

	//secondPassParser.parse();

	//for (auto const& value : tokens)
	//{
	//	cout << value.getTokenType() << " " << value.text() << endl;
	//}

	vector<vector<int>> data;

	vector<int> temp;
	temp.push_back(0xB0);
	temp.push_back(0xB1);
	temp.push_back(0xB2);
	temp.push_back(0xB3);

	data.push_back(temp);


	temp.push_back(0xA0);
	temp.push_back(0xA1);
	temp.push_back(0xA3);
	temp.push_back(0xA4);

	data.push_back(temp);

	vector<int> offsets;
	offsets.push_back(1);
	offsets.push_back(2);

	cout << data.size() << endl;
	cout << data[0].size() << endl;
	cout << offsets.size() << endl;

	for (size_t i = 0; i < offsets.size(); i++)
	{
		cout << endl << offsets[i] << endl;

		vector<int> temp = data[i];

		for (auto& i : temp)
		{
			cout << i << endl;
		}
	}


	return 0;

}