#include "frame.h"
#include <stdlib.h>
#include <stdio.h>


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
			}

			for (int i = 0; i < current->var_count; i++)
			{
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
	}
}


void FS_push(TFstack *stack, Tframe *frame)
{
	frame->next = stack->top;
	stack->top = frame;

}

Tframe *create_frame()
{
	Tframe *new_frame = (Tframe *) malloc(sizeof(Tframe));
	if (new_frame == NULL)
	{
		fprintf(stderr, "Memory allocation failed!\n");
		return NULL;
	}

	new_frame->return_value = NULL;
	new_frame->vars = NULL;
	new_frame->var_count = 0;
	new_frame->next = NULL;

	return new_frame;
}

void def_return_value(Tframe *frame, Tvariable *var)
{	
	frame->return_value = (Tvariable *) malloc(sizeof(Tvariable));
	if (frame->return_value == NULL)
	{
		fprintf(stderr, "Memory allocation failed!\n");
		return ;
	}

	frame->return_value = var;
}

void add_param_to_frame(Tframe *frame, Tvariable *param)
{
	(frame->var_count)++;
	if (frame->var_count == 1)
	{
		frame->vars = (Tvariable *) malloc(sizeof(Tvariable));
		if (frame->vars == NULL)
		{
			fprintf(stderr, "Memory allocation failed!\n");
			return ;
		}

		frame->vars[frame->var_count-1] = *param;
	} else 
	{
		frame->vars = (Tvariable *) realloc(frame->vars, sizeof(Tvariable) * frame->var_count);
		if (frame->vars == NULL)
		{
			fprintf(stderr, "Memory allocation failed!\n");
			return ;
		}

		frame->vars[frame->var_count-1] = *param;
	}
}

void push_frame(TFstack *stack, Tframe *frame, Tvariable *var)
{
	FS_push(stack, frame);
	def_return_value(frame, var);
}

/*void destruct_frame(TFstack *stack, Tframe *frame)
{

}*/
