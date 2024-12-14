#include "headers/finance.h"
#include "headers/utils.h"
#include <stdio.h>

#define JSON_FILE "resources/finances.json"

void show_main_menu() {
  printf("\033[1;34m\n==> Finance Tracker <==\033[0m\n");
  printf("1. Add a transaction\n");
  printf("2. List transactions\n");
  printf("3. Exit\n");
}

int main() {
  init_json_file(JSON_FILE);
  while (1) {
    show_main_menu();
    printf("Choose an option: ");

    int choice = get_choice();

    switch (choice) {
    case 1:
      add_transaction(JSON_FILE);
      break;
    case 2:
      list_transactions(JSON_FILE);
      break;
    case 3:
      printf("\033[1;31mExiting...\033[0m\n");
      return 0;
    default:
      printf("\033[1;31mInvalid choice. Try again.\033[0m\n\n");
    }
  }
}
