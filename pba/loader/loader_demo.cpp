#include <stdio.h>
#include <stdint.h>
#include <string>
#include "loader.hpp"

int main(int argc, char** argv){
	if(argc < 2){
		printf("Usage: %s <filename> <section_name>\n", argv[0]);
		return 1;
	}

	size_t i;
	Binary bin;
	Section *sec, *dump = NULL;
	Symbol *sym;
	std::string filename;

	filename.assign(argv[1]);
	if(load_binary(filename, &bin, Binary::BIN_TYPE_AUTO) < 0){
		return 1;
	}

	printf("loaded binary '%s' %s/%s (%u bits) entry@0x%016jx\n", 
			bin.filename.c_str(), bin.type_str.c_str(),
			bin.arch_str.c_str(), bin.bits, bin.entrypoint);

	for(i = 0; i < bin.sections.size(); i++){
		sec = &bin.sections[i];
		if(argc > 2 && sec->name == argv[2]){
			dump = sec;
		}
		printf("0x%016jx %-8ju %-20s %s\n",
				sec->vma, sec->size, sec->name.c_str(),
				sec->type == Section::SEC_TYPE_CODE ? "CODE" : "DATA");
	}

	if(bin.symbols.size() > 0){
		printf("scanned symbol tables\n");
		for(i = 0; i < bin.symbols.size(); i++){
			sym = &bin.symbols[i];
			printf("%-40s 0x%016jx ",
				sym->name.c_str(), sym->addr);
				if(sym->type & Symbol::SYM_TYPE_FUNC){
					printf("FUNC\n");
				}else if(sym->type & Symbol::SYM_TYPE_DATA){
					printf("DATA\n");
				}
		}
	}
	if(dump){
		printf("Dump of section '%s'\n", dump->name.c_str());
		for(int i = 0; i < dump->size; i++){
			printf("%02x ", dump->bytes[i]);
			if((i != 0 && i % 16 == 0) || i == dump->size - 1){
				printf("\n");
			}
		}	
	}
	unload_binary(&bin);
	return 0;
}
