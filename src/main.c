#include "headers/finance.h"
#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>

void show_main_menu() {
  printf("\033[1;34m\n==> Finance Tracker <==\033[0m\n");
  printf("1. Add a transaction\n");
  printf("2. List transactions\n");
  printf("3. Delete a transaction\n");
  printf("4. Exit\n");
}

int main() {
  ensure_cache_dir(); // Ensure the cache directory exists
  // char *json_file_path = get_cache_file_path(); // Get the full JSON file
  // path
  char *json_file_path = get_cache_file_path();

  struct json_object *data = NULL;
  initialize_data(json_file_path, &data);

  while (1) {
    show_main_menu();
    printf("Choose an option: ");

    int choice = get_choice();

    switch (choice) {
    case 1:
      add_transaction(data);
      save_data(json_file_path, data);
      break;
    case 2:
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

  json_object_put(data);
  free(json_file_path);
  return 0;
}
