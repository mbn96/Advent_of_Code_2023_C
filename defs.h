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

#define SPLITTER(name, strv, dlm)                                              \
  Spliter name = {.fc = strv, .pos = strv.data, .delim = dlm}
#define LINER(name, fc) SPLITTER(name, fc, '\n')

bool read_file(const char *path, file_content *fc);

bool next_token(Spliter *liner, str_view *line);

int str_find_char(const str_view *sv, predicate pred);
int str_find_last_char(const str_view *sv, predicate pred);

// HASH-MAP
typedef size_t (*hash_func)(const void *key);
typedef bool (*cmp_func)(const void *a, const void *b);

// some hash functions
size_t hash_str(const void *key);
size_t hash_int(const void *key);

typedef struct hash_map_node hash_map_node;

typedef struct {
  hash_func hf;
  cmp_func cf;
  size_t size;
  size_t count;
  hash_map_node **nodes; /*A singly linked list*/
} hash_map;

hash_map *hash_map_new(hash_func hf, cmp_func cf);
void hash_map_free(hash_map *hm);

void *hash_map_get(hash_map *hm, void *key);
void hash_map_put(hash_map *hm, void *key, void *value);

bool hash_map_has(hash_map *hm, void *key);
bool hash_map_remove(hash_map *hm, void *key);
