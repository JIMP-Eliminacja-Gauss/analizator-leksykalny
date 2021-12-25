#include "store.h"
#include <stdlib.h>

store_t store_init() {
    store_t new;
    new.line_number[0] = -1; // -1 to indicate that nothing changed this
    new.line_number[1] = -1; // value after the initialization
    new.func_def = NULL;
    new.func_call = NULL;
}

void store_add_proto(char *funame, int line_number, char *inpname, store_t *fstore) {
   ; 
}

void store_add_def(char *funame, int line_number, char *inpname, store_t *fstore) {
    ;
}

void store_add_call(char *funame, int line_number, char *inpname, store_t *fstore) {
    ;
} 
