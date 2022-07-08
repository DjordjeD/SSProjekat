#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <map>

#include "symbol_table.h"

namespace ss
{
	namespace assembler 
	{

		class AssemblerException : public std::exception
		{
		public:
			AssemblerException(std::string msg);
			const char* what() const noexcept;
		private:
			std::string _msg;
		};

		using namespace ss::assembler::symbol_table;
		class Assembler
		{
		public:
			Assembler();

			void labelDef(std::string symbol_name);
			void sectionDef(std::string section_name);

			void globalDef(std::string symbol_name);
			void externDef(std::string symbol_name);

			void skipDef(int bytes_to_skip);

			void dataDef(int size, int value);
			void dataDef(int size, std::string symbol_name);
		private:
			std::map<std::string, Symbol> _symbol_table;

			Symbol* _current_section;

			Symbol* _undef_section;
			Symbol* _const_section;

			int _loc_cnt;


			Symbol* getSymbol(std::string symbol_name);

			void insertSymbol(Symbol symbol);
		};
	}
}

#endif