#include "revert_string.h"

void RevertString(char *str)
{
	// your code here
	char *reversed = (char*)malloc(sizeof(char) * (strlen(str) + 1));
	int i = strlen(str)-1;
	for (; i > -1; i--) {

		*(reversed + (strlen(str) - i) - 1) = *(str + i);
	}
	*(reversed + strlen(str)) = '\0';
	strcpy(str, reversed);
	free(reversed);
}

