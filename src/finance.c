#include "headers/finance.h"
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Load transactions into memory from the file
void initialize_data(const char *filename, struct json_object **data) {
  FILE *file = fopen(filename, "r");
  *data = NULL;

  if (file) {
    char *buffer = NULL;
    size_t length;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = malloc(length);
    fread(buffer, 1, length, file);
    fclose(file);

    *data = json_tokener_parse(buffer);
    free(buffer);
  }

  if (!(*data) || !json_object_is_type(*data, json_type_object)) {
    printf("\033[1;31mNo data found. Creating new file.\033[0m\n");
    if (*data)
      json_object_put(*data);

    *data = json_object_new_object();
    json_object_object_add(*data, "balance", json_object_new_double(0.0));
    json_object_object_add(*data, "transactions", json_object_new_array());
  }

  // Print the loaded or initialized balance
  struct json_object *balance_obj;
  double balance = 0.0;
  if (json_object_object_get_ex(*data, "balance", &balance_obj)) {
    balance = json_object_get_double(balance_obj);
  }
  printf("\033[1;34mLoaded Balance: %.2f\033[0m\n", balance);
}

// Save transactions back to the file
void save_data(const char *filename, struct json_object *data) {
  FILE *file = fopen(filename, "w");
  if (file) {
    fprintf(file, "%s",
            json_object_to_json_string_ext(data, JSON_C_TO_STRING_PRETTY));
    fclose(file);
    printf("\033[1;32mData saved successfully!\033[0m\n");
  } else {
    printf("\033[1;31mError writing to file.\033[0m\n");
  }
}

// Add a transaction to the data
void add_transaction(struct json_object *data) {
  char name[100], category[50], date[11];
  double amount;

  printf("Enter transaction name: ");
  if (fgets(name, sizeof(name), stdin)) {
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

  printf("Enter date (YYYY-MM-DD or empty for current date): ");
  if (fgets(date, sizeof(date), stdin)) {
    size_t len = strlen(date);
    if (len > 0 && date[len - 1] == '\n') {
      date[len - 1] = '\0';
      len--;
    }
    if (len == 0) {
      time_t now = time(NULL);
      struct tm *timeInfo = localtime(&now);
      strftime(date, sizeof(date), "%Y-%m-%d", timeInfo);
    }
  }

  printf("Enter category: ");
  if (fgets(category, sizeof(category), stdin)) {
    size_t len = strlen(category);
    if (len > 0 && category[len - 1] == '\n') {
      category[len - 1] = '\0';
    }
  }

  struct json_object *transaction = json_object_new_object();
  json_object_object_add(transaction, "name", json_object_new_string(name));
  json_object_object_add(transaction, "amount", json_object_new_double(amount));
  json_object_object_add(transaction, "date", json_object_new_string(date));
  json_object_object_add(transaction, "category",
                         json_object_new_string(category));

  struct json_object *transactions;
  json_object_object_get_ex(data, "transactions", &transactions);
  json_object_array_add(transactions, transaction);

  struct json_object *balance_obj;
  double balance = 0.0;
  if (json_object_object_get_ex(data, "balance", &balance_obj)) {
    balance = json_object_get_double(balance_obj);
  }
  balance += amount;
  json_object_set_double(balance_obj, balance);

  printf("\033[1;32mTransaction added successfully!\033[0m\n");
  printf("\033[1;34mUpdated Balance: %.2f\033[0m\n", balance);
}

// List all transactions
void list_transactions(struct json_object *data) {
  struct json_object *transactions;
  if (json_object_object_get_ex(data, "transactions", &transactions) &&
      json_object_is_type(transactions, json_type_array)) {
    printf("\033[1;34m\nTransactions:\033[0m\n");
    for (size_t i = 0; i < json_object_array_length(transactions); i++) {
      struct json_object *transaction =
          json_object_array_get_idx(transactions, i);
      const char *name =
          json_object_get_string(json_object_object_get(transaction, "name"));
      double amount =
          json_object_get_double(json_object_object_get(transaction, "amount"));
      const char *date =
          json_object_get_string(json_object_object_get(transaction, "date"));
      const char *category = json_object_get_string(
          json_object_object_get(transaction, "category"));

      printf("%zu. %s | %s | %.2f | %s\n", i + 1, date, name, amount, category);
    }
  } else {
    printf("\033[1;31mNo transactions available.\033[0m\n");
  }
}

// Delete a transaction
void delete_transaction(struct json_object *data) {
  list_transactions(data);
  printf("Enter the transaction number to delete: ");
  size_t index;
  if (scanf("%zu", &index) != 1) {
    printf("\033[1;31mInvalid input.\033[0m\n");
    while (getchar() != '\n')
      ; // Clear input buffer
    return;
  }
  while (getchar() != '\n')
    ; // Clear input buffer

  struct json_object *transactions;
  if (json_object_object_get_ex(data, "transactions", &transactions) &&
      json_object_is_type(transactions, json_type_array)) {
    if (index > 0 && index <= json_object_array_length(transactions)) {
      struct json_object *transaction =
          json_object_array_get_idx(transactions, index - 1);
      double amount =
          json_object_get_double(json_object_object_get(transaction, "amount"));

      // Adjust balance
      struct json_object *balance_obj;
      if (json_object_object_get_ex(data, "balance", &balance_obj)) {
        double balance = json_object_get_double(balance_obj);
        balance -= amount;
        json_object_set_double(balance_obj, balance);
        printf("\033[1;34mUpdated Balance: %.2f\033[0m\n", balance);
      }

      json_object_array_del_idx(transactions, index - 1, 1);
      printf("\033[1;32mTransaction deleted successfully!\033[0m\n");
    } else {
      printf("\033[1;31mInvalid transaction number.\033[0m\n");
    }
  } else {
    printf("\033[1;31mNo transactions available to delete.\033[0m\n");
  }
}
