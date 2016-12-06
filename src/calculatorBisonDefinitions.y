%{
#include <stdio.h>
#include "lex.yy.c"
#include <string.h>

int yyerror(char *s);
int openFile(char *);
%}

/*We can process a double or a symbol*/
%union{
  double value;
  struct symbol *id;
}

/* Tokens that are used */
%token <value> NUMBER INTEGER FLOAT
%token <id> IDENTIFIER FNCT CONSTANT PATH
%token ADD SUB MUL DIV ABS EQUALSYMBOL WORKSPACE
%token EOL SEMICOLON HELP LEFTPARENTHESES RIGHTPARENTHESES COMMA

/*We define the type of the non terminal strings*/
%type <id> help
%type <value> term factor exp expCol
%%
/*First string that processes the returned content from flex*/
calclist:
 | calclist assignation EOL {}
 | SEMICOLON EOL{return EOL;}
 | calclist expCol EOL{return EOL;}
 | calclist help EOL {return EOL;}
 | calclist workspace EOL {return EOL;}
 | path EOL{}
 ;

/*Processes the asignation of values to identifiers and constants*/
assignation:
  IDENTIFIER EQUALSYMBOL expCol {modifyOnTable($1->lexeme,$3);};
| CONSTANT EQUALSYMBOL expCol {modifyOnTable($1->lexeme,$3);};
;

/*Processes expressions, if there's a semicolon, then the value is printed on screen*/
expCol: exp
 | LEFTPARENTHESES exp RIGHTPARENTHESES        { $$ = $2;           }
 | LEFTPARENTHESES exp RIGHTPARENTHESES SEMICOLON { $$ = $2; printf("+  %g\n",$2);  }
 | exp SEMICOLON  { $$ = $1 ; printf("+  %g\n",$1);}
 ;

/*Processes factor values, it also supports parentheses priority changes*/
exp: factor
 | exp ADD factor { $$ = $1 + $3;}
 | exp SUB factor { $$ = $1 - $3;}
 | exp ADD LEFTPARENTHESES exp RIGHTPARENTHESES { $$ = $1 + $4;}
 | exp SUB LEFTPARENTHESES exp RIGHTPARENTHESES { $$ = $1 - $4;}
 | LEFTPARENTHESES exp RIGHTPARENTHESES  ADD LEFTPARENTHESES exp RIGHTPARENTHESES { $$ = $2 + $6;}
 | LEFTPARENTHESES exp RIGHTPARENTHESES  SUB LEFTPARENTHESES exp RIGHTPARENTHESES { $$ = $2 - $6;}
 | LEFTPARENTHESES exp RIGHTPARENTHESES  ADD factor { $$ = $2 + $5;}
 | LEFTPARENTHESES exp RIGHTPARENTHESES  SUB factor { $$ = $2 - $5;}
 ;

/*The same as above but, in this case it uses sums*/
factor: term
 | factor MUL LEFTPARENTHESES exp RIGHTPARENTHESES{ $$ = $1 * $4; }
 | factor DIV LEFTPARENTHESES exp RIGHTPARENTHESES { $$ = $1 / $4; }
 | LEFTPARENTHESES exp RIGHTPARENTHESES MUL term { $$ = $2 * $5; }
 | LEFTPARENTHESES exp RIGHTPARENTHESES DIV term { $$ = $2 / $5; }
 | LEFTPARENTHESES exp RIGHTPARENTHESES MUL  LEFTPARENTHESES exp RIGHTPARENTHESES { $$ = $2 * $6; }
 | LEFTPARENTHESES exp RIGHTPARENTHESES DIV  LEFTPARENTHESES exp RIGHTPARENTHESES { $$ = $2 / $6; }
 | factor MUL term { $$ = $1 * $3; }
 | factor DIV term { $$ = $1 / $3; }
;

/*Terminal members, basically all entries have a number value at the end, this returns that value*/
term: NUMBER  {$$ = $1;}
 | IDENTIFIER {$$ = getValue($1->lexeme);}
 | CONSTANT {$$ = getValue($1->lexeme);}
 | INTEGER {$$ = $1;}
 | FLOAT {$$ = $1;}
 | FNCT LEFTPARENTHESES exp RIGHTPARENTHESES   {$$ = (*($1->value.fnctptr))($3); }
 | FNCT LEFTPARENTHESES exp COMMA exp RIGHTPARENTHESES   {$$ = (*($1->value.fnctptr2))($3,$5); }
 | SUB term {$$ = - $2;}
 | ABS term { $$ = $2 >= 0? $2 : - $2;}
;

/*Prints the help menu*/
help:
HELP {printf("\033c\n============================================================================\n"
                "\t\tTHIS IS THE HELP PAGE FOR THE SCIENTIFIC CALCULATORr:\n\n"
                "\tHELP,help:\tShows this help\n"
                "\tWORKSPACE,workspace:\tPrints the current wrokspace\n"
                "\texit:\tExits the calculator.\n"
                "\n\n\tUsage:\n"
                "\t$ELEMENT;\tPrints the value of the $ELEMENT on screen\n"
                "\t$ELEMENT $OPERATOR $ELEMENT\tOperates two $ELEMENTS\n"
                "\t$IDENTIFIER = $ELEMENT\tAsigns the result of the $ELEMENT to the $IDENTIFIER\n"
                "\t$CONSTANT = $ELEMENT\tAsigns the result of the $ELEMENT to the $CONSTANT\n"
                "\t$OPERATION ($ELEMENT,...)\tApplies the operation to the $ELELEMENT OR $ELEMENTS passed as argument.\n"
                "\tLOAD,load  $PATH:\tLoads the file designed by $PATH and executes it's operations\n"
                "\t$ELEMENT:\t $INTEGERS,$CONSTANTS,$FLOATS,$SCIENTIFICNUMBERS,$IDENTIFIERS,$OPERATIONS -> all identities avaliable \n"
                "\n\n\tDefinitions: \n"
                "\t$OPERATORS:\t+ - * / \n"
                "\t$INTEGER:\tInteger number \n"
                "\t$CONSTANT:\tString in capitals, it can be changed (in case we introduce an incorrect number) but it won't belong to the workspace  \n"
                "\t$FLOAT:\tA float number in the form: [+-].[X], [+-][X]. or [+-][X].[X] where [X] is any strig of integers\n"
                "\t$SCIENTIFICNUMBER:\tA number in the form [+-][X]e[+-][X]  where [X] is any strig of integers\n"
                "\n==============================================================================\n");}
;

/*Prints the workspace*/
workspace: WORKSPACE {printWorkspace();}
| WORKSPACE SEMICOLON{printWorkspace();}
;

/*Opens the path passed as an argument. It is needed to delete the las \n from that string*/
path:
PATH {$1->lexeme[strlen($1->lexeme)-1]='\0'; openFile($1->lexeme); return EOL;}
%%
/*Printf manages unmanaged errors*/
int yyerror(char *s)
{
 showSyntaxError(ERROR_NOT_RECOGNIZED ,s);
 return 1;
}

/*Opens a file and processes it. At the end returns the yyin pointer to stdin so it can read from keyboard again*/
int openFile( char * path){
  if((yyin = fopen(path,"r"))==NULL){
    showSyntaxError(FILE_NOT_FOUND,path);
    printf("\n");
  }else{
  printf("Opened File: %s\n",path);
  for(;!feof(yyin);){
    yyparse();
  }
  fclose(yyin);
  }
  yyrestart(stdin);
  fflush(stdout);
  printf("\n");
  return 0;
  }


int main(int argc,char *argv[]){
printf("\033c");
printf("=======================================================\n"
                            "\tWelcome to the Calc-Oh-Lator!\n"
                            "\n->type help for viewing the help menu\n"
      "========================================================\n");
if(argc>1)
  openFile(argv[1]);
for(;;){
  yyparse();
}
    return 0;
}
