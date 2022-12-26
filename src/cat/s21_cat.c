#include "header.h"

int main(int argc, char* argv[]) {
  char line[CHRMAX] = {0};

  for (int i = 1; i < argc; i++) {
    strcat(line, " ");
    strcat(line, argv[i]);
  }

  strcat(line, " \n");
  parser(line);
  return 0;
}
