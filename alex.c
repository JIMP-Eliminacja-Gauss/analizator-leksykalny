#include "alex.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>
static int  ln= 0;
static char ident[256];
static FILE *ci= NULL;

int isKeyword(char *name) {
    char *reserved_names[] = {
        "auto", "break", "case", "char", "const",
         "continue", "default", "do", "double", "else", "enum", "extern",
         "float", "for", "goto", "if", "inline", "int", "long", "register",
         "restrict", "return", "short", "signed", "sizeof", "static",
         "struct", "switch", "typedef", "union", "unsigned", "void",
         "volatile", "while"
        };

    for (int i = 0; i < (int) (sizeof reserved_names / sizeof reserved_names[0]); i++) {
        if (strcmp(reserved_names[i], name) == 0)
            return 1;
    }
    return 0;
}

void    alex_init4file( FILE *in ) {
   ln= 0;
   ci= in;
}

lexem_t alex_nextLexem( void ) {
  int c;
  while( (c= fgetc(ci)) != EOF ) {
    if( isspace( c ) && c != '\n' )
        continue;
    else if( c == '\n' )
        ln++;
    else if( c == '(' )
        return OPEPAR;
    else if( c == ')' )
        return CLOPAR;
    else if( c == '{' )
        return OPEBRA;
    else if( c == '}' )
        return CLOBRA;
    else if( isalpha( c ) ) {
        int i= 1;
        ident[0] = c;
        while( isalnum( c= fgetc(ci) ) )
            ident[i++] = c;
        ident[i] = '\0';
        int len = ftell( ci );
        fseek( ci, len-1, SEEK_SET );
        return isKeyword(ident) ? OTHER : IDENT;
    } else if( c == '"' ) {
      /* Uwaga: tu trzeba jeszcze poprawic obsluge nowej linii w trakcie napisu
         i \\ w napisie 
      */
        int cp = c;
        while( (c= fgetc(ci)) != EOF && c != '"' && cp == '\\' ) {
            if( c == '\n' )
                return ERROR; 
            cp = c;
        }

        return c==EOF ? EOFILE : OTHER; 

    } else if( c == '/' ) {
      /* moze byc komentarz */
      c = fgetc( ci );
      if( c == '/' ) {
          while( ( c = fgetc( ci ) ) != EOF && c != '\n' )
              ;
          if( c ==  EOF )
              return EOFILE;
          if( c == '\n' )
              ln++;
          return OTHER;
      }
      else if( c == '*' ) {
          //while( 1 ) {
            while( ( c = fgetc( ci ) ) != EOF && c != '*' ) {
                if( c == '\n' )
                    ln++;
            }
            if( c == EOF ) {
                    break;
                    return EOFILE;
            }
            if( c == '*' ) {
                if( ( c = fgetc( ci ) ) == '/' ) {
                    break;
                    return OTHER;
                }
            }
          //}
      } else {
        return OTHER;
      }

      if( isdigit( c ) || c == '.' ) {
      /* liczba */
         return OTHER;
      }
    } else {
        return OTHER;
                }
        }       
  return EOFILE;
}

char *  alex_ident( void ) {
   return ident;
}

int     alex_getLN() {
        return ln;
}

