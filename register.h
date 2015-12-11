#ifndef REGISTER_H
#define REGISTER_H

extern int register_usage [];

const char *register_name(int r);
int register_alloc();
void register_free(int r);

char *register_next_label();
#endif
