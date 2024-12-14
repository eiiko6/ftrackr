#include "headers/finance.h"
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_transaction(const char *filename) {
  char name[100], category[50], date[11];
  double amount;

  // Get transaction details
  printf("Enter transaction name: ");
  if (fgets(name, sizeof(name), stdin)) {
    // Remove newline character if it exists
    size_t len = strlen(name);
    if (len > 0 && name[len - 1] == '\n') {
      name[len - 1] = '\0';
    }
  }

  printf("Enter amount (positive for income, negative for expense): ");
  while (scanf("%lf", &amount) != 1) {
    printf("Invalid input. Enter a numeric value: ");
    while (getchar() != '\n')
      ; // Clear input buffer
  }
  while (getchar() != '\n')
    ; // Clear input buffer

  printf("Enter date (YYYY-MM-DD): ");
  if (fgets(date, sizeof(date), stdin)) {
    // Remove newline character if it exists
    size_t len = strlen(date);
    if (len > 0 && date[len - 1] == '\n') {
      date[len - 1] = '\0';
    }
  }
  while (getchar() != '\n')
    ; // Clear input buffer

  printf("Enter category: ");
  if (fgets(category, sizeof(category), stdin)) {
    // Remove newline character if it exists
    size_t len = strlen(category);
    if (len > 0 && category[len - 1] == '\n') {
      category[len - 1] = '\0';
    }
  }

  // Create JSON object
  struct json_object *transaction = json_object_new_object();
  json_object_object_add(transaction, "name", json_object_new_string(name));
  json_object_object_add(transaction, "amount", json_object_new_double(amount));
  json_object_object_add(transaction, "date", json_object_new_string(date));
  json_object_object_add(transaction, "category",
                         json_object_new_string(category));

  // Read existing JSON file
  FILE *file = fopen(filename, "r");
  struct json_object *root = NULL;
  if (file) {
    char *buffer = NULL;
    size_t length;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(length);
    fread(buffer, 1, length, file);
    fclose(file);

    root = json_tokener_parse(buffer);
    free(buffer);
  } else {
    root = json_object_new_array();
  }

  // Append transaction to JSON array
  json_object_array_add(root, transaction);

  // Write back to file
  file = fopen(filename, "w");
  if (file) {
    fprintf(file, "%s",
            json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
    fclose(file);
    printf("\033[1;32mTransaction added successfully!\033[0m\n");
  } else {
    printf("\033[1;31mError writing to file.\033[0m\n");
  }

  json_object_put(root);
}

void list_transactions(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("\033[1;31mNo transactions found.\033[0m\n");
    return;
  }

  char *buffer = NULL;
  size_t length;

  fseek(file, 0, SEEK_END);
  length = ftell(file);
  fseek(file, 0, SEEK_SET);

  buffer = malloc(length);
  fread(buffer, 1, length, file);
  fclose(file);

  struct json_object *root = json_tokener_parse(buffer);
  free(buffer);

  if (!json_object_is_type(root, json_type_array)) {
    printf("\033[1;31mInvalid file format.\033[0m\n");
    json_object_put(root);
    return;
  }

  printf("\033[1;34m\nTransactions:\033[0m\n");
  for (size_t i = 0; i < json_object_array_length(root); i++) {
    struct json_object *transaction = json_object_array_get_idx(root, i);
    const char *name =
        json_object_get_string(json_object_object_get(transaction, "name"));
    double amount =
        json_object_get_double(json_object_object_get(transaction, "amount"));
    const char *date =
        json_object_get_string(json_object_object_get(transaction, "date"));
    const char *category =
        json_object_get_string(json_object_object_get(transaction, "category"));

    printf("%zu. %s | %s | %.2f | %s\n", i + 1, date, name, amount, category);
  }

  json_object_put(root);
}
