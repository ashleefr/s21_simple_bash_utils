#include "s21_grep.h"

const char *short_options = "e:ivclnhsf:o";

const struct option long_options[] = {{"e", required_argument, NULL, 'e'},
                                      {"f", required_argument, NULL, 'f'},
                                      {NULL, 0, NULL, 0}};

int main(int argc, char *argv[]) {
  flags option_word = {0};
  char *path_file;
  FILE *file;
  int error = 0, rez, option_index;
  opterr = 0;
  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    error = check_flags(&option_word, rez);
  }
  path_file = "-";

  if (option_word.count == 0 && optind != argc) {
    //          printf("[DEBUG] optind = %i", optind);
    for (int i = 0; i < (int)strlen(argv[optind]); i++) {
      option_word.pattern[0][i] = argv[optind][i];
    }
    option_word.count++;
  }

  //  option_word.count == 0 ? option_word.pattern[option_word.count] =
  //  argv[optind] : 0; option_word.count == 0 ? option_word.count++ : 0;
  int arg_now = optind;
  option_word.e == 0 &&option_word.f == 0 ? arg_now = arg_now + 1 : 0;
  (arg_now < argc - 1) ? option_word.check_multifile = 1 : 0;
  //  if (options.f == 1) {
  //      f_add_pattern(&options);
  //  }
  while (arg_now < argc) {
    path_file = argv[arg_now];
    file = fopen(path_file, "r");
    file == NULL &&option_word.s == 0
        ? fprintf(stderr, "grep: %s: No such file or directory\n", path_file)
        : 0;
    if (error == 0 && file != NULL) {
      reader(&file, option_word, path_file);
      file != NULL ? fclose(file) : 0;
    }
    arg_now++;
    //        file != NULL ? fclose(file) : 0;
  }

  if (error > 0) {
    fprintf(stderr, "usage: cat [-eivclnhsfo] [file ...]");
  }

  //  for (int k = 0; k < option_word.count; k++) {
  //      free(option_word.pattern[k]);
  //  }

  return 0;
}
