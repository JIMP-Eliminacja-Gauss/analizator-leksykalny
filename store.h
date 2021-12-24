#ifndef _STORE_H_IS_INCLUDED_
#define _STORE_H_IS_INCLUDED_

typedef struct {
    char *funame; // name of the function
    int line_number;
    char *inpname; // FILE name
} store_t;

void store_add_def(char *funame, int line_number, char *inpname);
void store_add_proto(char *funame, int line_number, char *inpname);
void store_add_call(char *funame, int line_number, char *inpname);


#endif
