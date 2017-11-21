#ifndef NAME_GENERATOR
#define NAME_GENERATOR

typedef enum{
    gt_variable,
    gt_label
}GType;

char * generate_name(GType type);

#endif