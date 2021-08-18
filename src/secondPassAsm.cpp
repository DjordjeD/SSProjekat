
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

		cout << endl;

		cout << "Relocation for " << i.first << endl;
		cout << "Offset " << "\t" << "IsData" << "\t" << "relocationType" << "\t" << "sectionName" << "\t" << "symbolName(value)" << endl;
		for (auto& j : relocationTable)
		{

			if (i.first == j.sectionName) {


				cout << j.offset << "\t" << j.isData << "\t" << j.relocationType << "\t" << j.sectionName << "\t\t" << j.symbolName << endl;
			}
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
		cout << hex << i.getValue() << "\t " << scopePrint(i) << "\t " << i.getSection() << "\t " << i.getSymbolName() << "\t " << i.getNumberID() << endl;
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
		if (symbol->getSection() == "ABSOLUTE")// valjda samo za equ
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
					temp.symbolName = symbol->getSection();//ovde uzima sekciju jer je lokalan, to je dovoljno alternativno broj sekcije
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

void SecondPassAsm::updateSectionMap(vector<char> data, int locationCounterInc)
{
	sectionMap[currentSection].data.push_back(data);
	sectionMap[currentSection].offsets.push_back(locationCounter);

	locationCounter += locationCounterInc;
}

void SecondPassAsm::updateSectionMap(int offset, vector<char> data, int locationCounterInc)
{
	sectionMap[currentSection].data.push_back(data);
	sectionMap[currentSection].offsets.push_back(offset);

	locationCounter += locationCounterInc;
}

void SecondPassAsm::updateSectionMap(int offset, vector<char> data, int locationCounterInc, string section)
{
	sectionMap[section].data.push_back(data);
	sectionMap[section].offsets.push_back(offset);

	locationCounter += locationCounterInc;
}

int SecondPassAsm::instructionBinary(string insTxt)
{
	if (insTxt == "int") return 0x10;
	else if (insTxt == "pop") return 0xA0;
	else if (insTxt == "push") return 0xB0;
	else if (insTxt == "not") return 0x80;


	else if (insTxt == "halt") return 0x00;
	else if (insTxt == "iret") return 0x20;
	else if (insTxt == "ret") return 0x40;


	else if (insTxt == "call") return 0x30;
	else if (insTxt == "jmp") return 0x50;
	else if (insTxt == "jeq") return 0x51;
	else if (insTxt == "jne") return 0x52;
	else if (insTxt == "jgt") return 0x53;


	else if (insTxt == "xchg")return 0x60;
	else if (insTxt == "add") return 0x70;
	else if (insTxt == "sub") return 0x71;
	else if (insTxt == "mul") return 0x72;
	else if (insTxt == "div") return 0x73;
	else if (insTxt == "cmp") return 0x74;
	else if (insTxt == "and") return 0x81;
	else if (insTxt == "or")  return 0x82;
	else if (insTxt == "xor") return 0x83;
	else if (insTxt == "test")return 0x84;
	else if (insTxt == "shl") return 0x90;
	else if (insTxt == "shr") return 0x91;

	else if (insTxt == "ldr") return 0xA0;
	else if (insTxt == "str") return 0xB0;
}

int SecondPassAsm::absoluteSymbolValue(string symbolName)
{

	Symbol* symbol = getSymbol(symbolName);

	if (symbol != NULL)
	{
		if (symbol->getSection() == "ABSOLUTE")
			return symbol->getValue();
		else {

			RelocationRecord temp;
			temp.relocationType = "R_HYP_16";
			temp.isData = false;
			temp.sectionName = currentSection;
			temp.offset = locationCounter + 4;

			if (symbol->getSymbolScope() == SymbolScope::EXTERN || symbol->getSymbolScope() == SymbolScope::GLOBAL)
			{
				temp.symbolName = symbol->getSymbolName();
				relocationTable.push_back(temp);
				return 0;
			}
			else {
				temp.symbolName = symbol->getSection();//ovde uzima simbol
				relocationTable.push_back(temp);
				return symbol->getValue();
			}
		}
	}
	else throw AssemblerException("simbol ne postoji u absolute symbol value" + symbol->getSymbolName());


	return -1000;
}

int SecondPassAsm::pcRelativeSymbolValue(string symbolName)
{

	Symbol* symbol = getSymbol(symbolName);

	if (symbol != NULL)
	{
		RelocationRecord temp;

		if (symbol->getSection() == "ABSOLUTE") {

			temp.relocationType = "R_HYP_16_PC";
			temp.isData = false;
			temp.sectionName = currentSection;
			temp.offset = locationCounter + 4;
			temp.symbolName = symbol->getSymbolName();

			relocationTable.push_back(temp);

			return -2;
		}
		else {


			temp.relocationType = "R_HYP_16_PC";
			temp.isData = false;
			temp.sectionName = currentSection;
			temp.offset = locationCounter + 4;

			if (symbol->getSymbolScope() == SymbolScope::EXTERN || symbol->getSymbolScope() == SymbolScope::GLOBAL)
			{
				temp.symbolName = symbol->getSymbolName();
				relocationTable.push_back(temp);
				return -2; // linker posle popuni lagano
			}
			else {

				if (currentSection != symbol->getSection())// razlciite sekcije
				{
					temp.symbolName = symbol->getSection();
					relocationTable.push_back(temp);
					return symbol->getValue() - 2; // vrednost simbola fiksna, linker popravlja sekciju
				}
				else {
					//iste sekcije uvek ista prica

					temp.symbolName = symbol->getSection();
					relocationTable.push_back(temp);
					return symbol->getValue() - 2 - (locationCounter + 3);
				}
			}
		}
	}
	else throw AssemblerException("simbol ne postoji u pcrelative symbol value" + symbol->getSymbolName());


	return -1000;
}

AssemblerException::AssemblerException(std::string msg)
{
	_msg = msg;
}

const char* AssemblerException::what() const noexcept
{
	return _msg.c_str();
}
