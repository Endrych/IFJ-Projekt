#include "destructor.h"
#include "string_storage.h"
#include "stack.h"
#include "token.h"
#include "at_que.h"
#include "frame.h"
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

/*
int main()
{
	add_string_to_storage("ahoj");
	Token *token = create_token();
	symtab = symtab_init(11);
	tStack *stack = malloc(sizeof(tStack));
	ATLeaf *leaf = malloc(sizeof(ATLeaf));
	eQueue *s2 = malloc(sizeof(eQueue));
	ATQueue *s1 = malloc(sizeof(ATQueue));
	TFstack *frame_stack = malloc(sizeof(TFstack));
	FS_init(frame_stack);
	GPStack *s3 = malloc(sizeof(GPStack));
	dispose_all(stack, leaf, s1, s2, frame_stack, s3);
	//dispose_global();
	print_error(LEXICAL_ERROR);

	return 0;
}*/