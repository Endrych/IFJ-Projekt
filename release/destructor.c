#include "destructor.h"
#include "string_storage.h"
#include "stack.h"
#include "token.h"
#include "at_que.h"
#include "frame.h"
/* Nazev - Implementace prekladace imperativniho jazyka IFJ17
 * Petr Zubalik - xzubal04
 * Marek Kukucka - xkukuc04
 * Jan Koci - xkocij01
 * David Endrych - xendry02
 */

#include "gen_stacks.h"
#include "error.h"
#include <stdlib.h>

extern Tsymtab *symtab;
extern char* _string_storage;
extern TSItem * _token_storage;

void dispose_all(tStack *stack, ATLeaf *leaf, ATQueue* s, eQueue* s2, TFstack *frame_stack, GPStack *s3)
{
	destruct_storage();
	destruct_token_storage();
	dispose_at(leaf);
	stackDestruct(stack);
	symtab_free(symtab);
	queDestruct(s);
	equeDestruct(s2);
	FS_destruct(frame_stack);
	gsptr_stackDestruct(s3);

}

void dispose_global()
{
	destruct_storage();

	destruct_token_storage();

	symtab_free(symtab);

	exit(COMPILER_ERROR);
}