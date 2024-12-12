#include "headers/finance.h"
#include "headers/utils.h"
#include <stdio.h>

void show_main_menu() {
  printf("\033[1;34m\nFinance Tracker\033[0m\n");
  printf("1. Add a transaction\n");
  printf("2. List transactions\n");
  printf("3. Exit\n");
  printf("Choose an option: ");
}

int main() { show_main_menu(); }
