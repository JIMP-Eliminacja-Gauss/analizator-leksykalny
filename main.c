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

        // PROTOTYP
        printf( "Funkcja: %s\n", fname);
        printf("-- Prototyp funkcji --\n");
        tmp2 = tmp -> inpname_linenum;

        if (tmp2 -> inpname == NULL) 
            printf("\t Funkcja nie posiada prototypu\n\n");
        else {
            while (tmp2 != NULL) {
                printf("\tW pliku: %s\n"
                       "\tOd linijki %d do linijki %d\n\n", 
                       tmp2 -> inpname, tmp2 -> line_number[0],
                       (tmp2 -> line_number[1] != -1 ? 
                       tmp2 -> line_number[1] : tmp2 -> line_number[0]));
                tmp2 = tmp2 -> next;
        }
        }
        // DEFINICJA
        printf("-- Definicja funkcji --\n");

        tmp2 = tmp -> func_def -> inpname_linenum;
        if (tmp2 -> inpname == NULL)
            printf("\tFunkcja nie jest zdefiniowana w zadnym z plikow!\n\n");
        else {
            while (tmp2 != NULL) {
                printf("\tW pliku: %s\n"
                       "\tOd linijki %d do linijki %d\n\n", 
                       tmp2 -> inpname, tmp2 -> line_number[0],
                       (tmp2 -> line_number[1] != -1 ? 
                       tmp2 -> line_number[1] : tmp2 -> line_number[0]));
                tmp2 = tmp2 -> next;
            }
        }
       
        // WYWOLANIA
        printf("-- Wywolania funkcji --\n");

        tmp2 = tmp -> func_call -> inpname_linenum;

        if (tmp2 -> inpname == NULL)
            printf("\tFunkcja nie jest wywolywana ani razu!\n\n"); 
        else {
            while (tmp2 != NULL) {
                printf("\tW pliku: %s\n"
                       "\tOd linijki %d do linijki %d\n\n", 
                       tmp2 -> inpname, tmp2 -> line_number[0],
                       (tmp2 -> line_number[1] != -1 ? 
                       tmp2 -> line_number[1] : tmp2 -> line_number[0]));
                tmp2 = tmp2 -> next;
            }
        }

        tmp = tmp -> next;
        printf("\n\n");
    }

}

int get_filenames(char *file, char **file_array) {
    FILE *we;
    static int counter = 1;
    char c;
    char *name = malloc(sizeof *name * 256);
    if ( (we = fopen(file, "r")) == NULL ) {
        return -1;
    }

    while ( (c = fgetc(we)) != EOF) {
        if (c == '#' && fscanf(we, "include \"%s.h", name) == 1) {
            name[strlen(name)-1] = '\0'; // ostatni znak to "
            name = realloc(name, sizeof *name * (strlen(name) + 1)); 
            *(file_array + counter) = strdup(name);
            counter += get_filenames( *(file_array + counter), file_array );
            counter++;
        }
    } 

    free(name);
    fclose(we);

    return counter;

}



int main(int argc, char **argv) {
    store_t *head =  malloc( sizeof *head );
    char **files;
    files = malloc(sizeof *files * 100);
    *files = strdup(argv[1]);
    int end = get_filenames(argv[1], files);
    for (int i = 0; i < end; i++) {
        printf("%s", files[i]);
    }
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
