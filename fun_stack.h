#ifndef _FUN_STACK_H_IS_INCLUDED_
#define _FUN_STACK_H_IS_INCLUDED_

typedef struct funstack {
    int level;
    char *fun_name;
    struct funstack *next;
} *funstack_t;

int top_of_funstack( void );  // zwraca par_level - "zagłębienie nawiasowe" przechowywane na szczycie
int put_on_fun_stack( int par_level, char *funame ); // odkłada na stos parę (funame,par_level)
char *get_from_fun_stack( void ); // usuwa z wierzchołka parę (funame,par_level), zwraca zdjętą funame

#endif

