#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	int which;
	struct type *type;
	char *name;
	int code;
};

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name);
char *symbol_code(struct symbol *s);

#endif
