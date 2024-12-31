#include "headers/finance.h"
#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>

void show_main_menu() {
  printf("\033[1;34m\n==> Finance Tracker <==\033[0m\n");
  printf("1. Add a transaction\n");
  printf("2. List transactions\n");
  printf("3. Exit\n");
}

int main() {
  ensure_cache_dir(); // Ensure the cache directory exists
  char *json_file_path = get_cache_file_path(); // Get the full JSON file path

  initialize_balance(json_file_path);

  while (1) {
    show_main_menu();
    printf("Choose an option: ");

    int choice = get_choice();

    switch (choice) {
    case 1:
      add_transaction(json_file_path);
      break;
    case 2:
      list_transactions(json_file_path);
      break;
    case 3:
      printf("\033[1;31mExiting...\033[0m\n");
      free(json_file_path);
      return 0;
    default:
      printf("\033[1;31mInvalid choice. Try again.\033[0m\n\n");
    }
  }

  free(json_file_path);
  return 0;
}
