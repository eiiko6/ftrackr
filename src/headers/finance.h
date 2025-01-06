#ifndef FINANCE_H
#define FINANCE_H

#include <json-c/json.h>

void initialize_data(const char *filename, struct json_object **data);
void save_data(const char *filename, struct json_object *data);
void add_transaction(struct json_object *data);
void list_transactions(struct json_object *data);
void delete_transaction(struct json_object *data);
void print_balance(struct json_object *data);

#endif // FINANCE_H
