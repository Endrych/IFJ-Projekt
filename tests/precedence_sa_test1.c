#include "../src/precedence_sa.h"
#include "../src/lexical.h"
#include "../src/precedence_sa.h"
#include <stdlib.h>
#include <stdio.h>
#include "../src/error.h"
#include "../src/string_storage.h"

int _print_t(ATLeaf *tree, int is_left, int offset, int depth, char s[20][255]);
void print_t(ATLeaf *tree);

int main(){
    load_file("../tests/precedence-test1.ifj");
    printf("\n\n____________________________________________________\n");
    Token * token;
    Tsymtab * symtable = symtab_init(17);
   
        Token * tokenv = get_token();
    Tsymtab_item * item = symtab_insert(symtable,tokenv,type_variable); 
    item->type_strct.variable->declared = false;
    item->type_strct.variable->type = type_int;
    printf("Expr: prom + 5  \n");
    PrecendentOutput * out = precedence_analysis(NULL,symtable);
    printf("Return integer: ");
    if(out->Type == type_int)
        printf("Correct\n");
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
    while(token != NULL && (token->type != type_eol && token->type != type_eof)){
        token = get_token();
    }
    free(out);

    Token * tokenv1 = get_token();
    Tsymtab_item * item1 =  symtab_insert(symtable,tokenv1,type_variable);   
    item1->type_strct.variable->declared = true;
    item1->type_strct.variable->type = type_int;
    printf("Expr: prom + 5.4  \n");
    out = precedence_analysis(NULL,symtable);
    printf("Return double: ");
    if(out->Type == type_doub)
        printf("Correct\n");
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
    while(token != NULL && (token->type != type_eol && token->type != type_eof)){
        token = get_token();
    }
    free(out);

    printf("Expr: 5 + + 3 + - 3  4 * 5  \n");
    out = precedence_analysis(NULL,symtable);
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
    while(token->type != type_eol){
        token = get_token();
    }
    free(out);

    printf("Expr: 4 * !\"5\"  \n");
    out = precedence_analysis(NULL,symtable);
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
    while(token->type != type_eol){
        token = get_token();
    }
    free(out);

    printf("Expr: inputToken  +5 \n");
    Token * inp_token = create_token();
    inp_token->type = type_integer;
    inp_token->atribute.int_value = 16;
    out = precedence_analysis(inp_token,symtable);
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
        if(token != NULL){
        while(token->type != type_eol){
            token = get_token();
        }
    }
    free(out);

    printf("Expr: 4 < 5 > 4  \n");
    out = precedence_analysis(NULL,symtable);
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
    while(token != NULL && token->type != type_eol){
        token = get_token();
    }
    free(out);

    printf("Expr: !\"test\" < !\"ahoj\"  \n");
    out = precedence_analysis(NULL,symtable);
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
    while(token != NULL && (token->type != type_eol && token->type != type_eof)){
        token = get_token();
    }
    free(out);

    printf("Expr: jak + se + mas  \n");
    out = precedence_analysis(NULL,symtable);
    if(out->StatusCode == OK)
        print_t(out->Tree);
    dispose_at(out->Tree);
    token = out->ReturnToken;
    while(token != NULL && (token->type != type_eol && token->type != type_eof)){
        token = get_token();
    }
    free(out);
    


    symtab_free(symtable);
    destruct_storage();
    destruct_token_storage();   
    printf("\n\n____________________________________________________\n");
    close_file();
}

int _print_t(ATLeaf *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;

    if (!tree) return 0;

    if(tree->data.type == at_operators){
        if(tree->data.Atr.op_value == op_add)
            sprintf(b, "(%s)", " + ");
        else if(tree->data.Atr.op_value == op_sub)
            sprintf(b, "(%s)", " - ");
        else if(tree->data.Atr.op_value == op_mul)
            sprintf(b, "(%s)", " * ");
        else if(tree->data.Atr.op_value == op_slash)
            sprintf(b, "(%s)", " / ");
        else if(tree->data.Atr.op_value == op_division_int)
            sprintf(b, "(%s)", " \\ ");
        else if(tree->data.Atr.op_value == op_lesser)
            sprintf(b, "(%s)", " < ");    
        else if(tree->data.Atr.op_value == op_greater)
            sprintf(b, "(%s)", " > ");            
        else if(tree->data.Atr.op_value == op_lesser_equal)
            sprintf(b, "(%s)", " <=");    
        else if(tree->data.Atr.op_value == op_greater_equal)
            sprintf(b, "(%s)", " >=");
        else if(tree->data.Atr.op_value == op_assign)
            sprintf(b, "(%s)", " = ");
        else if(tree->data.Atr.op_value == op_not_equal)
            sprintf(b, "(%s)", " <>");
    }
    else if(tree->data.type == at_type_cast){
        if(tree->data.Atr.type_cast == Integer2Double)
            sprintf(b, "(I2D)");
        else
            sprintf(b, "(D2I)");
    }
    else if(tree->data.type == at_tsitem){
        char * str = tree->data.Atr.tsItem->key;
        sprintf(b, "(%c%c%c)",str[0],str[1],str[2]);
    }
    else{
        Token * tk = tree->data.Atr.token;
        if(tk->type == type_double){
            sprintf(b, "(%.1f)",tk->atribute.double_value);
        }
        else if(tk->type == type_integer){
            sprintf(b, "(%03d)",tk->atribute.int_value);
        }
        else if(tk->type == type_string){
            char * str = get_string(tk->atribute.int_value);
            sprintf(b, "(%c%c%c)",str[0],str[1],str[2]);
        }
    }

    int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return left + width + right;
}


void print_t(ATLeaf *tree)
{
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(tree, 0, 0, 0, s);

    //Vypis
    for (int i = 0; i < 10; i++)
        printf("%s\n", s[i]);
}