#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

//// LIBRARIES ////

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP 1000

//// STRUCTURES ////

typedef struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char pattern[MAXOP][MAXOP];
  int count_files;
  int count;
  int check_multifile;
} flags;

typedef struct check {
  int multifile;
  int print;
  int v;
  int countline;
} type_check;
//// FUNCTIONS ////

void f_add_pattern(flags* options, char* file_path);
void cl_work(flags options, char* workline, int* count_c);
void print_cl(flags options, char* path_file, int count_c);
void v_work(flags options, char* workline, int i, type_check* check,
            char* path_file, int* check_o);
int print_o(char* workline, flags flag, int i, int* check_o);
void print_line(char* workline, char* path_file, type_check* check,
                flags options, int i, int* check_o);
int check_flags(flags* option_word, int res);
void reader(FILE** file, flags options, char* path_file);
int reg_compare(char* pattern, flags flag, int i);

#endif  //  SRC_GREP_S21_GREP_H_
