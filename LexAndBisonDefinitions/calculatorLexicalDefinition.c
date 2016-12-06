%{
#include <stdio.h>

#define MAX_STRING_SIZE 200

int lines=1;
char pastCharacter;
char actualCharacter;
short nestedComments=0;

char string_buf[MAX_STRING_SIZE];
char *string_buf_ptr;
%}

%%
.*   return 1;
%%
