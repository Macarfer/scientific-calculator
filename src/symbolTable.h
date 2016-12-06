#include "calculatorBisonDefinitions.tab.h"
/*Main struct where symbols are going to be stored*/
typedef double (*func_t) (double);
typedef double (*func_t2) (double,double);

typedef struct symbol{
	char * lexeme;
	int type;
	union{
		double var;      /* value of a VAR */
		func_t fnctptr;  /* value of a FNCT */
		func_t2 fnctptr2;  /* value of a FNCT */
	} value;
	short line;
}symbol;

/*Allocates resources for the symbol table*/
void initializeSymbolTable();
/*Given the parameters inserts a symbol on the table*/
symbol * insertOnSymbolTable(char * lexeme, int type, short level);

/*Returns a symbol if the lexeme is in any symbol of the table. If not, returns NULL*/
symbol * searchSymbol(char * lexeme);
/*Prints the table in alphabetical order*/
void printSymbolTable();

/*Frees all the resources allocated for the table*/
void finalizeSymbolTable();
