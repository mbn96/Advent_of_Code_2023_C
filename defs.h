#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef struct {
  char *data;
  size_t len;
} file_content;

typedef file_content str_view;

typedef struct {
  file_content fc;
  char *pos;
  char delim;
} Spliter;

typedef bool (*predicate)(char a);

#define SPLITTER(name, strv, dlm) Spliter name = {.fc = strv, .pos = strv.data, .delim = dlm}
#define LINER(name, fc) SPLITTER(name, fc, '\n')

bool read_file(const char *path, file_content *fc);

bool next_token(Spliter *liner, str_view *line);

int str_find_char(const str_view *sv, predicate pred);
int str_find_last_char(const str_view *sv, predicate pred);
