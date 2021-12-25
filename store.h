#ifndef _STORE_H_IS_INCLUDED_
#define _STORE_H_IS_INCLUDED_

typedef struct elem {
    char *funame; // name of the function
    int line_number[2];
    char *inpname; // FILE name
    
    struct elem *func_def;
    struct elem *func_call; 
} store_t;

void store_add_def(char *funame, int line_number, char *inpname, store_t *fstore);
void store_add_proto(char *funame, int line_number, char *inpname, store_t *fstore);
void store_add_call(char *funame, int line_number, char *inpname, store_t *fstore);
// store_add_call wywowalanie funkcji


#endif
