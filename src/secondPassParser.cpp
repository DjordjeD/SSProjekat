#include "secondPassParser.h"

void SecondPassParser::parse()
{
	assembler.printSectionList();
	assembler.printSymbolTable();

}
