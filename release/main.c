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

extern QStack* qstack;

int main(){
    int return_value;
    void insert_length();
    void insert_substr();
    void insert_asc();
    void insert_chr();
    void insert_built_in();
	return_value = parse();
	fprintf(stderr,"\n");
	switch (return_value)
	{
		case OK:
			fprintf(stderr,"Everything is OK\n");
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
	generate_start(qstackTop(qstack));
	dispose_global();
	return 0;
}