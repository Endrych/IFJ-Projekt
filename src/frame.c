#include "frame.h"
#include "destructor.h"
#include <stdlib.h>
#include <stdio.h>

Tframe *temp_frame = NULL; // docasny ramec 

void FS_init(TFstack *stack)
{
	stack->top = NULL;
}



bool FS_empty(TFstack *stack)
{	
	return (stack->top == NULL);
} 



void FS_destruct(TFstack *stack)
{
	if (!(FS_empty(stack)))
	{
		Tframe *temp = stack->top;
		
		while(temp)
		{
			Tframe *current = temp;
			temp = temp->next;

			for (int i = 0; i < current->var_count; i++)
			{
				if (current->vars[i].type == type_str)
				{
					free(current->vars[i].value.string);	
				}

				free(current->vars[i].id);
			}

			free(current->return_value);
			free(current->vars);
			free(current);
		}
	}

	free(stack);
}




Tframe *FS_top(TFstack *stack)
{
	if (!(FS_empty(stack)))
	{
		return stack->top;

	} else
	{
		return NULL;
	}
}



void FS_pop(TFstack *stack)
{
	if (!(FS_empty(stack)))
	{
		stack->top = stack->top->next;

	} else
	{
		; // chyba 55
	}
}



void FS_push(TFstack *stack, Tframe *frame)
{
	if(frame != NULL)
	{
		frame->next = stack->top;
		stack->top = frame;

	} else 
	{
		; // chyba 55 
	}
}



void create_frame()
{
	if (temp_frame != NULL)
	{
		temp_frame->return_value = NULL;
		temp_frame->vars = NULL;
		temp_frame->next = NULL;
		temp_frame->var_count = 0;

	} else
	{
		temp_frame = (Tframe *) malloc(sizeof(Tframe));
		if (temp_frame == NULL)
		{
			fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        	dispose_global();
		}

		temp_frame->return_value = NULL;
		temp_frame->vars = NULL;
		temp_frame->var_count = 0;
		temp_frame->next = NULL;
	}

}



void def_return_value(Tframe *frame, Tvariable *var)
{	
	/*frame->return_value = (Tvariable *) malloc(sizeof(Tvariable));
	if (frame->return_value == NULL)
	{
		fprintf(stderr, "Memory allocation failed!\n");
		return ;
	}*/

	frame->return_value = var;
}



void add_var_to_frame(Tframe *frame, Tvariable *var)
{
	(frame->var_count)++;
	
	if (frame->var_count == 1)
	{
		frame->vars = (Tvariable *) malloc(sizeof(Tvariable));
		if (frame->vars == NULL)
		{
			fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        	dispose_global();
		}

		frame->vars[frame->var_count-1] = *var;

	} else 
	{
		frame->vars = (Tvariable *) realloc(NULL, sizeof(Tvariable) * frame->var_count);
		if (frame->vars == NULL)
		{
			fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        	dispose_global();
		}

		frame->vars[frame->var_count-1] = *var;
	}
}


void push_frame(TFstack *stack, Tvariable *var, unsigned next_instr)
{
	if (temp_frame != NULL)
	{
		Tframe *local_frame = (Tframe *) malloc(sizeof(Tframe));
		if (local_frame == NULL)
		{
			fprintf(stderr, "%s\n", COMPILER_MESSAGE);
        	dispose_global();
		}

		*local_frame = *temp_frame;

		def_return_value(local_frame, var);

		local_frame->next_instr = next_instr;

		FS_push(stack, local_frame);

		if (temp_frame->var_count != 0)
		{
			free(temp_frame->vars);
			
		}
		if (temp_frame->return_value != NULL)
		{
			free(temp_frame->return_value);
		}

		free(temp_frame);
		temp_frame = NULL;
	} else
	{
		; // chyba 55
	}
}

void pop_frame(TFstack *stack)
{
	if (FS_empty(stack))
	{
		; // chyba 55
	} else
	{
		Tframe *local_frame;
		local_frame = FS_top(stack);

		*temp_frame = *local_frame;

		FS_pop(stack);

		Tframe *test = FS_top(stack);

		if (test != NULL)
		{
			//Haze segmentetaion
			//get_return_value(temp_frame, FS_top(stack));
		}

		if (local_frame->var_count != 0)
		{
			// Haze error ???
			//free(local_frame->vars);
			
		}

		if (local_frame->return_value != NULL)
		{
			free(local_frame->return_value);
		}

		free(local_frame);
		//free(temp_frame);
	}	
}

void get_return_value(Tframe *end_frame, Tframe *frame)
{
	add_var_to_frame(frame, end_frame->return_value);
}
