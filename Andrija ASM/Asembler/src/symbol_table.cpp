#include "symbol_table.h"

using namespace ss::assembler::symbol_table;



Symbol::Symbol(std::string symbol_name, int value, bool is_defined, Symbol* section, SymbolType symbol_type)
	: _symbol_name(symbol_name),
	  _value(value),
	  _is_defined(is_defined),
	  _section(section),
	  _symbol_type(symbol_type),
	  _symbol_scope(SymbolScope::LOCAL)
{

}

std::string ss::assembler::symbol_table::Symbol::symbolName() const
{
	return _symbol_name;
}

SymbolType ss::assembler::symbol_table::Symbol::symbolType() const
{
	return _symbol_type;
}

int ss::assembler::symbol_table::Symbol::value() const
{
	return _value;
}

Symbol* ss::assembler::symbol_table::Symbol::section() const
{
	return _section;
}

bool ss::assembler::symbol_table::Symbol::isDefined() const
{
	return _is_defined;
}

std::list<ForwardReference>& ss::assembler::symbol_table::Symbol::forwardReferences()
{
	return _forward_references;
}

SymbolScope ss::assembler::symbol_table::Symbol::symbolScope() const
{
	return _symbol_scope;
}

void ss::assembler::symbol_table::Symbol::symbolType(SymbolType symbol_type)
{
	_symbol_type = symbol_type;
}

void ss::assembler::symbol_table::Symbol::value(int val)
{
	_value = val;
}

void ss::assembler::symbol_table::Symbol::section(Symbol* sec)
{
	_section = sec;
}

void ss::assembler::symbol_table::Symbol::isDefined(bool def)
{
	_is_defined = def;
}

void ss::assembler::symbol_table::Symbol::symbolScope(SymbolScope symScope)
{
	_symbol_scope = symScope;
}

