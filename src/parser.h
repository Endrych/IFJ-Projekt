
#include <stdbool.h>
#include "precedence_sa.h"
#include "symtable.h"

#ifndef PARSER_H
#define PARSER_H

int Prog();
int Scope();
int St_list();
int Stat();
int Else();
int Assign();
int Func();
int Param_list();
int Param();
int Next_par();
int ExprPrint();
int Tyype();
int set_return(Tfunction_item *function);
int set_type_variable(Tvariable_item *variable);
int check_type(Tsymtab_item* symtab_item, PrecendentOutput* out);

#endif
