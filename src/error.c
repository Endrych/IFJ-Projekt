/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "error.h"
#include <stdio.h>
#include <stdlib.h>


void print_error(int error_code)
{
	switch (error_code)
	{
		case LEXICAL_ERROR:
			fprintf(stderr, "%s\n", LEXICAL_MESSAGE);
			break;

		case SYNTAX_ERROR:
			fprintf(stderr, "%s\n", SYNTAX_MESSAGE);
			break;

		case SEMANTIC_ERROR:
			fprintf(stderr, "%s\n", SEMANTIC_MESSAGE);
			break;
		
		case SEMANTIC_TYPE_ERROR:
			fprintf(stderr, "%s\n", SEMANTIC_TYPE_MESSAGE);
			break;

		case OTHER_ERROR:
			fprintf(stderr, "%s\n", OTHER_MESSAGE);
			break;

		case COMPILER_ERROR:
			fprintf(stderr, "%s\n", COMPILER_MESSAGE);
			break;
	}
	exit(error_code);
}
