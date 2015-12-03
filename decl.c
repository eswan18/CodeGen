#include <stdlib.h>
#include <string.h>
#include "decl.h"
#include "symbol.h"
#include "scope.h"

struct type *return_type = 0;

extern int type_error_count;

struct decl *decl_create(char *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next) {
	struct decl *decl = malloc(sizeof(struct decl));
	char *n = malloc(sizeof(char) * 256);
	strcpy(n,name);
	decl -> name = n;
	decl -> type = t;
	decl -> value = v;
	decl -> code = c;
	decl -> next = next;
	return decl;
}

void decl_print(struct decl *d, int indent) {
	if(!d)
		return;
	int i;
	for(i=0;i<indent;i++)
		printf("\t");
	printf("%s: ",d->name);
	type_print(d->type);
	if(d->value) {
		printf("=");
		expr_print(d->value);
	}
	if(d->code) {
		printf("= {");
		stmt_print(d->code,indent+1);
		printf("\n");
		for(i=0;i<indent;i++)
			printf("\t");
		printf("}");
	} else {
		printf(";");
	}
	printf("\n");
	decl_print(d->next,indent);
}

void decl_resolve(struct decl *d, int should_print) {
	if (!d)
		return;
	//Check if the name is already defined in the local scope
	struct symbol *s = scope_lookup_local(d->name);
	if (s) {
		if(!d->code) {
			fprintf(stderr,"Error: variable %s already defined in current scope\n",d->name);
			exit(1);
		} else {
			d->symbol = s;
			if(should_print) {
				printf("%s resolves to ",d->name);
				switch(s->kind) {
					case SYMBOL_GLOBAL:
						printf("global %s\n",s->name);
						break;
					case SYMBOL_LOCAL:
					case SYMBOL_PARAM:
						fprintf(stderr,"Error: functions must be defined globally\n");
						break;
				}
			}
			scope_enter();
			param_list_resolve(d->type->params, should_print);
			stmt_resolve(d->code, should_print);
			scope_exit();
			decl_resolve(d->next,should_print);
			return;
		}
	}
	//Create the symbol
	struct symbol *symbol = 0;
	if (scope_level() == 0)
		symbol = symbol_create(SYMBOL_GLOBAL,d->type,d->name);
	else {
		symbol = symbol_create(SYMBOL_LOCAL,d->type,d->name);
		symbol->which = scope_symbol_count(SYMBOL_LOCAL)+1;
	}
	//Bind it and resolve internal expressions
	scope_bind(d->name, symbol);
	d->symbol = symbol;
	expr_resolve(d->value, should_print);
	//If it's a function, enter a new scope and resolve the internal statement
	if (d->code) {
		scope_enter();
		param_list_resolve(d->type->params, should_print);
		stmt_resolve(d->code, should_print);
		scope_exit();
	}
	decl_resolve(d->next, should_print);
}

void decl_typecheck(struct decl *d) {
	if(!d)
		return;
	if(d->type->kind == TYPE_FUNCTION) {
		if(!type_compare(d->type,d->symbol->type)) {
			printf("Type Error: function %s should have a type of ",d->name);
			type_print(d->symbol->type);
			printf("\n");
			type_error_count++;
		}
	}
	if(d->value) {
		if(d->type->kind == TYPE_ARRAY) {
			struct type *right = expr_typecheck(d->value);
			if(right->kind == TYPE_ARRAY) {
				//if the right side is a symbol of an array, check subtypes
				if(!type_compare(right->subtype,d->type->subtype)) {
					printf("Type Error: cannot assign array of type ");
					type_print(right->subtype);
					printf(" to array of type ");
					type_print(d->type->subtype);
					type_error_count++;
				}
			} else {
				//if the right side is an expr list, check each element
				struct expr *e_right = d->value;
				while(e_right) {
					if(!type_compare(d->type->subtype,expr_typecheck(e_right->left))) {
						printf("Type Error: cannot assign element ");
						expr_print(e_right->left);
						printf(" (");
						type_print(expr_typecheck(e_right->left));
						printf(") to array %s of subtype ",d->name);
						type_print(d->type->subtype);
						printf("\n");
						type_error_count++;
					}
					e_right = e_right -> right;
				}
			}
		} else {
			struct type *value_type = expr_typecheck(d->value);
			if(!type_compare(d->type,value_type)) {
				printf("Type Error: cannot assign ");
				expr_print(d->value);
				printf(" (");
				type_print(value_type);
				printf(") to variable %s (",d->name);
				type_print(d->type);
				printf(") \n");
				type_error_count++;
			}
		}
	}
	if(d->symbol && d->symbol->kind == SYMBOL_GLOBAL) {
		if(d->value && !expr_is_constant(d->value)) {
			printf("Type Error: global variable %s may only be assigned a literal value\n",d->name);
			type_error_count++;
		}
	}
	if(d->code) {
		//if it's a function, typecheck the content and record the expected return value
		return_type = d->type->subtype;
		stmt_typecheck(d->code);
	}
	decl_typecheck(d->next);
}
