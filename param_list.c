#include <stdlib.h>
#include <string.h>
#include "param_list.h"
#include "scope.h"

struct param_list *param_list_create(char *name, struct type *type, struct param_list *next) {
	struct param_list *param_list = malloc(sizeof(struct param_list));
	char *n = malloc(sizeof(char) * 256);
	strcpy(n,name);
	param_list -> name = n;
	param_list -> type = type;
	param_list -> next = next;
	return param_list;
}

void param_list_print(struct param_list *a) {
	if(!a)
		return;
	printf("%s: ",a->name);
	type_print(a->type);
	if(a->next) {
		printf(", ");
	}
	param_list_print(a->next);
}

void param_list_resolve(struct param_list *a, int should_print) {
	if (!a)
		return;
	//Check if the name is already defined in the local scope
	if (scope_lookup_local(a->name)) {
		fprintf(stderr,"Error: variable %s already defined in current scope\n",a->name);
		exit(1);
	}
	//Create and bind the symbol
	struct symbol *symbol = symbol_create(SYMBOL_PARAM,a->type,a->name);
	symbol->which = scope_symbol_count(SYMBOL_PARAM)+1;
	scope_bind(a->name,symbol);
		
	param_list_resolve(a->next, should_print);
}
