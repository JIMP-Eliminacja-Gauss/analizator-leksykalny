#ifndef _STORE_H_IS_INCLUDED_
#define _STORE_H_IS_INCLUDED_

typedef struct helper {
    int line_number[2];
    char *inpname;
    struct helper *next;
} file_num_t;

typedef struct elem {
    char *funame; // name of the function
    file_num_t *inpname_linenum;
    
    struct elem *func_def;
    struct elem *func_call; 

    struct elem *next;
} store_t;

void *store_init(store_t *fstore);
store_t *store_find(char *funame, store_t *head);
int store_add(char *funame, int line_number, char *inpname, store_t *head, store_t *fstore);
int store_add_proto(char *funame, int line_number, char *inpname, store_t *head);
int store_add_def(char *funame, int line_number, char *inpname, store_t *head);
int store_add_call(char *funame, int line_number, char *inpname, store_t *head);
// store_add_call wywowalanie funkcji


#endif
