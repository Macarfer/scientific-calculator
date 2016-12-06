#include "tableInterface.h"

short initialized = 0;
symbol * auxiliarSymbol=NULL;

/*Structs to create pointers to one parameter and two-parameter functions*/
struct init
{
  char *fname;
  double (*fnct) (double);
};

struct init2
{
  char *fname;
  double (*fnct) (double,double);
};

/*Definitions of strcutures holding identifiers and pinters to functions*/
struct init arith_fncts[] =
{
  { "atan", atan },
  { "cos",  cos  },
  { "exp",  exp  },
  { "ln",   log  },
  { "sin",  sin  },
  { "sqrt", sqrt },
  { 0, 0 },
};

struct init2 arith_fncts2[] =
{
  { "pow",  pow },
  { 0, 0 },
};


void init_table();

/*Initializes the table*/
void initialize(){
  initializeSymbolTable();
  init_table();
}

/*Returns a symbol from the table and, if it is not initialized then initializes the table*/
symbol * searchOnTable(char * lexeme){
  if(!initialized){
    initialized=1;
    initialize();
  }
  return searchSymbol(lexeme);
}
/*Insertas an element in the table*/
symbol * insertOnTable(char * lexeme, int type){

    return insertOnSymbolTable(lexeme,type,1);
}

/*Assigns a value to a symbol and returns that symbol*/
symbol * modifyOnTable(char * lexeme, double value){
  auxiliarSymbol = searchSymbol(lexeme);
  auxiliarSymbol->value.var = value;
  return auxiliarSymbol;
}
/*Assigns a value to a symbol and returns that value*/
double assignValue(char * name, double value){
  auxiliarSymbol = searchSymbol(name);
  auxiliarSymbol->value.var=value;
  return auxiliarSymbol->value.var;
}
/*returns the value of the selected symbol*/
double getValue(char * name){
  return searchSymbol(name)->value.var;
}
/*Initializes the function pointers in the table*/
void init_table ()
{
  int i;
  /*Adds the one argument functions*/
  for (i = 0; arith_fncts[i].fname != 0; i++)
    {
      auxiliarSymbol = insertOnSymbolTable(arith_fncts[i].fname, FNCT, 1);
      auxiliarSymbol->value.fnctptr = arith_fncts[i].fnct;
    }
  /*Adds the two-parameter functions*/
  for (i = 0; arith_fncts2[i].fname != 0; i++)
      {
        auxiliarSymbol = insertOnSymbolTable(arith_fncts2[i].fname, FNCT, 1);
        auxiliarSymbol->value.fnctptr2 = arith_fncts2[i].fnct;
      }
}

/*Prints the workspace*/
void printWorkspace(){
  printSymbolTable();
}

/*Frees the memory used by the table*/
void finalizeTable(){
  finalizeSymbolTable();
};
