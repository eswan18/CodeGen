#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include <stdio.h>

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl *decl_create(char *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next);
void decl_print(struct decl *d, int indent);

void decl_resolve(struct decl *d, int should_print);

void decl_typecheck(struct decl *d);

void decl_codegen(struct decl *d, FILE *file);
void decl_codegen_func(struct decl *d, FILE *file);
void decl_codegen_literal(struct decl *d, FILE *file);

#endif
