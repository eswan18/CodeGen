#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include "hash_table.h"

struct scope_table {
	struct hash_table *hash_table;
	struct scope_table *next;
	int param_count;
	int local_count;
};

void scope_enter();
void scope_exit();
int scope_level();
int scope_symbol_count(symbol_t st);
void scope_bind(const char *name, struct symbol *s);
struct symbol *scope_lookup(const char *name);
struct symbol *scope_lookup_local(const char *name);

#endif
