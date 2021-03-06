#include <stdio.h>
#include <stdlib.h> // exit - ale exit trzeba kiedyś usunąć i nie będzie to potrzebne
#include <string.h>
#include "alex.h"       // analizator leksykalny
#include "fun_stack.h"  // stos funkcji

#define MAXINDENTLENGHT 256     // maks długość identyfikatora

static funstack_t stack = NULL;

int top_of_funstack( void ) {
    funstack_t s = stack;
    while( s->next != NULL )
        s = s->next;
    return s->level;
}
void put_on_fun_stack( int par_level, char *funame ) {
    if( stack == NULL ) {
        stack = malloc( sizeof( struct funstack ) );
        if( stack == NULL ) {
            printf( "Za malo pamieci...\n" );
            exit(1);
        }
        stack->level = par_level;
        stack->fun_name = malloc( ( strlen( funame ) + 1 ) * sizeof( char ) );
        if( stack->fun_name == NULL ) {
            printf( "Za malo pamieci...\n" );
            exit(1);
        }
        strcpy( stack->fun_name, funame );
        stack->next = NULL;
    } else {
        funstack_t s = stack;
        while( s->next != NULL )
            s = s->next;
        s->next = malloc( sizeof( struct funstack ) );
        if( s->next == NULL ) {
            printf( "Za malo pamieci...\n" );
            exit(1);
        }
        s = s->next;
        s->level = par_level;
        s->fun_name = malloc( ( strlen( funame ) + 1 ) * sizeof( char ) );
        if( s->fun_name == NULL ) {
            printf( "Za malo pamieci...\n" );
            exit(1);
        }
        strcpy( s->fun_name, funame );
        s->next = NULL;
    }
}
char *get_from_fun_stack( void ) {
    funstack_t s = stack;
    while( s->next != NULL )
        s = s->next;
    return s->fun_name;
}

void free_stack( void ) {
    if( stack == NULL )
        exit(1);
    funstack_t s = stack;
    if( s->next != NULL ) {
        funstack_t s2 = stack;
        while( s2->next != NULL ) {
            s2 = s2->next; 
            free( s->fun_name );
            free( s );
            s = s2;
        }
        /* s2->next = NULL */
        free( s->fun_name );
        free( s );
    } else {
        free( s->fun_name );
        free( s );
    }
}

void
analizatorSkladni (char *inpname)
{                               // przetwarza plik inpname
  
  FILE *in = fopen (inpname, "r");

  int nbra = 0;   // bilans nawiasów klamrowych {}
  int npar = 0;   // bilans nawiasów zwykłych ()

  alex_init4file (in);          // ustaw analizator leksykalny, aby czytał in

  lexem_t lex;

  lex = alex_nextLexem ();      // pobierz następny leksem
  while (lex != EOFILE) {
    switch (lex) {
    case IDENT:{
        char *iname = alex_ident ();   // zapamiętaj identyfikator i patrz co dalej
        lexem_t nlex = alex_nextLexem ();
        if (nlex == OPEPAR) {   // nawias otwierający - to zapewne funkcja
          npar++;
          if( put_on_fun_stack (npar, iname) == 1 )
              exit(1);       // odłóż na stos funkcji
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
        if (top_of_funstack () == npar) {       // sprawdzamy, czy liczba nawiasów bilansuje się z wierzchołkiem stosu funkcji
                                                // jeśli tak, to właśnie wczytany nawias jest domknięciem nawiasu otwartego
                                                // za identyfikatorem znajdującym się na wierzchołku stosu
          lexem_t nlex = alex_nextLexem ();     // bierzemy nast leksem
          funstack_t s;
          if (nlex == OPEBRA){   // nast. leksem to klamra a więc mamy do czynienia z def. funkcji
            store_add_def (get_from_fun_stack(), alex_getLN (), inpname);
            s = stack;
            while( s->next != NULL )
                s = s->next;
            free( s->fun_name );
            free( s );
          }
          else if (nbra == 0) {   // nast. leksem to nie { i jesteśmy poza blokami - to musi być prototyp
            store_add_proto (get_from_fun_stack(), alex_getLN (), inpname);
            s = stack;
            while( s->next != NULL )
                s = s->next;
            free( s->fun_name );
            free( s );
          }
          else {                  // nast. leksem to nie { i jesteśmy wewnątrz bloku - to zapewne wywołanie
            store_add_call (get_from_fun_stack(), alex_getLN (), inpname);
            s = stack;
            while( s->next != NULL )
                s = s->next;
            free( s->fun_name );
            free( s );
          }
        }
        npar--;
      }
      break;
    case OPEBRA:
      nbra++;
      break;
    case CLOBRA:
      nbra--;
      break;
    case ERROR:{
        fprintf (stderr, "\nBUUUUUUUUUUUUUUUUUUUUUU!\n"
                 "W pliku %s (linia %d) są błędy składni.\n"
                 "Kończę!\n\n", inpname, alex_getNL ());
        exit (1);               // to nie jest najlepsze, ale jest proste ;-)
      }
      break;
    default:
      break;
    }
    lex = alex_nextLexem ();
  }
  free_stack();
}
