
#include "secondPassAsm.h"
#include<iomanip>

void SecondPassAsm::printSectionMap()
{
	cout << endl;
	for (auto& i : sectionMap)
	{
		cout << "Section data of " << i.first << " :" << endl;

		for (size_t j = 0; j < i.second.offsets.size(); j++) //j iterira po redovima jer isto ima offseta i redova
		{
			cout << hex << setfill('0') << setw(4) << i.second.offsets.at(j) << " : " << "\t";

			for (size_t k = 0; k < i.second.data[j].size(); k++)
			{
				cout << hex << setfill('0') << setw(2) << (0xff & i.second.data[j][k]) << " ";
			}

			cout << endl;
		}

		cout << endl << endl;
	}
}




void SecondPassAsm::printSymbolTable()
{
	cout << "Symbol table:" << endl;
	cout << "Value\tType\tSection\t\tName\t\tId" << endl;

	for (auto& i : symbolTable)
	{
		cout << hex << i.getValue() << " " << scopePrint(i) << " " << i.getSection() << " " << i.getSymbolName() << " " << i.getNumberID() << endl;
	}
	cout << endl << " END OF SYMBOL TABLE" << endl << endl;
}



string SecondPassAsm::scopePrint(Symbol s)
{
	if (s.getSymbolScope() == SymbolScope::GLOBAL)
	{
		return "g";
	}
	else if (s.getSymbolScope() == SymbolScope::EXTERN)
	{
		return "e";
	}
	else 	if (s.getSymbolScope() == SymbolScope::LOCAL) return "l";
}

void SecondPassAsm::addWord(string name)
{

	// sectionMap[currentSection].offsets.size() znaci kad ovo napises ides kroz istu liniju 
	/*Section data <ivt> :
		0000 : 00 00
		0002 : 00 00
		*/
	Symbol* symbol = getSymbol(name);
	if (symbol != NULL)
	{
		if (symbol->getSection() == "ABSOLUTE")
		{
			vector<char> dataTemp;
			dataTemp.push_back(symbol->getValue() & 0xff);
			dataTemp.push_back((symbol->getValue() >> 8) & 0xff);
			sectionMap[currentSection].data.push_back(dataTemp);
			sectionMap[currentSection].offsets.push_back(locationCounter);
		}
		else
		{
			if (symbol->isDefined())
			{
				if (symbol->getSymbolScope() == SymbolScope::LOCAL)
				{

					vector<char> dataTemp;
					dataTemp.push_back(symbol->getValue() & 0xff);
					dataTemp.push_back((symbol->getValue() >> 8) & 0xff);
					sectionMap[currentSection].data.push_back(dataTemp);

					sectionMap[currentSection].offsets.push_back(locationCounter);


					RelocationRecord temp;
					temp.relocationType = "R_HYP_16";
					temp.isData = true;
					temp.sectionName = currentSection;
					temp.symbolName = symbol->getSection();//ovde uzima sekciju jer je lokalan, to je dovoljno
					temp.offset = locationCounter;

					relocationTable.push_back(temp);
				}
				else
				{
					//OVDE TREBA DA SE DODAJU NULE
					vector<char> dataTemp;
					dataTemp.push_back(0 & 0xff);
					dataTemp.push_back((0 >> 8) & 0xff);
					sectionMap[currentSection].data.push_back(dataTemp);
					sectionMap[currentSection].offsets.push_back(locationCounter);


					RelocationRecord temp;
					temp.relocationType = "R_HYP_16";
					temp.isData = true;
					temp.sectionName = currentSection;
					temp.symbolName = symbol->getSymbolName();//ovde uzima simbol
					temp.offset = locationCounter;

					relocationTable.push_back(temp);
				}
			}
			else
			{
				//symbol niej definisan, ima NULE
				vector<char> dataTemp;
				dataTemp.push_back(0 & 0xff);
				dataTemp.push_back((0 >> 8) & 0xff);
				sectionMap[currentSection].data.push_back(dataTemp);
				sectionMap[currentSection].offsets.push_back(locationCounter);


				RelocationRecord temp;
				temp.relocationType = "R_HYP_16";
				temp.isData = true;
				temp.sectionName = currentSection;
				temp.symbolName = symbol->getSymbolName();
				temp.offset = locationCounter;

				relocationTable.push_back(temp);
			}
		}
		//sectionMap[currentSection]
	}
	else {
		throw AssemblerException("word symbol returned null (doesnt exist)");
	}

	locationCounter += 2;
}

void SecondPassAsm::addWord(int value)
{
	//iz literala odmah dobijamo value koji treba da se upise
	vector<char> dataTemp;
	dataTemp.push_back(value & 0xff);
	dataTemp.push_back((value >> 8) & 0xff);
	sectionMap[currentSection].data.push_back(dataTemp);
	sectionMap[currentSection].offsets.push_back(locationCounter);
	locationCounter += 2;
}

void SecondPassAsm::addSection(string sectionName)
{
	locationCounter = 0;
	currentSection = sectionName;
}

Symbol* SecondPassAsm::getSymbol(string symbolName)
{
	for (auto& i : symbolTable)
	{
		if (i.getSymbolName() == symbolName) return &i;
	}
	return NULL;
}

static int offsetEqu = 0;
void SecondPassAsm::addEqu(string symbolName, string tokenText)
{
	//je lmoze biti relokaciej?
	int value = stoi(tokenText);
	if (tokenText.at(0) == '0')
	{
		value = stoi(tokenText, 0, 16); //hexa cifra
	}
	else value = stoi(tokenText); //decimalna


	vector<char> dataTemp;
	dataTemp.push_back(value & 0xff);
	dataTemp.push_back((value >> 8) & 0xff);
	sectionMap["ABSOLUTE"].data.push_back(dataTemp);
	sectionMap["ABSOLUTE"].offsets.push_back(offsetEqu);
	offsetEqu += 2;

}

void SecondPassAsm::skipDef(int value)
{
	vector<char> dataTemp;
	for (size_t i = 0; i < value; i++)
	{
		dataTemp.push_back(0);
	}
	sectionMap[currentSection].data.push_back(dataTemp);
	sectionMap[currentSection].offsets.push_back(locationCounter);

	locationCounter += value;
}

AssemblerException::AssemblerException(std::string msg)
{
	_msg = msg;
}

const char* AssemblerException::what() const noexcept
{
	return _msg.c_str();
}
