#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <list>
#include <string>

namespace ss
{
	namespace assembler
	{
		namespace symbol_table
		{
			class Symbol;
			struct ForwardReference
			{
				Symbol* section;
				int offset;
			};

			enum class SymbolType { EQU, LABEL, SECTION };
			enum class SymbolScope {GLOBAL, LOCAL};
			class Symbol
			{
			public:
				Symbol(std::string symbol_name, int value, bool is_defined, Symbol* section, SymbolType symbolType);

				std::string symbolName() const;
				SymbolType symbolType() const;
				int value() const;
				Symbol* section() const;
				bool isDefined() const;
				std::list<ForwardReference>& forwardReferences();
				SymbolScope symbolScope() const;

				void symbolType(SymbolType);
				void value(int val);
				void section(Symbol* sec);
				void isDefined(bool def);
				void symbolScope(SymbolScope symScope);
			private:
				std::string _symbol_name;
				SymbolType _symbol_type;
				SymbolScope _symbol_scope;

				// VELICINA SEKCIJE, ukoliko je SymbolType == SECTION
				// Offset unutar neke sekcije, ukoliko je SymbolType == LABEL
				// Sracunate vrednosti u izrazu, ukoliko je SymbolType == EQU
				int _value;
				Symbol* _section;
				bool    _is_defined;
				std::list<ForwardReference> _forward_references;

			};
		}
	}
}


#endif