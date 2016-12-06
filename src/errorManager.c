#include "errorManager.h"
#include <stdio.h>


/*Meant for displaying and handling errors of the lexicalAnalyzer*/
void showLexicalError(int code,char * string){
	/*Just switches the error code and prints a matching message on screen*/
	switch(code){
		case STRING_NOT_RECOGNIZED :
			printf("The string \"%s\" has not been recognized!\n",string);
		break;
		default:
		break;

	}
}


/*Meant for displaying and handling errors of the inputSystem*/
void showSyntaxError(int code,char *string){
	/*Just switches the error code and prints a matching message on screen*/
	switch (code){
	case FILE_NOT_FOUND:
		printf("File \"%s\" not found!\n",string);
	break;
	case ERROR_NOT_RECOGNIZED:
		printf("A not recognized error has been produced! \"%s\"\n",string);
		break;

}

}
