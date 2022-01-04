#include <stdio.h>
#include <stdlib.h> // exit - ale exit trzeba kiedyś usunąć i nie będzie to potrzebne
#include <string.h>
#include "alex.h"       // analizator leksykalny
#include "fun_stack.h"  // stos funkcji
#include "store.h"
#define MAXINDENTLENGHT 256     // maks długość identyfikatora

static funstack_t stack = NULL;

int top_of_funstack( void ) {
    return stack -> level;
}

int put_on_fun_stack( int par_level, char *funame ) {
    if( stack == NULL ) {
        stack = malloc( sizeof( struct funstack ) );
        if( stack == NULL )
            return -2;
        stack->level = par_level;
        stack->fun_name = malloc( ( strlen( funame ) + 1 ) * sizeof( char ) );
        if( stack->fun_name == NULL )
            return -2;
        strcpy( stack->fun_name, funame );
        stack->next = NULL;
    } else {
        /* TESTOWE */ /*funstack_t test = stack;
        while( test->next != NULL ) {
            if( strcmp( test->fun_name, funame ) == 0 )
                return 0;
            test = test->next;
        }
        if( strcmp( test->fun_name, funame ) == 0 )
            return 0;*/
        funstack_t new = malloc( sizeof( struct funstack ) );
        if( new == NULL )
            return -2;
        new->level = par_level;
        new->fun_name = malloc( ( strlen( funame ) + 1 ) * sizeof( char ) );
        if( new->fun_name == NULL )
            return -2;
        strcpy( new->fun_name, funame );
        new->next = stack;
        stack = new;
    }
    return 0;
}

char *get_from_fun_stack( void ) {
    char *fname = strdup( stack->fun_name );
    if( stack->next != NULL ) {
        funstack_t tmp = stack->next;
        free( stack->fun_name );
        free( stack );
        stack = tmp;
    } else {
        free( stack->fun_name );
        free( stack );
        stack = NULL;
    }
    return fname;
}

void free_stack( void ) {
    if( stack != NULL ) {
        if( stack->next != NULL ) {
            while( stack->next != NULL ) {
                funstack_t tmp =  stack->next;
                free( stack->fun_name );
                free( stack );
                stack = tmp;
            }
            free( stack->fun_name );
            free( stack );
        } else {
            free( stack->fun_name );
            free( stack );
        }
    }
}


void
analizatorSkladni (char *inpname, store_t *head )
{                               // przetwarza plik inpname
  int  h = 0;
  stack = NULL;
  FILE *in = fopen (inpname, "r");
  char *fname;
  char *defname;

  int nbra = 0;   // bilans nawiasów klamrowych {}
  int npar = 0;   // bilans nawiasów zwykłych ()

  alex_init4file (in);          // ustaw analizator leksykalny, aby czytał in

  lexem_t lex;

  lex = alex_nextLexem ();      // pobierz następny leksem
  while (lex != EOFILE) {
      /* ================= */
      /* na koncu na stosie poinna zostac tylko ostatnia funkcja */
      /* strcpy nie zostalo zdjete ze stostu */
    switch (lex) {
    case MAKRODEF:{
        npar++;
    }
      break;
    case IDENT:{
        char *iname = alex_ident ();   // zapamiętaj identyfikator i patrz co dalej
        lexem_t nlex = alex_nextLexem ();
        if (nlex == OPEPAR) {   // nawias otwierający - to zapewne funkcja
          npar++;
          if( put_on_fun_stack (npar, iname) == -2 ) {
            exit(1);       // odłóż na stos funkcji
          }
          h++;
          
                                                // stos f. jest niezbędny, aby poprawnie obsłużyć sytuacje typu
                                                // f1( 5, f2( a ), f3( b ) )
        }
        else {                  // nie nawias, czyli nie funkcja
          lex = nlex;
          continue;
        }
      }
      break;
    case OPEPAR:
      npar++;
      break;
    case CLOPAR:{              // zamykający nawias - to może być koniec prototypu, nagłówka albo wywołania
            if( h != 0 ) {
            if ( top_of_funstack () == npar ) {       // sprawdzamy, czy liczba nawiasów bilansuje się z wierzchołkiem stosu funkcji
            
                                                // jeśli tak, to właśnie wczytany nawias jest domknięciem nawiasu otwartego
                                                // za identyfikatorem znajdującym się na wierzchołku stosu
                lexem_t nlex = alex_nextLexem ();     // bierzemy nast leksem
                fname = get_from_fun_stack();
                if (nlex == OPEBRA){   // nast. leksem to klamra a więc mamy do czynienia z def. funkcji
                    store_add_def (fname, alex_getLN (), inpname, head);
                    defname = strdup( fname );
                    nbra++;
                }
                else if (nbra == 0) {   // nast. leksem to nie { i jesteśmy poza blokami - to musi być prototyp
                    store_add_proto (fname, alex_getLN (), inpname, head);
                }
                else {                  // nast. leksem to nie { i jesteśmy wewnątrz bloku - to zapewne wywołanie
                    store_add_call (fname, alex_getLN (), inpname, head);
                }
                free( fname );
                h--;
            }
            //h--;
            }
        npar--;
      }
      break;
    case OPEBRA:
      nbra++;
      break;
    case CLOBRA:{
      /*if( nbra == 2 )
          store_add_def(fname, alex_getLN(), inpname, head);*/
      nbra--;
      if (nbra == 0) { 
          store_add_def(defname, alex_getLN(), inpname, head); /* tutaj dla pierszej ze stosu daje */
          free( defname );
      }
    }
      break;
    case ERROR:{
        fprintf (stderr, "\nBUUUUUUUUUUUUUUUUUUUUUU!\n"
                 "W pliku %s (linia %d) są błędy składni.\n"
                 "Kończę!\n\n", inpname, alex_getLN ());
        exit (1);               // to nie jest najlepsze, ale jest proste ;-)
      }
      break;
    default:
      break;
    }
    lex = alex_nextLexem ();
  }
  fclose(in);
  freeRes();
  free_stack();
}
