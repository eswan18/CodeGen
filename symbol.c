#include <stdlib.h>
#include "symbol.h"

int symbol_current_code = 0;

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
	char *str = malloc(sizeof(char *)*256);
	sprintf(str,"%d",s->code);
	return str;
}
