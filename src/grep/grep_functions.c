#define _GNU_SOURCE

#include "s21_grep.h"

int check_flags(flags* option_word, int rez) {
  int check = 0;
  char* file_path;
  switch (rez) {
    case 'e':
      option_word->e = 1;
      if (optarg == NULL) {
        check = 1;
        //                printf("OPTARG = %s ", argv[optind]);
        fprintf(stderr, "grep: option requires an argument -- %c\n", rez);
      } else {
        //        option_word->pattern[option_word->count] = optarg;
        for (int i = 0; i <= (int)strlen(optarg); i++)
          option_word->pattern[option_word->count][i] = optarg[i];
        option_word->count = option_word->count + 1;
        //        printf("[DEBGUG] pattern[%i] = %s\n", option_word->count - 1,
        //               option_word->pattern[option_word->count]);
      }
      break;
    case 'i':
      option_word->i = 1;
      break;
    case 'v':
      option_word->v = 1;
      break;
    case 'c':
      option_word->c = 1;
      break;
    case 'l':
      option_word->l = 1;
      break;
    case 'n':
      option_word->n = 1;
      break;
    case 'h':
      option_word->h = 1;
      break;
    case 's':
      option_word->s = 1;
      break;
    case 'f':
      option_word->f = 1;
      if (optarg == NULL) {
        check = 1;
        fprintf(stderr, "grep: option requires an argument -- %c\n", rez);
      } else {
        file_path = optarg;
        f_add_pattern(option_word, file_path);
        //          printf("[DEBUG] READ FILE FOR NEW PATTERNS: %s\n",
        //          file_path);
      }
      break;
    case 'o':
      option_word->o = 1;
      break;
    case '?': {
      check = 1;
      fprintf(stderr, "cat: illegal option -- %c\n", optopt);
    } break;
  }
  return check;
}

void reader(FILE** file, flags options, char* path_file) {
  //  if (options.f == 1) {
  //    f_add_pattern(&options);
  //  }

  // for (int k = 0; k < options.count; k++) {
  //     printf("[DEBUG] pattern[%i] = %s\n", k, options.pattern[k]);
  // }

  options.v == 1 ? options.o = 0 : 0;
  char* workline = NULL;
  size_t line_t = 0;
  int count_c = 0, count_cv = 0, count_cc = 0;
  type_check check = {0};
  //    printf("[DEBUG] Count patterns: %i\n", options.count);
  while (getline(&workline, &line_t, *file) != -1) {
    check.countline++;
    int check_o = 1;
    check.print = 0;
    check.v = 0;
    check.multifile = options.check_multifile;
    if (options.c == 1 || options.l == 1) {
      cl_work(options, workline, &count_c);
      if (options.v == 1) {
        count_c == options.count ? count_cv = count_cv + 1 : 0;
        count_c = 0;
      } else {
        count_c > 0 && strcmp(workline, "\n") != 0 ? count_cc = count_cc + 1
                                                   : 0;
        count_c = 0;
      }
      continue;
    }
    for (int i = 0; i < options.count; ++i)
      v_work(options, workline, i, &check, path_file, &check_o);

    check.v == options.count
        ? print_line(workline, path_file, &check, options, 0, &check_o)
        : 0;

    if (workline != NULL) {
      free(workline);
      workline = NULL;
    }
  }

  (workline != NULL) ? free(workline) : 0;

  options.v == 0 ? print_cl(options, path_file, count_cc)
                 : print_cl(options, path_file, count_cv);
}

void f_add_pattern(flags* options, char* file_path) {
  char* line_pattern = NULL;
  size_t line_t = 0;
  FILE* file = fopen(file_path, "r");
  if (file != NULL) {
    while (getline(&line_pattern, &line_t, file) != -1) {
      for (int j = 0; j < (int)strlen(line_pattern) - 1; ++j) {
        options->pattern[options->count][j] = line_pattern[j];
      }
      if (line_pattern[strlen(line_pattern) - 1] != '\n')
        options->pattern[options->count][strlen(line_pattern) - 1] =
            line_pattern[strlen(line_pattern) - 1];
      options->count = options->count + 1;

      if (line_pattern != NULL) {
        free(line_pattern);
        line_pattern = NULL;
      }
    }
    line_pattern != NULL ? free(line_pattern) : 0;
    file != NULL ? fclose(file) : 0;
  } else {
    options->s == 0
        ? fprintf(stderr, "grep: %s: No such file or directory\n", file_path)
        : 0;
  }
}

void cl_work(flags options, char* workline, int* count_c) {
  for (int i = 0; i < options.count; ++i)
    if ((reg_compare(workline, options, i) == 0 && options.v == 0) ||
        (reg_compare(workline, options, i) != 0 && options.v == 1))
      *count_c = *count_c + 1;
}

void print_cl(flags options, char* path_file, int count_c) {
  //    printf("[DEBUG] FLAG C = %i, FLAG L = %i, FLAG H = %i, MultiFile =
  //    %i\n", options.c, options.l, options.h, options.check_multifile);
  if (options.c == 1 && options.l == 0) {
    options.h == 0 && options.check_multifile == 1
        ? printf("%s:%i\n", path_file, count_c)
        : printf("%i\n", count_c);
  }
  //        options.check_multifile == 1 ? printf("%i\n", count_c) :
  //        printf("%i\n", count_c);
  options.l == 1 && options.c == 0 && count_c > 0 ? printf("%s\n", path_file)
                                                  : 0;
  if (options.c == 1 && options.l == 1) {
    if (options.h == 0 && options.check_multifile == 1)
      count_c > 0 ? printf("%s:1\n%s\n", path_file, path_file)
                  : printf("%s:0\n", path_file);
    else
      count_c > 0 ? printf("1\n%s\n", path_file) : printf("0\n");
  }
}

void v_work(flags options, char* workline, int i, type_check* check,
            char* path_file, int* check_o) {
  if (options.v == 0) {
    if (reg_compare(workline, options, i) == 0 && check->print == 0)
      print_line(workline, path_file, check, options, i, check_o);
  } else if (options.v == 1) {
    if (reg_compare(workline, options, i) == REG_NOMATCH && check->print == 0)
      check->v = check->v + 1;
  }
}

int print_o(char* workline, flags flag, int i, int* check_o) {
  int status = 1;
  regmatch_t pmatch[0];
  size_t nmatch = 1;
  regex_t reg;

  flag.i == 1 ? regcomp(&reg, flag.pattern[i], REG_EXTENDED | REG_ICASE)
              : regcomp(&reg, flag.pattern[i], REG_EXTENDED);
  status = regexec(&reg, workline, nmatch, pmatch, 0);
  status == 0 ? * check_o = *check_o + 1 : 0;
  while (status == 0 && (int)pmatch[0].rm_eo + 1 < (int)strlen(workline)) {
    for (i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i) {
      putchar(workline[i]);
    }
    putchar('\n');
    workline = workline + pmatch[0].rm_eo + 1;

    //        printf("[DEBUG] WorkLine = %s; strlen(workline) = %li;
    //        strlen(pattern) = %li\n", workline, strlen(workline),
    //        strlen(flag.pattern[i]));
    status = regexec(&reg, workline, nmatch, pmatch, REG_NOTBOL);

    //        printf("[DEBUG] workline = %s\n[DEBUG] strlen(workline) = %i\t
    //        size (pmatch + 1) = %i\n", workline,
    //               (int) strlen(workline), (int) pmatch[0].rm_eo + 1);
    //        getchar();
  }

  if (status == 0) {
    for (i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i) {
      putchar(workline[i]);
    }
    putchar('\n');
  }

  regfree(&reg);
  return status;
}

void print_line(char* workline, char* path_file, type_check* check,
                flags options, int i, int* check_o) {
  options.h == 1 ? check->multifile = 0 : 0;
  if (options.n == 0) {
    if (options.o == 0 || options.v == 1) {
      check->multifile == 1 ? printf("%s:%s", path_file, workline)
                            : printf("%s", workline);
      if (workline[strlen(workline) - 1] != '\n') {
        //                printf("[DEBUG] last symbol = %i",
        //                workline[strlen(workline) - 1]);
        putchar('\n');
      }
      check->print = check->print + 1;
    } else {
      check->multifile == 1 && *check_o == 1 ? printf("%s:", path_file) : 0;
      print_o(workline, options, i, check_o);
    }
  } else {
    if (options.o == 0) {
      check->multifile == 1
          ? printf("%s:%i:%s", path_file, check->countline, workline)
          : printf("%i:%s", check->countline, workline);
      if (workline[strlen(workline) - 1] != '\n') putchar('\n');
      check->print = check->print + 1;
    } else {
      if (*check_o == 1) {
        check->multifile == 1 ? printf("%s:%i:", path_file, check->countline)
                              : printf("%i:", check->countline);
      }
      print_o(workline, options, i, check_o);
    }
  }
  //    check->print = check->print + 1;
}

int reg_compare(char* workline, flags flag, int i) {
  int status = 1;
  //    int cflags = REG_EXTENDED;
  regmatch_t pmatch[1];
  size_t nmatch = 1;
  regex_t reg;

  flag.i == 1 ? regcomp(&reg, flag.pattern[i], REG_EXTENDED | REG_ICASE)
              : regcomp(&reg, flag.pattern[i], REG_EXTENDED);

  status = regexec(&reg, workline, nmatch, pmatch, 0);
  strlen(flag.pattern[i]) == 0 ? status = 0 : 0;

  //    printf("[DEBUG] flag.pattern[%i] = %s. STATUS = %i, strlen = %li\n", i,
  //    flag.pattern[i], status, strlen(flag.pattern[i]));

  regfree(&reg);
  //    printf("[DEBUG] Status reg_compare = %i\n", status);

  return status;
}
