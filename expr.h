#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"

typedef enum {
	EXPR_ASSIGN,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_AND,
	EXPR_OR,
	EXPR_NOT,
	EXPR_LT,
	EXPR_LE,
	EXPR_GT,
	EXPR_GE,
	EXPR_EQ,
	EXPR_NE,
	EXPR_EXPON,
	EXPR_INCR,
	EXPR_DECR,
	EXPR_NEG,

	EXPR_FUNC,
	EXPR_LIST,
	EXPR_ARRAY_DEREF,

	EXPR_NAME,
	EXPR_BOOLEAN_LITERAL,
	EXPR_INTEGER_LITERAL,
	EXPR_CHARACTER_LITERAL,
	EXPR_STRING_LITERAL
} expr_t;

struct expr {
	/* used by all expr types */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by leaf expr types */
	const char *name;
	struct symbol *symbol;
	int literal_value;
	const char *string_literal;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

struct expr *expr_create_name(const char *n);
struct expr *expr_create_boolean_literal(int c);
struct expr *expr_create_integer_literal(int c);
struct expr *expr_create_character_literal(int c);
struct expr *expr_create_string_literal(const char *str);

void expr_print(struct expr *e);
void expr_pretty_print(struct expr *e);

void expr_resolve(struct expr *e, int should_print);

int expr_is_constant(struct expr *e);
struct type *expr_typecheck(struct expr *e);
int expr_check_args_params(struct expr *e);

#endif
