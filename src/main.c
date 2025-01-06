#include "headers/finance.h"
#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_main_menu() {
  printf("\033[1;34m\n==> Finance Tracker <==\033[0m\n");
  printf("1. Add a transaction\n");
  printf("2. List transactions\n");
  printf("3. Delete a transaction\n");
  printf("4. Exit\n");
}

void handle_arguments(int argc, char *argv[], struct json_object *data,
                      char *json_file_path) {
  if (argc < 2) {
    printf("\033[1;31mError: No arguments provided. Use the menu "
           "instead.\033[0m\n");
    return;
  }

  if (strcmp(argv[1], "list") == 0 || strcmp(argv[1], "ls") == 0) {
    list_transactions(data);
  } else if (strcmp(argv[1], "add") == 0 || strcmp(argv[1], "a") == 0) {
    add_transaction(data);
    save_data(json_file_path, data);
  } else if (strcmp(argv[1], "delete") == 0 || strcmp(argv[1], "rm") == 0) {
    delete_transaction(data);
    save_data(json_file_path, data);
  } else if (strcmp(argv[1], "balance") == 0 || strcmp(argv[1], "b") == 0) {
    print_balance(data);
  } else {
    printf("\033[1;31mUnknown command: %s\033[0m\n", argv[1]);
    printf("Available commands: add|a, list|ls, delete|rm, balance|b\n");
  }
}

int main(int argc, char *argv[]) {
  ensure_cache_dir(); // Ensure the cache directory exists
  char *json_file_path = get_cache_file_path();

  struct json_object *data = NULL;
  initialize_data(json_file_path, &data);

  // Handle arguments if provided
  if (argc > 1) {
    handle_arguments(argc, argv, data, json_file_path);
  } else {
    // Fallback to menu if no arguments provided
    while (1) {
      print_balance(data);
      show_main_menu();
      printf("Choose an option: ");

      int choice = get_choice();

      switch (choice) {
      case 1:
        add_transaction(data);
        save_data(json_file_path, data);
        break;
      case 2:
        printf("\033[1;34m\nTransactions:\033[0m\n");
        list_transactions(data);
        break;
      case 3:
        delete_transaction(data);
        save_data(json_file_path, data);
        break;
      case 4:
        printf("\033[1;31mExiting...\033[0m\n");
        json_object_put(data);
        free(json_file_path);
        return 0;
      default:
        printf("\033[1;31mInvalid choice. Try again.\033[0m\n\n");
      }
    }
  }

  json_object_put(data);
  free(json_file_path);
  return 0;
}
