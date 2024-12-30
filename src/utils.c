#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int get_choice() {
  int choice;
  while (scanf("%d", &choice) != 1) {
    printf("Invalid input. Please enter a number: ");
    while (getchar() != '\n')
      ; // Clear input buffer
  }
  getchar(); // Remove newline character left in buffer after input
  return choice;
}
