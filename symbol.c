#include <stdlib.h>
#include "symbol.h"

int symbol_current_code = 0;
extern int param_count;

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name) {
	struct symbol *symbol = malloc(sizeof(struct symbol));
	symbol -> kind = kind;
	symbol -> type = type;
	symbol -> name = name;
	symbol -> code = ++symbol_current_code;
	return symbol;
}

char *symbol_code(struct symbol *s) {
	if(s->kind == SYMBOL_GLOBAL)
		return s->name;
	if(s->kind == SYMBOL_PARAM) {
		char *str = malloc(sizeof(char)*256);
		sprintf(str,"-%d(%%rbp)",8*s->which);
		return str;
	}
	if(s->kind == SYMBOL_LOCAL) {
		char *str = malloc(sizeof(char)*256);
		sprintf(str,"-%d(%%rbp)",8*(s->which + param_count));
		return str;
	}
	return 0;
}
