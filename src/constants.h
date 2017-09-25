typedef enum{
    ID, KEY_WORD, STRING, INT, DOUBLE
} types;

typedef struct token{
    int type;
    char value;
}Token;

