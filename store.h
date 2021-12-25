#ifndef _STORE_H_IS_INCLUDED_
#define _STORE_H_IS_INCLUDED_

typedef struct elem {
    char *funame; // name of the function
    char **inpname; // FILE names
    int line_number[2];
    int inpname_amount;
    
    struct elem *func_def;
    struct elem *func_call; 
} store_t;

store_t *store_init(void);
int store_add_def(char *funame, int line_number, char *inpname, store_t *fstore);
void store_add_proto(char *funame, int line_number, char *inpname, store_t *fstore);
void store_add_call(char *funame, int line_number, char *inpname, store_t *fstore);
// store_add_call wywowalanie funkcji


#endif
