#include "register.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char *register_name(int r) {
	char *c = malloc(4*sizeof(char *));
	switch(r) {
		case 1:
			strcpy(c,"%rax");
			break;
		case 2:
			strcpy(c,"%rbx");
			break;
		case 3:
			strcpy(c,"%rcx");
			break;
		case 4:
			strcpy(c,"%rdx");
			break;
		case 5:
			strcpy(c,"%rsi");
			break;
		case 6:
			strcpy(c,"%rdi");
			break;
		case 7:
			strcpy(c,"%rsp");
			break;
		case 8:
			strcpy(c,"%rbp");
			break;
		case 9:
			strcpy(c,"%r8");
			break;
		case 10:
			strcpy(c,"%r9");
			break;
		case 11:
			strcpy(c,"%r10");
			break;
		case 12:
			strcpy(c,"%r11");
			break;
		case 13:
			strcpy(c,"%r12");
			break;
		case 14:
			strcpy(c,"%r13");
			break;
		case 15:
			strcpy(c,"%r14");
			break;
		case 16:
			strcpy(c,"%r15");
			break;
		default:
			fprintf(stderr,"Error: illegal register address specified\n");
			exit(1);
	}
	return c;
}

int register_alloc() {
	int i = 0;
	return i;
}

void register_free(int r) {
}
