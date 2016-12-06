
#include <stdio.h>
#include <math.h>
#include "symbolTable.h"
# include "calculatorBisonDefinitions.tab.h"



/*This library aims to provide an interface between our calculator and the actual symbolTable*/
/*Searchs for a symbol in the table, if not found returns NULL*/
symbol * searchOnTable(char * lexeme);
/*Initializes the symbol table*/
void initialize();
/*Inserts an element on the table*/
symbol * insertOnTable(char * lexeme, int type);
/*Assigns a value to an element present on the table*/
double assignValue(char * name, double value);
/*Gets the value of an element given it's name*/
double getValue(char * name);
/*Modifies the value of a symbol in the table*/
symbol * modifyOnTable(char * lexeme, double value);
/*Prints the workspace of the calculator*/
void printWorkspace();
/*Frees the memory used by the table*/
void finalizeTable();
