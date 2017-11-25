/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include <stdlib.h>
#include "error.h" 
#include <stdio.h> 
#include "parser.h"
#include "destructor.h"
#include "generator.h"
#include "built_in.h"

extern QStack* qstack;
extern Tsymtab * symtab;

int main(){
    int return_value;
	symtab = symtab_init(42);
    insert_built_in();
	return_value = parse();
	fprintf(stderr,"\n");
	switch (return_value)
	{
		case OK:
			fprintf(stderr,"Everything is OK\n");
			generate_start(qstackTop(qstack));
			break;
		case LEXICAL_ERROR:
			fprintf(stderr,"LEXICAL_ERROR\n");
			break;
		case SYNTAX_ERROR:
			fprintf(stderr,"SYNTAX_ERROR\n");
			break;
		case SEMANTIC_ERROR:
			fprintf(stderr,"SEMANTIC_ERROR\n");
			break;
		case SEMANTIC_TYPE_ERROR:
			fprintf(stderr,"SEMANTIC_TYPE_ERROR\n");
			break;
		case OTHER_ERROR:
			fprintf(stderr,"OTHER_ERROR\n");
			break;
		case COMPILER_ERROR:
			fprintf(stderr,"COMPILER_ERROR\n");
			break;
	}
	
	dispose();
	return return_value;
}