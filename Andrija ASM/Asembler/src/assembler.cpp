#include "assembler.h"

using namespace ss::assembler::symbol_table;

ss::assembler::Assembler::Assembler()
{
	insertSymbol(Symbol("*UNDEF*", 0, true, nullptr, SymbolType::SECTION));
	insertSymbol(Symbol("*CONST*", 0, true, nullptr, SymbolType::SECTION));

	_undef_section = _current_section = getSymbol("*UNDEF*");

	_const_section = getSymbol("*CONST*");
}

void ss::assembler::Assembler::labelDef(std::string symbol_name)
{
	// Dohvatimo simbol, ukoliko ne postoji u tabeli, bice kreiran
	Symbol* symbol = getSymbol(symbol_name);


	// Simbol nije definisan, definisemo ga
	if (symbol->isDefined() == false)
	{
		symbol->section(_current_section);
		symbol->value(_loc_cnt);
		symbol->symbolType(SymbolType::LABEL);
		symbol->isDefined(true);
	}
	else
	{
		throw AssemblerException("Simbol '" + symbol_name + "' je vec definisan");
	}
}

void ss::assembler::Assembler::sectionDef(std::string section_name)
{
	Symbol* symbol = getSymbol(section_name);

	if (symbol->isDefined() == false)
	{
		_current_section->value(_loc_cnt);
		_loc_cnt = 0;

		_current_section = symbol;
		symbol->section(symbol);
		symbol->value(0);
		symbol->symbolType(SymbolType::SECTION);
		symbol->isDefined(true);
	}
	else
	{
		throw AssemblerException("Simbol '" + section_name + "' je vec definisan");
	}
}

void ss::assembler::Assembler::globalDef(std::string symbol_name)
{
	Symbol* symbol = getSymbol(symbol_name);

	symbol->symbolScope(SymbolScope::GLOBAL);
}

void ss::assembler::Assembler::externDef(std::string symbol_name)
{
	globalDef(symbol_name);
}

void ss::assembler::Assembler::skipDef(int bytes_to_skip)
{
	_loc_cnt += bytes_to_skip;
}

void ss::assembler::Assembler::dataDef(int size, int value)
{
	// TODO: popuniti memoriu sa datom vrednoscu
	_loc_cnt += size;
}

void ss::assembler::Assembler::dataDef(int size, std::string symbol_name)
{
	Symbol* symbol = getSymbol(symbol_name);

	// TODO: upisati vrednost simbola u memoriju i generisati
	// relokacioni zapis po potrebi

	_loc_cnt += size;
}

Symbol* ss::assembler::Assembler::getSymbol(std::string symbol_name)
{
	try
	{
		return &_symbol_table.at(symbol_name);
	}
	catch (std::exception e)
	{
		// Kreiramo simbol u tabeli simbola, pretpostavljamo da je u pitanju labela
		// ukoliko se ispostavi da nije, to cemo posle naknadno promeniti.
		insertSymbol(Symbol(symbol_name, 0, false, _undef_section, SymbolType::LABEL));
		return &_symbol_table.at(symbol_name);
	}
}

void ss::assembler::Assembler::insertSymbol(Symbol symbol)
{
	_symbol_table.insert(std::make_pair(symbol.symbolName(), symbol));
}

ss::assembler::AssemblerException::AssemblerException(std::string msg)
	: _msg(msg)
{
}

const char* ss::assembler::AssemblerException::what() const noexcept
{
	return _msg.c_str();
}
