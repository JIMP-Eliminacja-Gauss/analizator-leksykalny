#include "store.h"
#include <stdlib.h>
#include <string.h>

void *store_init(store_t *fstore) {
    // store_init edytuje parametry podanej jako argument struktury
    
    if (fstore == NULL)
        return NULL; 

    fstore -> func_def = malloc(sizeof *(fstore -> func_def));
    fstore -> func_call = malloc(sizeof *(fstore -> func_call));
    if (fstore -> func_call == NULL || fstore -> func_def == NULL) {
        return NULL;
    }

    fstore -> funame = NULL;
    fstore -> func_def -> funame = NULL;
    fstore -> func_call -> funame = NULL;

    fstore -> inpname_linenum = malloc(sizeof *(fstore -> inpname_linenum));
    fstore -> func_def -> inpname_linenum = malloc(sizeof *(fstore -> func_def -> inpname_linenum));
    fstore -> func_call -> inpname_linenum = malloc(sizeof *(fstore -> func_call -> inpname_linenum));

    if (fstore -> inpname_linenum == NULL || fstore -> func_def -> inpname_linenum == NULL ||
        fstore -> func_call -> inpname_linenum == NULL) 
        return NULL;

    fstore -> inpname_linenum -> next = NULL;
    fstore -> inpname_linenum -> inpname = NULL;

    fstore -> func_def -> inpname_linenum -> next = NULL;
    fstore -> func_def -> inpname_linenum -> inpname = NULL;

    fstore -> func_call -> inpname_linenum -> next = NULL;
    fstore -> func_call -> inpname_linenum -> inpname = NULL;

    for (int i = 0; i < 2; i++) {
        fstore -> inpname_linenum -> line_number[i] = -1;
        fstore -> func_def -> inpname_linenum -> line_number[i] = -1; 
        fstore -> func_call -> inpname_linenum -> line_number[i] = -1;
    }

    fstore -> next = NULL;

    return (void *) 1;
}

store_t *store_find(char *funame, store_t *head) {
    // zwraca strukture w ktorej jest juz zapisana funkcja
    // badz nowa do ktorej mozna zapisac
    store_t *temp = head; 
    store_t *temp2;

    // sprawdzanie czy funkcja nie jest juz zapisana w magazynie
    while (temp != NULL) {
        if (temp -> funame != NULL && strcmp(temp -> funame, funame) == 0)
            return temp;
        else if (temp -> func_def -> funame != NULL && strcmp(temp -> func_def -> funame, funame) == 0 )
            return temp;
        else if (temp -> func_call -> funame != NULL && strcmp(temp -> func_call -> funame, funame) == 0)
            return temp; 

        // temp2 jest nullem jesli temp nie jest ostatnia struktura
        temp2 = temp -> next == NULL ? temp : NULL;
        temp = temp -> next;
    }

    // temp = NULL
    temp = malloc(sizeof *temp);
    temp2 -> next = temp;
    store_init(temp);
    return temp;
}


int store_add_proto(char *funame, int line_number, char *inpname, store_t *head) {
    // return -1 | za malo pamieci (malloc)
    // return -2 | za malo pamieci (realloc)
    if (head -> funame == NULL && head -> func_def -> funame == NULL && head -> func_call -> funame == NULL ) { 
        head -> funame = malloc(sizeof *(head -> funame) * (strlen(funame) + 1));
        head -> inpname_linenum -> inpname = malloc(sizeof *(head -> inpname_linenum -> inpname) * (strlen(inpname) + 1));

        if (head -> funame == NULL || head -> inpname_linenum -> inpname == NULL)
            return -1;

        head -> inpname_linenum -> line_number[0] = line_number;
        strcpy(head -> funame, funame);
        strcpy(head -> inpname_linenum -> inpname, inpname); 
        return 0;
    }

    store_t *new = store_find(funame, head);
    if (new -> funame == NULL) {
        new -> funame = malloc(sizeof *(new -> funame) * (strlen(funame) + 1));

        if (new -> funame == NULL)
            return -1;

        strcpy(new -> funame, funame);
    }
    
    file_num_t *temp = new -> inpname_linenum;
    file_num_t *temp2;
    while (temp != NULL) {
        if (temp -> inpname == NULL) {
            temp -> inpname = malloc(sizeof *(temp -> inpname) * (strlen(inpname) + 1));
            strcpy(temp -> inpname, inpname);
            temp -> line_number[0] = line_number;
            temp -> line_number[1] = -1;
            return 0;
        } else if (strcmp(temp -> inpname, inpname) == 0 && (temp -> line_number[0] == line_number - 1 || temp -> line_number[1] == line_number - 1)) {
            temp -> line_number[1] = line_number;
            return 0;
        } else {
            // temp2 jest nullem jezeli temp nie jest ostatnia struktura
            temp2 = temp -> next == NULL ? temp : NULL;
            temp = temp -> next;
        }
    }


    //  temp jest NULLEM
    temp = malloc(sizeof *(temp));
    temp2 -> next = temp;
    temp -> inpname = malloc(sizeof *(temp -> inpname) * (strlen(inpname) + 1));
    temp -> line_number[0] = line_number;
    temp -> line_number[1] = -1;
    temp -> next = NULL;
    strcpy(temp -> inpname, inpname); 
    
    return 0;
    
}

/*int store_add_def(char *funame, int line_number, char *inpname, store_t *fstore) {
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
            fstore -> func_def -> inpname[fstore -> func_def -> inpname_amount - 1] = malloc(sizeof **(fstore -> func_def -> inpname) * (strlen(inpname) + 1));
            strcpy(fstore -> func_def -> inpname[fstore -> func_def -> inpname_amount - 1], inpname);
        }
    }

    if (fstore -> func_def -> line_number[0] == -1)
        fstore -> func_def -> line_number[0] = line_number;
    else
        fstore -> func_def -> line_number[1] = line_number; 

    return 0;
}*/

void store_add_call(char *funame, int line_number, char *inpname, store_t *fstore) {
    ;
} 
