#include <bfd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <stdio.h>
#include <algorithm>
#include "loader.hpp"

static bfd* open_bfd(std::string &filename){
	static int bfd_inited = 0;
	if(!bfd_inited){
		bfd_init();
		bfd_inited = 1;
	}

	bfd* bin_handler = bfd_openr(filename.c_str(), NULL);
	if(!bin_handler){
		fprintf(stderr, "failed to open '%s' (%s)\n", filename.c_str(),
			       	bfd_errmsg(bfd_get_error()));
		return NULL;
	}

	if(!bfd_check_format(bin_handler, bfd_object)){
		fprintf(stderr, "file '%s' does not look like an executable (%s)\n",
				filename.c_str(), bfd_errmsg(bfd_get_error()));
		return NULL;
	}
	bfd_set_error(bfd_error_no_error);

	if(bfd_get_flavour(bin_handler) == bfd_target_unknown_flavour){
		fprintf(stderr, "unrecognized format for binary '%s' (%s)\n",
				filename.c_str(), bfd_errmsg(bfd_get_error()));
		return NULL;
	}
	
	return bin_handler;
}

static int filter_symbols(Binary* bin){
	/* find weak symbols, for each, check if there is a non-weak symbol with
	 * the same name, if so, remove the weak
	 * return amount of overwritten symbols.
	 */
	int count = bin->symbols.size();
	std::vector<Symbol> notweak;
	for(int i = 0; i < count; i++){
		if(!bin->symbols[i].is_weak){
			notweak.push_back(bin->symbols[i]);
		}
	}
	bin->symbols.erase(std::remove_if(bin->symbols.begin(), bin->symbols.end(),
				[notweak](Symbol s){
					if(s.is_weak){
						for(int i = 0; i < notweak.size(); i++){
							if(s.name == notweak[i].name){
								return true;
							}
						}
					}
					return false;
				}), bin->symbols.end());
	return count - bin->symbols.size();
}

static int load_symbols_bfd(bfd* bin_handler, Binary* bin){
	int ret;
	long table_size, symbols, i;

	asymbol **bin_symtable = NULL;
	Symbol *sym;

	table_size = bfd_get_symtab_upper_bound(bin_handler);
	if(table_size < 0){
		fprintf(stderr, "failed to read symbol table (%s)\n",
			       	bfd_errmsg(bfd_get_error()));
		goto fail;
	}else if(table_size > 0){
		bin_symtable = (asymbol**)malloc(table_size);
		if(!bin_symtable){
			fprintf(stderr, "malloc fail\n");
			goto fail;
		}

		symbols = bfd_canonicalize_symtab(bin_handler, bin_symtable);
		if(symbols < 0){
			fprintf(stderr, "failed to read symbol table (%s)\n",
					bfd_errmsg(bfd_get_error()));
			goto fail;
		}
		
		for(int i = 0; i < symbols; i++){
			if(bin_symtable[i]->flags & BSF_FUNCTION){
				/* Write everything, then filter overwritten */
				bin->symbols.push_back(Symbol());
				sym = &bin->symbols.back();
				sym->type = Symbol::SYM_TYPE_FUNC;
				sym->name = std::string(bin_symtable[i]->name);
				sym->addr = bfd_asymbol_value(bin_symtable[i]);
				sym->is_weak = bin_symtable[i]->flags & BSF_WEAK;
			}else if(bin_symtable[i]->flags & BSF_OBJECT){
				bin->symbols.push_back(Symbol());
				sym = &bin->symbols.back();
				sym->type = Symbol::SYM_TYPE_DATA;
				sym->name = std::string(bin_symtable[i]->name);
				sym->addr = bfd_asymbol_value(bin_symtable[i]);
				sym->is_weak = bin_symtable[i]->flags & BSF_WEAK;
			}
		}
		filter_symbols(bin);
	}
	ret = 0;
	goto cleanup;

	fail:
	ret = -1;

	cleanup:
	if(bin_symtable){
		free(bin_symtable);
	}
	return ret;
}

static int load_dynamic_bfd(bfd* bin_handler, Binary* bin){
	int ret;
	long table_size, symbols, i;

	asymbol **bin_symtable = NULL;
	Symbol *sym;

	table_size = bfd_get_dynamic_symtab_upper_bound(bin_handler);
	if(table_size < 0){
		fprintf(stderr, "failed to read symbol table (%s)\n",
			       	bfd_errmsg(bfd_get_error()));
		goto fail;
	}else if(table_size > 0){
		bin_symtable = (asymbol**)malloc(table_size);
		if(!bin_symtable){
			fprintf(stderr, "malloc fail\n");
			goto fail;
		}

		symbols = bfd_canonicalize_dynamic_symtab(bin_handler, bin_symtable);
		if(symbols < 0){
			fprintf(stderr, "failed to read symbol table (%s)\n",
					bfd_errmsg(bfd_get_error()));
			goto fail;
		}

		for(int i = 0; i < symbols; i++){
			if(bin_symtable[i]->flags & BSF_FUNCTION){
				bin->symbols.push_back(Symbol());
				sym = &bin->symbols.back();
				sym->type = Symbol::SYM_TYPE_FUNC;
				sym->name = std::string(bin_symtable[i]->name);
				sym->addr = bfd_asymbol_value(bin_symtable[i]);
			}
		}
	}
	ret = 0;
	goto cleanup;

	fail:
	ret = -1;

	cleanup:
	if(bin_symtable){
		free(bin_symtable);
	}
	return ret;
}

static int load_sections_bfd(bfd* bin_handler, Binary* bin){
	int bin_flags;
	uint64_t vma, size;
	const char* secname;
	asection* bin_sec;
	Section* sec;
	Section::SectionType sectype;

	for(bin_sec = bin_handler->sections; bin_sec; bin_sec = bin_sec->next){
		bin_flags = bfd_section_flags(bin_sec);

		sectype = Section::SEC_TYPE_NONE;
		if(bin_flags & SEC_CODE){
			sectype = Section::SEC_TYPE_CODE;
		}else if(bin_flags & SEC_DATA){
			sectype = Section::SEC_TYPE_DATA;
		}else{
			continue;
		}
		vma = bfd_section_vma(bin_sec);
		size = bfd_section_size(bin_sec);
		secname = bfd_section_name(bin_sec);
		if(!secname){
			secname = "<unnamed>";
		}

		bin->sections.push_back(Section());
		sec = &bin->sections.back();

		sec->binary = bin;
		sec->name = secname;
		sec->type = sectype;
		sec->vma = vma;
		sec->size = size;
		sec->bytes = (uint8_t*)malloc(size);
		if(!sec->bytes){
			fprintf(stderr, "malloc fail\n");
			return -1;
		}

		if(!bfd_get_section_contents(bin_handler, bin_sec, sec->bytes, 0, size)){
			fprintf(stderr, "failed to read section '%s' (%s)\n",
				       	sec->name, bfd_errmsg(bfd_get_error()));
			return -1;
		}
	}
	return 0;
}

static int load_binary_bfd(std::string &filename, Binary* bin, Binary::BinaryType type){
	int ret;
	const bfd_arch_info_type* bin_info;;

	bfd* bin_handler = open_bfd(filename);
	if(!bin_handler){
		goto fail;
	}

	bin->filename = std::string(filename);
	bin->entrypoint = bfd_get_start_address(bin_handler);
	bin->type_str = std::string(bin_handler->xvec->name);

	switch(bin_handler->xvec->flavour){
		case bfd_target_elf_flavour:
			bin->type = Binary::BIN_TYPE_ELF;
			break;
		case bfd_target_coff_flavour:
			bin->type = Binary::BIN_TYPE_PE;
			break;
		default:
			fprintf(stderr, "Unsupported binary type (%s)\n", bin_handler->xvec->name);
			goto fail;
	}

	bin_info = bfd_get_arch_info(bin_handler);
	bin->arch_str = std::string(bin_info->printable_name);
	switch(bin_info->mach){
		case bfd_mach_i386_i386:
			bin->arch = Binary::ARCH_X86;
			bin->bits = 32;
			break;
		case bfd_mach_x86_64:
			bin->arch = Binary::ARCH_X86;
			bin->bits = 64;
			break;
		default:
			fprintf(stderr, "Unsupported arch (%s)\n", bin_info->printable_name);
			goto fail;
	}

	load_symbols_bfd(bin_handler, bin);
	load_dynamic_bfd(bin_handler, bin);

	if(load_sections_bfd(bin_handler, bin) < 0){
	       	goto fail;
	}

	ret = 0;
	goto cleanup;

	fail:
	ret = -1;
	cleanup:
	if(bin_handler){
		bfd_close(bin_handler);
	}
	return ret;
}

int load_binary(std::string &fname, Binary* bin, Binary::BinaryType type){
	return load_binary_bfd(fname, bin, type);
}

void unload_binary(Binary* bin){
	Section *sec;
	for(int i = 0; i < bin->sections.size(); i++){
		sec = &bin->sections[i];
		if(sec->bytes){
			free(sec->bytes);
		}
	}
}

