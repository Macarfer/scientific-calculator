#include <stdlib.h>

/*Standart errors for using during the execution of the compiler*/
#define STRING_NOT_RECOGNIZED 400
#define ERROR_NOT_RECOGNIZED 401
#define FILE_NOT_FOUND 402

/*Meant for displaying and handling errors of the lexicalAnalyzer*/
void showLexicalError(int code, char * string);

/*Meant for displaying and handling errors of the syntax analyzer*/
void showSyntaxError(int code,char *string);
