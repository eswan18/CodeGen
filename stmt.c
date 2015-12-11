#include <stdlib.h>
#include "stmt.h"
#include "scope.h"
#include "register.h"

extern int type_error_count;
extern struct type *return_type;

struct stmt *stmt_create(stmt_kind_t kind, struct decl *d, struct expr *init_expr, struct expr *e, struct expr *next_expr, struct stmt *body, struct stmt *else_body) {
	struct stmt *stmt = malloc(sizeof(struct stmt));
	stmt -> kind = kind;
	stmt -> decl = d;
	stmt -> init_expr = init_expr;
	stmt -> expr = e;
	stmt -> next_expr = next_expr;
	stmt -> body = body;
	stmt -> else_body = else_body;
	return stmt;
}

void stmt_print(struct stmt *s, int indent) {
	if(!s)
		return;
	printf("\n");
	int i;
	for(i=0;i<indent-1;i++)
		printf("\t");
	//blocks should have their braces indented less than the indent number:
	if(s->kind != STMT_BLOCK)
		printf("\t");
	switch(s->kind) {
		case STMT_DECL:
			decl_print(s->decl,0);
			break;
		case STMT_EXPR:
			expr_print(s->expr);
			printf(";");
			break;
		case STMT_IF_ELSE:
			printf("if(");
			expr_print(s->expr);
			printf(")");
			stmt_print(s->body,indent+1);
			printf("\n");
			for(i=0;i<indent;i++)
				printf("\t");
			if(s->else_body)
				printf("else");
			stmt_print(s->else_body,indent+1);
			break;
		case STMT_FOR:
			printf("for(");
			expr_print(s->init_expr);
			printf(";");
			expr_print(s->expr);
			printf(";");
			expr_print(s->next_expr);
			printf(")");
			stmt_print(s->body,indent+1);
			break;
		case STMT_PRINT:
			printf("print ");
			expr_print(s->expr);
			printf(";");
			break;
		case STMT_RETURN:
			printf("return");
			if (s->expr) {
				printf(" ");
				expr_print(s->expr);
			}
			printf(";");
			break;
		case STMT_BLOCK:
			printf("{");
			stmt_print(s->body,indent+1);
			printf("\n");
			for(i=0;i<indent-1;i++)
				printf("\t");
			printf("}");
			break;
	}
	stmt_print(s->next,indent);
}

void stmt_resolve(struct stmt *s, int should_print) {
	if(!s)
		return;
	switch(s->kind) {
		case STMT_DECL:
			decl_resolve(s->decl, should_print);
			break;
		case STMT_IF_ELSE:
			expr_resolve(s->expr, should_print);
			stmt_resolve(s->body, should_print);
			stmt_resolve(s->else_body, should_print);
			break;
		case STMT_BLOCK:
			scope_enter();
			stmt_resolve(s->body, should_print);
			scope_exit();
			break;
		case STMT_FOR:
			expr_resolve(s->init_expr, should_print);
			expr_resolve(s->expr, should_print);
			expr_resolve(s->next_expr, should_print);
			stmt_resolve(s->body, should_print);
			break;
		case STMT_PRINT:
			expr_resolve(s->expr, should_print);
			break;
		case STMT_RETURN:
			expr_resolve(s->expr, should_print);
			break;
		case STMT_EXPR:
			expr_resolve(s->expr, should_print);
			break;
	}
	stmt_resolve(s->next, should_print);
}

void stmt_typecheck(struct stmt *s) {
	if(!s)
		return;
	//printf("STMT_TYPECHECK\n");
	struct type *expr_type = 0;
	switch(s->kind) {
		case STMT_DECL:
			decl_typecheck(s->decl);
			break;
		case STMT_EXPR:
			expr_typecheck(s->expr);
			break;
		case STMT_IF_ELSE:
			expr_type = expr_typecheck(s->expr);
			if(expr_type->kind != TYPE_BOOLEAN) {
				printf("Type Error: cannot use ");
				type_print(expr_type);
				printf(" in an if statement\n");
				type_error_count++;
			}
			stmt_typecheck(s->body);
			stmt_typecheck(s->else_body);
			break;
		case STMT_FOR:
			expr_typecheck(s->init_expr);
			expr_typecheck(s->expr);
			expr_typecheck(s->next_expr);
			break;
		case STMT_PRINT:
			expr_typecheck(s->expr);
			break;
		case STMT_RETURN:
			expr_type = expr_typecheck(s->expr);
			if(!type_compare(expr_type,return_type)) {
				printf("Type Error: cannot return ");
				expr_print(s->expr);
				printf(" (");
				type_print(expr_type);
				printf(") to function of type ");
				type_print(return_type);
				printf("\n");
				type_error_count++;
			}
			break;
		case STMT_BLOCK:
			stmt_typecheck(s->body);
			break;
	}
	stmt_typecheck(s->next);
}

void stmt_codegen(struct stmt *s, FILE *file) {
	if(!s)
		return;

	const char *expr_reg_name = 0;
	struct expr *current_expr = 0;

	switch(s->kind) {
		case STMT_DECL:
			break;
		case STMT_EXPR:
			break;
		case STMT_IF_ELSE:
			break;
		case STMT_FOR:
			break;
		case STMT_PRINT:
			current_expr = s->expr;
			while(current_expr && current_expr->kind == EXPR_LIST) {
				expr_codegen(current_expr->left,file);
				expr_reg_name = register_name(current_expr->left->reg);
				fprintf(file,"movq %s, %%rdi\n",expr_reg_name);
				register_free(current_expr->left->reg);
				switch(expr_typecheck(current_expr)->kind) {
					case TYPE_BOOLEAN:
						fprintf(file,"call print_boolean\n");
						break;
					case TYPE_CHARACTER:
						fprintf(file,"call print_character\n");
						break;
					case TYPE_INTEGER:
						fprintf(file,"call print_integer\n");
						break;
					case TYPE_STRING:
						fprintf(file,"call print_string\n");
						break;
					default:
						fprintf(stderr,"error: attempting to print illegal type\n");
						exit(1);
				}
				current_expr = current_expr -> right;
			}
			break;
		case STMT_RETURN:
			if(s->expr) {
				expr_codegen(s->expr,file);
				expr_reg_name = register_name(s->expr->reg);
				fprintf(file,"movq %s, %%rax\n", expr_reg_name);
				register_free(s->expr->reg);
			}
			break;
		case STMT_BLOCK:
			break;
	}	

	stmt_codegen(s->next,file);
}

int stmt_count_decl(struct stmt *s) {
	if(!s)
		return 0;
	int count_decl = 0;
	switch(s->kind) {
		case STMT_DECL:
			count_decl = 1;
			break;
		case STMT_EXPR:
		case STMT_PRINT:
		case STMT_RETURN:
			break;
		case STMT_IF_ELSE:
			count_decl = stmt_count_decl(s->else_body);
		case STMT_BLOCK:
		case STMT_FOR:
			count_decl += stmt_count_decl(s->body);
			break;
	}
	if(!s->next)
		return count_decl;
	else
		return count_decl + stmt_count_decl(s->next);
}
