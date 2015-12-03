#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "scope.h"

extern int resolve_error_count;
extern int type_error_count;

struct expr *expr_create( expr_t kind, struct expr *left, struct expr *right ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = kind;
	expr -> left = left;
	expr -> right = right;
	return expr;
}

struct expr *expr_create_name( const char *n ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_NAME;
	char *name = malloc(sizeof(char) * 256);
	strcpy(name, n);
	expr -> name = name;
	return expr;
}

struct expr *expr_create_boolean_literal( int c ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_BOOLEAN_LITERAL;
	expr -> literal_value = c;
	return expr;
}

struct expr * expr_create_integer_literal( int c ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_INTEGER_LITERAL;
	expr -> literal_value = c;
	return expr;
}

struct expr * expr_create_character_literal(int c) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_CHARACTER_LITERAL;
	expr -> literal_value = c;
	return expr;
}

struct expr * expr_create_string_literal( const char *str ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_STRING_LITERAL;
	char *s = malloc(sizeof(char) * 256);
	strcpy(s,str);
	expr -> string_literal = s;
	return expr;
}

void expr_print(struct expr *e) {
	if(!e)
		return;
	expr_print(e -> left);
	expr_pretty_print(e);
	expr_print(e->right);
	if (e->kind == EXPR_FUNC)
		printf(")");
	if (e->kind == EXPR_ARRAY_DEREF)
		printf("]");
}

void expr_pretty_print(struct expr *e) {
	if (!e)
		return;
	switch(e->kind) {
		case EXPR_ASSIGN:
			printf("=");
			break;
		case EXPR_ADD:
			printf("+");
			break;
		case EXPR_SUB:
			printf("-");
			break;
		case EXPR_MUL:
			printf("*");
			break;
		case EXPR_DIV:
			printf("/");
			break;
		case EXPR_MOD:
			printf("%%");
			break;
		case EXPR_AND:
			printf("&&");
			break;
		case EXPR_OR:
			printf("||");
			break;
		case EXPR_NOT:
			printf("!");
			break;
		case EXPR_LT:
			printf("<");
			break;
		case EXPR_LE:
			printf("<=");
			break;
		case EXPR_GT:
			printf(">");
			break;
		case EXPR_GE:
			printf(">=");
			break;
		case EXPR_EQ:
			printf("==");
			break;
		case EXPR_NE:
			printf("!=");
			break;
		case EXPR_EXPON:
			printf("^");
			break;
		case EXPR_INCR:
			printf("++");
			break;
		case EXPR_DECR:
			printf("--");
			break;
		case EXPR_NEG:
			printf("-");
			break;
		case EXPR_FUNC:
			printf("(");
			break;
		case EXPR_LIST:
			if(e->right)
				printf(",");
			break;
		case EXPR_ARRAY_DEREF:
			printf("[");
			break;
		case EXPR_NAME:
			printf("%s",e->name);
			break;
		case EXPR_BOOLEAN_LITERAL:
			if(e->literal_value == 1)
				printf("true");
			else
				printf("false");
			break;
		case EXPR_INTEGER_LITERAL:
			printf("%i",e->literal_value);
			break;
		case EXPR_CHARACTER_LITERAL:
			printf("'%c'",e->literal_value);
			break;
		case EXPR_STRING_LITERAL:
			printf("\"%s\"",e->string_literal);
			break;
	}
}

void expr_resolve(struct expr *e, int should_print) {
	if(!e)
		return;
	expr_resolve(e->left, should_print);
	expr_resolve(e->right, should_print);
	if(e->kind == EXPR_NAME) {
		struct symbol *s = scope_lookup(e->name);
		if(s) {
			e->symbol = s;
			if(should_print) {
				printf("%s resolves to ",e->name);
				switch(s->kind) {
					case SYMBOL_LOCAL:
						printf("local %d\n",s->which);
						break;
					case SYMBOL_GLOBAL:
						printf("global %s\n",s->name);
						break;
					case SYMBOL_PARAM:
						printf("parameter %d\n",s->which);
						break;
					default:
						fprintf(stderr,"SHOULDN'T DEFAULT\n");
				}
			}
		} else {
			printf("Variable %s is undefined\n",e->name);
			resolve_error_count++;
		}
	}
}

int expr_is_constant(struct expr *e) {
	if(e->kind == EXPR_BOOLEAN_LITERAL || e->kind == EXPR_INTEGER_LITERAL || e->kind == EXPR_CHARACTER_LITERAL || e->kind == EXPR_STRING_LITERAL)
		return 1;
	if(e->kind == EXPR_LIST) {
		struct expr *e_right = e;
		while(e_right) {
			if(!expr_is_constant(e_right->left))
				return 0;
			e_right = e_right->right;
		}
		return 1;
	}
	return 0;
}

struct type *expr_typecheck(struct expr *e) {
	if(!e)
		return type_create(TYPE_VOID,0,0,0);
	//printf("EXPR_TYPECHECK\n");
	struct type *left = 0;
	struct type *right = 0;
	left = expr_typecheck(e->left);
	right = expr_typecheck(e->right);
	struct expr *e_right = 0;
	switch(e->kind) {
		case EXPR_ASSIGN:
			if(left->kind == TYPE_ARRAY) {
				if(right->kind == TYPE_ARRAY) {
					//if the right side is a symbol of an array, check subtypes
					if(!type_compare(right->subtype,left->subtype)) {
						printf("Type Error: cannot assign array of type ");
						type_print(right->subtype);
						printf(" to array of type ");
						type_print(left->subtype);
						type_error_count++;
					}
				} else {
					//if the right side is an expr list, check each element
					e_right = e->right;
					while(e_right) {
						if(!type_compare(left->subtype,expr_typecheck(e_right->left))) {
							printf("Type Error: cannot assign element ");
							expr_print(e_right->left);
							printf(" (");
							type_print(expr_typecheck(e_right->left));
							printf(") to array %s of subtype ",e->left->name);
							type_print(expr_typecheck(e->left)->subtype);
							printf("\n");
							type_error_count++;
						}
						e_right = e_right -> right;
					}
				}
				return left;
			} else if(left->kind == TYPE_FUNCTION) {
				printf("Type Error: cannot assign anything to function %s\n",e->left->name);
				type_error_count++;
			} else {
				if(!type_compare(left,right)) {
					printf("Type Error: cannot assign ");
					expr_print(e->right);
					printf(" (");
					type_print(right);
					printf(") to variable ");
					expr_print(e->left);
					printf(" (");
					type_print(left);
					printf(") \n");
					type_error_count++;
				}
			}
			return left;
		case EXPR_ADD:
		case EXPR_SUB:
		case EXPR_MUL:
		case EXPR_DIV:
		case EXPR_MOD:
		case EXPR_EXPON:
			if(left->kind != TYPE_INTEGER || right->kind != TYPE_INTEGER) {
				printf("Type Error: cannot perform integer math on ");
				expr_print(e->left);
				printf(" (");
				type_print(left);
				printf(") and ");
				expr_print(e->right);
				printf(" (");
				type_print(right);
				printf(")\n");
				type_error_count++;
			}
			return left;
		case EXPR_LT:
		case EXPR_LE:
		case EXPR_GT:
		case EXPR_GE:
			if(left->kind != TYPE_INTEGER || right->kind != TYPE_INTEGER) {
				printf("Type Error: cannot perform integer comparison between ");
				expr_print(e->left);
				printf(" (");
				type_print(left);
				printf(") and ");
				expr_print(e->right);
				printf(" (");
				type_print(right);
				printf(") \n");
				type_error_count++;
			}
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_INCR:
		case EXPR_DECR:
			if(left->kind != TYPE_INTEGER) {
				printf("Type Error: cannot increment or decrement ");
				expr_print(e->left);
				printf(" (");
				type_print(left);
				printf(") \n");
				type_error_count++;
			}
			return left;
		case EXPR_NEG:
			if(left->kind != TYPE_INTEGER) {
				printf("Type Error: cannot perform unary negation on ");
				expr_print(e->left);
				printf(" (");
				type_print(left);
				printf(")\n");
				type_error_count++;
			}
			return left;
		case EXPR_AND:
		case EXPR_OR:
			if(left->kind != TYPE_BOOLEAN || right->kind != TYPE_BOOLEAN) {
				printf("Type Error: cannot perform boolean logic on ");
				type_print(left);
				printf(" and ");
				type_print(right);
				printf("\n");
				type_error_count++;
			}
			return left;
		case EXPR_NOT:
			if(left->kind != TYPE_BOOLEAN) {
				printf("Type Error: cannot perform boolean NOT on ");
				type_print(left);
				printf("\n");
				type_error_count++;
			}
			return left;
		case EXPR_EQ:
		case EXPR_NE:
			if(!type_compare(left,right) || left->kind == TYPE_ARRAY || right->kind == TYPE_ARRAY) {
				printf("Type Error: cannot evaluate equality between ");
				expr_print(e->left);
				printf(" (");
				type_print(left);
				printf(") and ");
				expr_print(e->right);
				printf(" (");
				type_print(right);
				printf(")\n");
				type_error_count++;
			}
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_FUNC:
			//check arguments
			if(e->left->kind != EXPR_NAME) {
				printf("Type Error: cannot use function notation on ");
				expr_print(e->left);
				printf(" (");
				type_print(left);
				printf(")\n");
				type_error_count++;
			}
			if(!expr_check_args_params(e)) {
				printf("Type Error: the arguments to %s do not match its parameters\n",e->left->name);
				type_error_count++;
			}
			return left->subtype;
		case EXPR_LIST:
			return left;
		case EXPR_ARRAY_DEREF:
			if(left->kind != TYPE_ARRAY) {
				printf("Type Error: cannot dereference ");
				expr_print(e->left);
				printf(" (");
				type_print(left);
				printf(")\n");
				return type_create(TYPE_VOID,0,0,0);
			} else if(right->kind != TYPE_INTEGER) {
				printf("Type Error: cannot dereference array ");
				expr_print(e->left);
				printf(" using ");
				expr_print(e->right);
				printf(" (");
				type_print(right);
				printf(")\n");
				type_error_count++;
			}
			return left->subtype;
		case EXPR_NAME:
			return e->symbol->type;
		case EXPR_BOOLEAN_LITERAL:
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_INTEGER_LITERAL:
			return type_create(TYPE_INTEGER,0,0,0);
		case EXPR_CHARACTER_LITERAL:
			return type_create(TYPE_CHARACTER,0,0,0);
		case EXPR_STRING_LITERAL:
			return type_create(TYPE_STRING,0,0,0);
		default:
			fprintf(stderr,"Invalid type\n");
			exit(1);
	}
	return 0;
}

int expr_check_args_params(struct expr *e) {
	struct param_list *current_param = expr_typecheck(e->left)->params;
	struct expr *current_arg_parent = e->right;
	while(current_arg_parent || current_param) {
		if(!current_arg_parent || !current_param) {
			return 0;
		}
		if(!type_compare(expr_typecheck(current_arg_parent->left),current_param->type))
			return 0;
		current_arg_parent = current_arg_parent->right;
		current_param = current_param->next;
	}
	return 1;
}
