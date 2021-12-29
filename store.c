#include "store.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void store_init(store_t *fstore) {
    // store_init edytuje parametry podanej jako argument struktury
    
    if (fstore == NULL) {
        printf("Za malo pamieci...\n");
        exit(1);
    }

    fstore -> func_def = malloc(sizeof *(fstore -> func_def));
    fstore -> func_call = malloc(sizeof *(fstore -> func_call));
    if (fstore -> func_call == NULL || fstore -> func_def == NULL) {
        printf("Za malo pamieci...\n");
        exit(1);
    }

    fstore -> funame = NULL;
    fstore -> func_def -> funame = NULL;
    fstore -> func_call -> funame = NULL;

    fstore -> inpname_linenum = malloc(sizeof *(fstore -> inpname_linenum));
    fstore -> func_def -> inpname_linenum = malloc(sizeof *(fstore -> func_def -> inpname_linenum));
    fstore -> func_call -> inpname_linenum = malloc(sizeof *(fstore -> func_call -> inpname_linenum));

    if (fstore -> inpname_linenum == NULL || fstore -> func_def -> inpname_linenum == NULL ||
        fstore -> func_call -> inpname_linenum == NULL) {
        printf("Za malo pamieci...\n");
        exit(1);
    }

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
    if (temp == NULL) {
        printf("Niestety niewystarczajaco duzo pamieci...\n");
        exit(1);
    }
    temp2 -> next = temp;
    store_init(temp);
    return temp;
}

int store_add(char *funame, int line_number, char *inpname, store_t *head, store_t *fstore) {
    // return -1 | za malo pamieci (malloc)
    // return -2 | za malo pamieci (realloc)
    // fstore: head, head -> fun_def, head -> func_call
    if (head -> funame == NULL && head -> func_def -> funame == NULL && head -> func_call -> funame == NULL ) { 
        fstore -> funame  = malloc(sizeof *(fstore -> funame) * (strlen(funame) + 1));
        fstore -> inpname_linenum -> inpname = malloc(sizeof *(fstore -> inpname_linenum -> inpname) * (strlen(inpname) + 1));

        if (fstore -> funame == NULL || fstore -> inpname_linenum -> inpname == NULL)
            return -1;

        fstore -> inpname_linenum -> line_number[0] = line_number;
        strcpy(fstore -> funame, funame);
        strcpy(fstore -> inpname_linenum -> inpname, inpname); 
        return 0;
    }

    store_t *new = NULL;
    if (fstore == head)
        new = store_find(funame, head);
    else if (fstore == head -> func_def)
        new = store_find(funame, head) -> func_def;
    else if (fstore == head -> func_call)
        new = store_find(funame, head) -> func_call;

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

            if (temp -> inpname == NULL)
                return -1;

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

    if (temp == NULL)
        return -1;

    temp2 -> next = temp;
    temp -> inpname = malloc(sizeof *(temp -> inpname) * (strlen(inpname) + 1));

    if (temp -> inpname == NULL)
        return -1;

    temp -> line_number[0] = line_number;
    temp -> line_number[1] = -1;
    temp -> next = NULL;
    strcpy(temp -> inpname, inpname); 
    
    return 0; 
}




void store_add_proto(char *funame, int line_number, char *inpname, store_t *head) {
    if (store_add(funame, line_number, inpname, head, head) != 0) {
        printf("Blad krytyczny, za malo pamieci...\n");
        exit(1);
    } 
}

void store_add_def(char *funame, int line_number, char *inpname, store_t *head) {
    if (store_add(funame, line_number, inpname, head, head -> func_def) != 0) {
        printf("Blad krytyczny, za malo pamieci...\n");
        exit(1);
    } 
}

void store_add_call(char *funame, int line_number, char *inpname, store_t *head) {
    int c;
    if ((c = store_add(funame, line_number, inpname, head, head -> func_call)) != 0) {
        printf("%d Blad krytyczny, za malo pamieci...\n", c);
        exit(1);
    } 
} 

void store_free(store_t *head) {
    store_t *temp = head;
    file_num_t *temp2 = NULL;
    while (head != NULL) {
        temp = head;
        head = head -> next;
        free(temp -> funame);
        free(temp -> func_def -> funame);
        free(temp -> func_call -> funame);

        while (temp -> inpname_linenum != NULL) {
            free(temp -> inpname_linenum -> inpname);
            temp2 = temp -> inpname_linenum;
            temp -> inpname_linenum = temp -> inpname_linenum -> next;
            free(temp2);
        }

        while (temp -> func_def -> inpname_linenum != NULL) {
            free(temp -> func_def -> inpname_linenum -> inpname);
            temp2 = temp -> func_def -> inpname_linenum;
            temp -> func_def -> inpname_linenum = temp -> func_def -> inpname_linenum -> next;
            free(temp2);
        }

        while (temp -> func_call -> inpname_linenum != NULL) {
            free(temp -> func_call -> inpname_linenum -> inpname);
            temp2 = temp -> func_call -> inpname_linenum;
            temp -> func_call -> inpname_linenum = temp -> func_call -> inpname_linenum -> next;
            free(temp2);
        }

        free(temp -> func_def);
        free(temp -> func_call); 
        free(temp); 
    }
}


