#include "sample.h"
#include <stdio.h>
#include <string.h>

#define text "Hello, World!"

int main(void) {
	char* buf = pmalloc(sizeof(text));
	strcpy(buf, text);
	printf("buf = %s\n", buf);
	pfree(buf);
}
