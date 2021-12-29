#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "store.h"

void store_cout(store_t *head) {
    store_t *tmp = head;
    while (tmp != NULL) {
        char *fname = tmp -> funame;
        if (fname == NULL)
            fname = tmp -> func_def -> funame == NULL ? tmp -> func_call -> funame : tmp -> func_def -> funame;

        printf( "Funkcja: %s\n", fname);
        printf("-- Prototyp funkcji --\n");
        while (tmp -> inpname_linenum != NULL) {
            printf("\tW pliku: %s\n"
                   "\tOd linijki %d do linijki %d\n\n", 
                   tmp -> inpname_linenum -> inpname, tmp -> inpname_linenum -> line_number[0],
                   (tmp -> inpname_linenum -> line_number[1] != -1 ? 
                   tmp -> inpname_linenum -> line_number[1] : tmp -> inpname_linenum -> line_number[0]));
            tmp -> inpname_linenum = tmp -> inpname_linenum -> next;
        }
        printf("-- Definicja funkcji --\n");
        while (tmp -> func_def -> inpname_linenum != NULL) {
            printf("\tW pliku: %s\n"
                   "\tOd linijki %d do linijki %d\n\n", 
                   tmp -> func_def -> inpname_linenum -> inpname, tmp -> func_def -> inpname_linenum -> line_number[0],
                   (tmp -> func_def -> inpname_linenum -> line_number[1] != -1 ? 
                   tmp -> func_def -> inpname_linenum -> line_number[1] : tmp -> func_def -> inpname_linenum -> line_number[0]));
            tmp -> func_def -> inpname_linenum = tmp -> func_def -> inpname_linenum -> next;
        }
        printf("-- Wywolania funkcji --\n");
        while (tmp -> func_call -> inpname_linenum != NULL) {
            printf("\tW pliku: %s\n"
                   "\tOd linijki %d do linijki %d\n\n", 
                   tmp -> func_call -> inpname_linenum -> inpname, tmp -> func_call -> inpname_linenum -> line_number[0],
                   (tmp -> func_call -> inpname_linenum -> line_number[1] != -1 ? 
                   tmp -> func_call -> inpname_linenum -> line_number[1] : tmp -> func_call -> inpname_linenum -> line_number[0]));
            tmp -> func_call -> inpname_linenum = tmp -> func_call -> inpname_linenum -> next;
        }
        tmp = tmp -> next;
    }
    store_free( head );
}


int main(int argc, char **argv) {
    store_t *head =  malloc( sizeof *head );
    store_init( head );
    if( argc < 2 ) {
        printf( "Podano za malo argumentow\n" );
        return 1;
    }
    analizatorSkladni( argv[1], head );
    store_cout( head );
    return 0;
}
