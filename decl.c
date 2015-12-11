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

void decl_codegen(struct decl *d, FILE *file) {
	if(!d)
		return;
	
	switch(d->type->kind) {
		case TYPE_FUNCTION:
			decl_codegen_func(d, file);
			break;
		case TYPE_ARRAY:
			fprintf(stderr,"Error: arrays not implemented\n");
			exit(1);
			break;
		case TYPE_STRING:
			break;
		case TYPE_INTEGER:
		case TYPE_CHARACTER:
		case TYPE_BOOLEAN:
			decl_codegen_literal(d, file);
			break;
		case TYPE_VOID:
			break;
	}

	decl_codegen(d->next,file);
}

void decl_codegen_func(struct decl *d, FILE *file) {
	if(!d)
		return;
		
	//Define function	
	fprintf(file,".text\n");
	fprintf(file,".globl %s\n",d->name);
	fprintf(file,"%s:\n",d->name);

	//Count parameters
	int param_count = 0;
	struct param_list *a = d->type->params;
	while(a) {
		param_count++;
		a = a->next;
	}

	//Preamble
	fprintf(file,"\tpushq %%rbp\n"); //save the base pointer
	fprintf(file,"\tmovq %%rsp, %%rbp\n\n"); // set the new base pointer to esp
	switch(param_count) { //save arguments on the stack
		case 6:
			fprintf(file,"\tpushq %%r9\n");
		case 5:
			fprintf(file,"\tpushq %%r8\n");
		case 4:
			fprintf(file,"\tpushq %%rcx\n");
		case 3:
			fprintf(file,"\tpushq %%rdx\n");
		case 2:
			fprintf(file,"\tpushq %%rsi\n");
		case 1:
			fprintf(file,"\tpushq %%rdi\n");
		case 0:
			break;
		default:
			fprintf(stderr,"Error: functions may not have more than 6 parameters\n");
			exit(1);
	}
	
	//allocate the appropriate number of local variables
	int locals_count = stmt_count_decl(d->code);
	if (locals_count > 0) {
		fprintf(file,"subq $%d, %%rsp\n",locals_count*8);
	}
	
	fprintf(file,"\tpushq %%rbx\n"); //save callee-saved registers
	fprintf(file,"\tpushq %%r12\n");
	fprintf(file,"\tpushq %%r13\n");
	fprintf(file,"\tpushq %%r14\n");
	fprintf(file,"\tpushq %%r15\n\n");

	//body of function
	fprintf(file,"########### BODY OF FUNCTION: ###########\n");
	stmt_codegen(d->code, file);
	fprintf(file,"########### END OF FUNCTION BODY ###########\n\n");
	
	//Epilogue
	fprintf(file,"\tpopq %%r15\n"); //restore callee-saved registers
	fprintf(file,"\tpopq %%r14\n");
	fprintf(file,"\tpopq %%r13\n");
	fprintf(file,"\tpopq %%r12\n");
	fprintf(file,"\tpopq %%rbx\n\n");
	
	fprintf(file,"\tmovq %%rbp, %%rsp\n"); //reset stack to base pointer
	fprintf(file,"\tpopq %%rbp\n\n"); //restore the old base pointer

	fprintf(file,"\tret\n"); //return

}

void decl_codegen_literal(struct decl *d, FILE *file) {
	switch(d->symbol->kind) {
		case SYMBOL_GLOBAL:
			fprintf(file,".data\n");
			fprintf(file,"%s:\n",d->name);
			if(d->value)
				fprintf(file,"\t.quad %d\n",d->value->literal_value);
			else
				fprintf(file,"\t.quad 0\n");
		case SYMBOL_LOCAL:
			//MORE TO DO HERE
			break;
		case SYMBOL_PARAM:
			fprintf(stderr,"Error: unexpected declaration of a parameter\n");
			exit(1);
			break;
	}
}
