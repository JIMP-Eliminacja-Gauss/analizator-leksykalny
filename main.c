#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "store.h"

void store_cout(store_t *head) {
    store_t *tmp = head;
    file_num_t *tmp2 = NULL;
    while (tmp != NULL) {
        char *fname = tmp -> funame;
        if (fname == NULL)
            fname = tmp -> func_def -> funame == NULL ? tmp -> func_call -> funame : tmp -> func_def -> funame;

        printf( "Funkcja: %s\n", fname);
        printf("-- Prototyp funkcji --\n");
        tmp2 = tmp -> inpname_linenum;
        while (tmp2 != NULL) {
            printf("\tW pliku: %s\n"
                   "\tOd linijki %d do linijki %d\n\n", 
                   tmp2 -> inpname, tmp2 -> line_number[0],
                   (tmp2 -> line_number[1] != -1 ? 
                   tmp2 -> line_number[1] : tmp2 -> line_number[0]));
            tmp2 = tmp2 -> next;
        }
        printf("-- Definicja funkcji --\n");
        tmp2 = tmp -> func_def -> inpname_linenum;
        while (tmp2 != NULL) {
            printf("\tW pliku: %s\n"
                   "\tOd linijki %d do linijki %d\n\n", 
                   tmp2 -> inpname, tmp2 -> line_number[0],
                   (tmp2 -> line_number[1] != -1 ? 
                   tmp2 -> line_number[1] : tmp2 -> line_number[0]));
            tmp2 = tmp2 -> next;
        }
        tmp2 = tmp -> func_call -> inpname_linenum;
        printf("-- Wywolania funkcji --\n");
        while (tmp2 != NULL) {
            printf("\tW pliku: %s\n"
                   "\tOd linijki %d do linijki %d\n\n", 
                   tmp2 -> inpname, tmp2 -> line_number[0],
                   (tmp2 -> line_number[1] != -1 ? 
                   tmp2 -> line_number[1] : tmp2 -> line_number[0]));
            tmp2 = tmp2 -> next;
        }
        tmp = tmp -> next;
    }

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
    store_free(head);
    return 0;
}
