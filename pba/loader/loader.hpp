#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>
#include <vector>
#include <string>

class Binary;
class Section;
class Symbol;

class Symbol{
	public:
		enum SymbolType{
			SYM_TYPE_UKN = 0,
			SYM_TYPE_FUNC = 1,
			SYM_TYPE_DATA = 2
		};
	Symbol() : type(SYM_TYPE_UKN), name(), addr(0) {}
	SymbolType type;
	std::string name;
	uint64_t addr;
	bool is_weak;
};

class Section{
	public:
		enum SectionType{
			SEC_TYPE_NONE = 0,
			SEC_TYPE_CODE = 1,
			SEC_TYPE_DATA = 2
		};
	Section() : binary(NULL), type(SEC_TYPE_NONE), vma(0), size(0), 
	bytes(NULL) {}

	bool contains(uint64_t addr){ return (vma <= addr) && (addr < vma + size); }

	Binary *binary;
	std::string name;
	SectionType type;
	uint64_t vma;
	uint64_t size;
	uint8_t *bytes;
};

class Binary{
	public:
		enum BinaryType{
			BIN_TYPE_AUTO = 0,
			BIN_TYPE_ELF = 1,
			BIN_TYPE_PE = 2
		};
		enum BinaryArch{
			ARCH_NONE = 0,
			ARCH_X86 = 1
		};
	Binary() : type(BIN_TYPE_AUTO), arch(ARCH_NONE), entrypoint(0),
       	bits(0) {}
	
	Section* get_text_section(){
		for(auto &s : sections){
			if(s.name == ".text"){
				return &s;
			}
		}
		return NULL;
	}

	std::string filename;
	std::vector<Section> sections;
	std::vector<Symbol> symbols;
	uint64_t entrypoint;
	BinaryType type;
	std::string type_str;
	BinaryArch arch;
	std::string arch_str;
	unsigned bits;

};

int load_binary(std::string&, Binary*, Binary::BinaryType);
void unload_binary(Binary*);

#endif
