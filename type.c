#include <stdlib.h>
#include "type.h"
#include "expr.h"

struct type *type_create(type_kind_t kind, struct param_list *params, struct type *subtype, struct expr *length) {
	struct type *type = malloc(sizeof(struct type));
	type -> kind = kind;
	type -> params = params;
	type -> subtype = subtype;
	type -> length = length;
	return type;
}

void type_print(struct type *t) {
	if(!t)
		return;
	switch(t->kind) {
		case TYPE_BOOLEAN:
			printf("boolean");
			break;
		case TYPE_CHARACTER:
			printf("character");
			break;
		case TYPE_INTEGER:
			printf("integer");
			break;
		case TYPE_STRING:
			printf("string");
			break;
		case TYPE_ARRAY:
			printf("array[");
			if(t->length)
				expr_print(t->length);
			printf("] ");
			type_print(t->subtype);
			break;
		case TYPE_FUNCTION:
			printf("function ");
			type_print(t->subtype);
			printf(" (");
			param_list_print(t->params);
			printf(") ");
			break;
		case TYPE_VOID:
			printf("void");
			break;
	}
}

struct type *type_copy(struct type *t) {
	if(!t)
		return 0;
	struct type *new_type = malloc(sizeof(struct type));
	new_type->kind = t->kind;
	new_type->params = t->params;
	new_type->subtype = type_copy(t->subtype);
	return new_type;
}

int type_compare(struct type *a, struct type *b) {
	if(!a && !b)
		return 1;
	if(!a || !b)
		return 0;
	if(a->kind != b->kind)
		return 0;
	if(a->kind == TYPE_FUNCTION) {
		if(!type_compare(a->subtype,b->subtype))
			return 0;
	}
	//The following may be necessary but I'm not sure
	/*if(a->params != b->params)
		return 0;*/
	return 1;
}

void type_delete(struct type *t) {
	if(!t)
		return;
	type_delete(t->subtype);
	free(t);
	return;
}
