#include "symtable.h"

#ifndef SET_VALUES_H
#define SET_VALUES_H

void set_type_variable(Tvariable_item *item, Tvalue value, Tvariable_type type);

void set_type_function(Tfunction_item *item, Tvariable_type return_type);

void set_args_function(Tfunction_item *item, char *key, Tvariable_type type, Tvalue value); 

void free_args_function(Tfunction_item *item);

void init_type_variable(Tvariable_item *item);

void init_type_function(Tfunction_item *item);

#endif /* SET_VALUES_H */