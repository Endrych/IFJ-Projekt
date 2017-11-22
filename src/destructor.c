#include "destructor.h"
#include "string_storage.h"
#include "stack.h"
#include "at_que.h"
#include "frame.h"
#include "gen_stacks.h"
#include <stdlib.h>

void dispose_all(tStack *stack, Tsymtab *symtab, ATLeaf *leaf, ATQueue* s, eQueue* s2, TFstack *frame_stack, GPStack *s3)
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



/*
int main()
{
	
	tStack *stack = malloc(sizeof(tStack));

	Tsymtab *symtab = symtab_init(101);

	ATLeaf *leaf = malloc(sizeof(ATLeaf));

	eQueue *s2 = malloc(sizeof(eQueue));

	ATQueue *s1 = malloc(sizeof(ATQueue));

	TFstack *frame_stack = malloc(sizeof(TFstack));

	GPStack *s3 = malloc(sizeof(GPStack));

	dispose_all(stack, symtab, leaf, s1, s2, frame_stack, s3);

	return 0;
}*/