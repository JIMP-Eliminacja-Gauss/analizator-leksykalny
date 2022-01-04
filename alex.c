#include "alex.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
static int  ln= 1;
static char ident[256];
static FILE *ci= NULL;

static char *reserved_names[] = {
    "auto", "break", "case", "char", "const",
    "continue", "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "inline", "int", "long", "register", 
    "restrict", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
    };

static char **res_names = NULL;
static int k = 0;
int initRes( void ) {
    k = sizeof( reserved_names ) / sizeof( reserved_names[0] );
    res_names = malloc( k * sizeof( char * ) );
    int i;
    if( res_names == NULL )
        return -2;
    for( i = 0; i < k; i++ ) {
        res_names[i] = malloc( ( strlen( reserved_names[i] ) + 1 ) * sizeof( char ) );
        if( res_names[i] == NULL )
            return -2;
        strcpy( res_names[i],  reserved_names[i] );
    }
    return 0;
}
int addRes( char *name ) {
    k++;
    char **r = realloc( res_names, ( k ) * sizeof( char * ) );
    /*char **x = res_names;*/
    if( r == NULL )
        return -2;
    /*for( int i = 0; i < k; i++ )
        res_names++;*/
    res_names = r;
    res_names[k-1] = malloc( ( strlen( name ) + 1 ) * sizeof( char ) );
    if( *res_names == NULL )
        return -2;
    strcpy( res_names[k-1], name );
    /*res_names = x;*/
    return 0;
}

void freeRes( void ) {
    int i;
    for( i = 0; i < k; i++ )
        free( res_names[i] );
    free( res_names );
    res_names = NULL;
    k = 0;
}
int isKeyword(char *name) {
    /*char *reserved_names[] = {
        "auto", "break", "case", "char", "const",
         "continue", "default", "do", "double", "else", "enum", "extern",
         "float", "for", "goto", "if", "inline", "int", "long", "register",
         "restrict", "return", "short", "signed", "sizeof", "static",
         "struct", "switch", "typedef", "union", "unsigned", "void",
         "volatile", "while"
        };*/
    if( k == 0 ) {
        for (int i = 0; i < (int) (sizeof reserved_names / sizeof reserved_names[0]); i++) {
            if (strcmp(reserved_names[i], name) == 0)
                return 1;
        }
    } else {
        for( int i = 0; i < k; i++ )  {
            if( strcmp( res_names[i],  name ) == 0 )
                return 1;
        }
    }
    return 0;
}

void    alex_init4file( FILE *in ) {
   ln= 1;
   ci= in;
}

lexem_t alex_nextLexem( void ) {
  int c;
  char d = 'd';
  char str1[20];
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
    else if( c == '#' ) {
        if( fscanf( ci, "%s", str1 ) == 1 ) {
            if( strcmp( str1, "define" ) == 0 ) {
                if( alex_nextLexem() == IDENT ) {
                    if( alex_nextLexem() == OPEPAR ) {
                        if( res_names == NULL ) {
                            if( initRes() == -2 )
                                return ERROR;
                        }
                        if( addRes( alex_ident() ) == -2 )
                            return ERROR;
                        return MAKRODEF;
                    }
                } else {
                    return OTHER;
                }
            } else {
                return OTHER;
            }
        } else {
            return OTHER;
        }
    }
    else if( isalpha( c ) ) {
        int i= 1;
        ident[0] = c;
        while( isalnum( c= fgetc(ci) ) || c == '_' ) {
            ident[i++] = c;
        }
        ident[i] = '\0';
        int len = ftell( ci );
        fseek( ci, len-1, SEEK_SET );
        /*if( isKeyword( ident ) ) {
            while( isspace( c ) )
                c = fgetc( ci );
            if( c == EOF )
                return EOFILE;
            if( isalpha( c ) ) {
                len = ftell( ci );
                fseek( ci, len-1, SEEK_SET );
                return OTHER;
            }
            if( c == ';' )
                return OTHER;
            if( c == ':' )
                return OTHER;
            if( c == '{' ) {
                len = ftell( ci );
                fseek( ci, len-1, SEEK_SET );
                return OTHER;
            }
            if( c == '(' ) {
                int a = 1;
                while( a != 0 && c != EOF ) {
                    d = c;
                    c = fgetc( ci );
                    if( c == EOF )
                        return EOFILE;
                    if( d == '\'' ) {
                        c = fgetc( ci );
                        if( c == EOF )
                            return EOFILE;
                        c =  fgetc( ci );
                        if( c == EOF )
                            return EOFILE;
                    }
                    if( c == '\n' )
                        ln++;
                    if( c == '(' )
                        a++;
                    if( c == ')' )
                        a--;
                }
                return OTHER;
            }
        }*/
        return isKeyword(ident) ? OTHER : IDENT;
    } else if( c == '"' ) {
      /* Uwaga: tu trzeba jeszcze poprawic obsluge nowej linii w trakcie napisu
         i \\ w napisie 
      */
        int cp = c;
        while(1) {
        while( (c= fgetc(ci)) != EOF && c != '"' && cp == '\\' ) {
            if( c == '\n' ) 
                return ERROR; 
            cp = c;
        }
        if( c == EOF )
            return EOFILE;
        if( c == '"' ) {
            while( isspace( c = fgetc( ci ) ) )
                if( c == '\n' )
                    ln++;
            if( c == EOF )
                return EOFILE;
            if( c != '"' )
                return OTHER;
            continue;
        }
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
          while( 1 ) {
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
                    /*break;*/
                    return OTHER;
                }
            }
          }
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

