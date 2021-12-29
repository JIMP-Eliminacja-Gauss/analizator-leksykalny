#include <stdio.h>
#include <stdlib.h>

#include "store.h"

void store_cout(store_t *head) {
    store_t *tmp = head;
    file_num_t *tmp2 = NULL;
    while (tmp != NULL) {
        printf( "Funkcja: %s\n", tmp -> funame);
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
    if (argc < 2 && argc % 2 == 0)  
        return -1;

    store_t *head = malloc(sizeof *head);
    store_init(head);

    store_add_proto("potega", 1, "abs.c", head);
    store_add_proto("potega", 2, "abs.c", head);
    
    for (int i = 0; i < 100; i++) 
        store_add_def("potega", 20 + i, "matematyka.c", head);

    store_add_call("potega", 101, "main.c", head);
    store_add_call("potega", 111, "main.c", head);
    store_add_call("potega", 181, "main.c", head);
    store_add_call("potega", 191, "main.c", head);
    /*
    for (int i = 1; i <= argc/2; i++) {
        store_add_proto(argv[i], 1, argv[argc/2+i], head);
        store_add_proto(argv[i], 2, argv[argc/2+i], head);
    }
    */

    store_cout(head);
    store_free(head);

    

    return 0;
}
