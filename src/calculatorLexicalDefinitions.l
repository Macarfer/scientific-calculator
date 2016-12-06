/* recognize tokens for the calculator and print them out */
%{
#include "tableInterface.h"
#include "errorManager.h"
struct symbol *s=NULL;
double * number;
double val;

%}

%x loadFile

%%/*Processes simple characters*/
"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
"|" { return ABS; }
";" { return SEMICOLON; }
"=" { return EQUALSYMBOL;  }
"("  { return LEFTPARENTHESES; }
")"  { return RIGHTPARENTHESES;  }
"," {return COMMA;}
<<EOF>> {return EOF;  }

"-h"  | /*Processes different commands for calling the help menu*/
"HELP"  |
"help" {  return HELP;  }

"exit" | /*Cals exit(0) for exiting the calculator*/
"EXIT" {finalizeTable(); exit(0);}

"LOAD " | /*Detects the load and processes the path to the wanted file*/
"load " BEGIN(loadFile);
<loadFile>[^\b^\n]* yymore();
<loadFile>\n {if((s=searchOnTable(yytext))==NULL)
                s=insertOnTable(yytext,PATH);
              *((symbol**) &yylval)=s;
                BEGIN(0); return s->type;}

"workspace" | /*processes the words workspace so it can be printed*/
"WORKSPACE" { return WORKSPACE; }



[0-9]+ { yylval.value = atoi(yytext); return INTEGER; } /*processes integer numbers*/
[a-z][a-zA-Z]*  {if((s=searchOnTable(yytext))==NULL) /*processes identifieres*/
              s=insertOnTable(yytext,IDENTIFIER);
              *((symbol**) &yylval)=s;
              return s->type; }
[A-Z]+        {if((s=searchOnTable(yytext))==NULL) /*processes constant names, same as identifiers but in  capitals*/
              s=insertOnTable(yytext,CONSTANT);
              *((symbol**) &yylval)=s;
              return s->type; }
[0-9]*\.?[0-9]*([Ee][+-]?[0-9]+)? {yylval.value =atof(yytext); return FLOAT;}/*processes float numbers and scientific notation ones*/
\n { return EOL; } /*returns an end of line*/
[ \t] { /* ignore whitespace */ }
. { showLexicalError(STRING_NOT_RECOGNIZED,yytext); } /*Detects an unmatched expression*/
%%
