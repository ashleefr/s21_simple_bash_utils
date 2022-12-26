#ifndef SRC_CAT_HEADER_H_
#define SRC_CAT_HEADER_H_

//// LIBRARIES AND DEFINITIONS ////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHRMAX 10000

//// STRUCTURES ////

typedef struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int check;
} flag;

//// FUNCTIONS ////

int parser(char command_in[CHRMAX]);
int reader(char* file_name, flag flags);
void print_e(flag flags, int countline);
void print_v(flag flags, char arg1);
void find_flags(flag* flags, char command[CHRMAX]);
void erase_flags(char line[CHRMAX]);
void move_arr(char line[CHRMAX], int i);

#endif  //  SRC_CAT_HEADER_H_
