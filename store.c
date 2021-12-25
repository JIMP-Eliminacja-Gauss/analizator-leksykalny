#include "store.h"
#include <stdlib.h>
#include <string.h>

store_t *store_init() {
    store_t *new = malloc(sizeof *new);
    // Malloc new, 2 wskaznikow na definicje i wywolanie,
    // Przypisanie wartosci NULL wskaznikom,  -1 numerom linii
    // oraz przypisanie ilosci plikow w tablicy 0
    
    if (new == NULL)
        return NULL; 

    new -> func_def = malloc(sizeof *(new -> func_def));
    new -> func_call = malloc(sizeof *(new -> func_call));
    if (new -> func_call == NULL || new -> func_def == NULL) {
        return NULL;
    }

    new -> funame = NULL;
    new -> func_def -> funame = NULL;
    new -> func_call -> funame = NULL;

    new -> inpname_amount = 0;
    new -> func_def -> inpname_amount = 0;
    new -> func_call -> inpname_amount = 0;

    for (int i = 0; i < 2; i++) {
        new -> line_number[i] = -1;
        new -> func_def -> line_number[i] = -1; 
        new -> func_call -> line_number[i] = -1;
    }

    return new;
}

void store_add_proto(char *funame, int line_number, char *inpname, store_t *fstore) {
   ; 
}

int store_add_def(char *funame, int line_number, char *inpname, store_t *fstore) {
    // Przypisywanie pamieci i wartosci jezeli jeszcze ich nie ma
    
    if (fstore -> func_def -> funame == NULL) {
        fstore -> func_def -> funame = malloc(sizeof *(fstore -> func_def -> funame) * (strlen(funame) + 1));
        if (fstore -> func_def -> funame == NULL) {
            // zabraklo pamieci
            return -1;
        } else 
            strcpy(fstore -> func_def -> funame, funame);
    }

    // jezeli nie ma jeszcze plikow w tablicy
    if (fstore -> func_def -> inpname_amount == 0) {
        fstore -> func_def -> inpname = malloc(sizeof *(fstore -> func_def -> inpname));
        if (fstore -> func_def -> inpname == NULL)
            return -1; // zabraklo pamieci
        else {
            fstore -> func_def -> inpname[0] = malloc(sizeof **(fstore -> func_def -> inpname) * (strlen(inpname) + 1));
            strcpy(fstore -> func_def -> inpname[0], inpname);
            fstore -> func_def -> inpname_amount = 1;
        } 
    } else {
        int bool_tmp = 0;
        for (int i = 0; i < fstore -> func_def -> inpname_amount; i++) {
            // jezeli nazwa pliku juz jest w tablicy
            if (strcmp(fstore -> func_def -> inpname[i], inpname) == 0) {
                bool_tmp = 1;
            }
        }

        if (!bool_tmp) {
            fstore -> func_def -> inpname_amount++;
            fstore -> func_def -> inpname = realloc(fstore -> func_def -> inpname, sizeof *(fstore -> func_def -> inpname) * fstore -> func_def -> inpname_amount);
            strcpy(fstore -> func_def -> inpname[fstore -> func_def -> inpname_amount - 1], inpname);
        }
    }

    if (fstore -> func_def -> line_number[0] == -1)
        fstore -> func_def -> line_number[0] = line_number;
    else
        fstore -> func_def -> line_number[1] = line_number; 

    return 0;
}

void store_add_call(char *funame, int line_number, char *inpname, store_t *fstore) {
    ;
} 
