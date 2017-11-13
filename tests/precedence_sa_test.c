#include "../src/precedence_sa.h"
#include "../src/lexical.h"
#include "../src/precedence_sa.h"
#include <stdlib.h>


int main(){
    load_file("../tests/precedence-test.ifj");
    precedence_analysis(NULL,NULL);
    close_file();
}