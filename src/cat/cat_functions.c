#include "header.h"

int parser(char command_in[CHRMAX]) {
  flag our_flags = {0};
  while (command_in[0] == ' ') {
    move_arr(command_in, 1);
  }

  while (command_in[0] == '-') {
    find_flags(&our_flags, command_in);
  }
  if (our_flags.check != -1) {
    //        printf("[DEBUG] AFTER CAT AND FLAGS: %s", command_in);
    if (command_in != NULL) {
      int i = 0;
      while (command_in[i] != '\n') {
        char file_name[CHRMAX] = {'\0'};
        while ((command_in[i] != ' ') && (command_in[i] != '\0')) {
          //                        printf("[DEBUG] COMMAND_IN[%i] = %c\n", i,
          //                        command_in[i]); getchar();

          file_name[i] = command_in[i];
          i++;
        }
        if (command_in[i] == ' ') i++;
        move_arr(command_in, i);

        //                    printf("\n[DEBUG] AFTER FILE PARSING: %s",
        //                    command_in); printf("[DEBUG] FILE NAME: %s",
        //                    file_name); getchar();

        reader(file_name, our_flags);
        if (our_flags.b == 1 || our_flags.n == 1)
            putchar('\n');
        i = 0;
      }
    }
  }
  return 0;
}

int reader(char* file_name, flag flags) {
  FILE* file = fopen(file_name, "r");
  if (file != NULL) {
    char arg1 = getc(file);
    int countline = 1, newline_counter = 0;
    if ((((flags.b == 1) || (flags.n == 1)) && (arg1 != '\n')) ||
        ((flags.n == 1) && (flags.b == 0) && (arg1 == '\n'))) {
      printf("%6d\t", countline);
      countline++;
    }
    while (arg1 != EOF) {
      int check = 0, check_b = 0, check_t = 0;
      if ((flags.t == 1) && (arg1 == '\t')) {
        printf("^I");
        check_t = 1;
      }
      if (arg1 == '\n') {
        newline_counter++;
        if ((flags.s == 1) && (newline_counter > 2)) {
          newline_counter--;
          arg1 = getc(file);
          if ((arg1 != '\n') && (flags.b == 1)) {
            printf("%6d\t", countline);
            countline++;
          }
          continue;
        } else {
          if ((flags.b == 0) && (flags.n == 1)) {
            print_e(flags, countline);
            countline++;
            check = 1;
          }
          if (flags.b == 1) {
            arg1 = getc(file);
            check_b = 1;
            if (arg1 != '\n' && arg1 != EOF) {
              print_e(flags, countline);
              countline++;
              check = 1;
            } else {
              if (flags.e == 1) printf("$");
            }
          }
        }
        if (flags.b == 0 && flags.n == 0) {
          print_e(flags, countline);
          check = 1;
        }
      } else {
        newline_counter = 0;
      }
      if ((check == 0 || (arg1 != '\n' && flags.b == 0)) && (check_t == 0))
        print_v(flags, arg1);
      if (check_b != 1) arg1 = getc(file);
    }
    fclose(file);
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", file_name);
  }
  return 0;
}

void print_e(flag flags, int countline) {
  if (flags.b == 0 && flags.n == 0) {
    if (flags.e == 1) {
      printf("$\n");
    } else {
      printf("\n");
    }
  } else {
    if (flags.e == 1)
      printf("$\n%6d\t", countline);
    else
      printf("\n%6d\t", countline);
  }
}

void print_v(flag flags, char arg1) {
  if (flags.v == 1) {
    int a = (int)(arg1);
    int alt = 64;
    if ((a > -1 && a < 9) || (a > 10 && a < 32)) printf("^%c", (char)(alt + a));
    if (a == 127) printf("^?");
    if (a >= 32 && a != 127) printf("%c", arg1);
    if (a == 10) printf("%c", arg1);
    if (a == 9) printf("%c", (char)(9));
  } else {
    if (arg1 != EOF && arg1 != '\0')
        putchar(arg1);
  }
}

void find_flags(flag* flags, char command[CHRMAX]) {
  int i = 0;
  if (command[i] == '-' && command[i + 1] != '-') {
    i++;
    while (command[i] != ' ' && command[i] != '\n') {
      switch (command[i]) {
        case 'b':
          flags->b = 1;
          break;
        case 'e':
          flags->e = 1;
          flags->v = 1;
          break;
        case 'n':
          flags->n = 1;
          break;
        case 's':
          flags->s = 1;
          break;
        case 't':
          flags->t = 1;
          flags->v = 1;
          break;
        case 'v':
          flags->v = 1;
          break;
        case 'T':
          flags->t = 1;
          break;
        case 'E':
          flags->e = 1;
          break;
        default:
          fprintf(
              stderr,
              "s21_cat: illegal flag -- %c\nusage: cat [-benstuv] [file ...]",
              command[i]);
          flags->check = -1;
          break;
      }
      if (flags->check == -1) break;
      i++;
    }
  } else if (command[i] == '-' && command[i + 1] == '-') {
    char temp_flag[20] = {0};
    int j = 0;
    while (command[i] != ' ') {
      temp_flag[j] = command[i];
      i++;
      j++;
    }
    //        printf("\n%s\n", temp_flag);
    if (strcmp(temp_flag, "--number") == 0) flags->n = 1;
    if (strcmp(temp_flag, "--number-nonblank") == 0) flags->b = 1;
    if (strcmp(temp_flag, "--squeeze-blank") == 0) flags->s = 1;
  } else {
    flags->check = -1;
    fprintf(stderr,
            "s21_cat: illegal flag -- %c\nusage: cat [-benstuv] [file ...]",
            command[i]);
  }
  erase_flags(command);
}

void erase_flags(char line[CHRMAX]) {
  int i = 0;
  while (line[i] != ' ') i++;
  i++;
  move_arr(line, i);
}

void move_arr(char line[CHRMAX], int i) {
  for (int j = 0; j < CHRMAX - i; j++) {
    line[j] = line[j + i];
  }
}
