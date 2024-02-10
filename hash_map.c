#include "defs.h"
#include <assert.h>

/*
 * A simple hash map implementation with chaining for collision resolution and
 * size control based on load factor.
 */

/*
 * We could use a pool of hash_map_node structs to reduce malloc calls.
 * But for now I'm just using malloc for bucket nodes.
 * TODO: Add a pool
 */

typedef struct hash_map_node {
  void          *key;
  void          *value;
  hash_map_node *next;
} hash_map_node;

#define HASH_MAP_NODE_SIZE               sizeof(hash_map_node)
#define NEW_HASH_MAP_NODE()              (hash_map_node *)calloc(1, HASH_MAP_NODE_SIZE)

#define SIZE_COUNT_THRESHOLD             0.75
#define BUCKET_COLLISION_COUNT_THRESHOLD 8
#define SIZE_MULTIPLIER                  2
#define HASH_MAP_SIZE_INIT               32

hash_map *hash_map_new(hash_func hf, cmp_func cf) {
  hash_map *hm = (hash_map *)calloc(1, sizeof(hash_map));
  hm->hf       = hf;
  hm->cf       = cf;
  hm->nodes    = (hash_map_node **)calloc(HASH_MAP_SIZE_INIT, sizeof(hash_map_node *));
  hm->size     = HASH_MAP_SIZE_INIT;
  assert(!(hm->size & (hm->size - 1))); // must be power of 2
  return hm;
}
void hash_map_free(hash_map *hm) {
  for (size_t i = 0; i < hm->size; i++) {
    hash_map_node *node = hm->nodes[i];
    while (node != NULL) {
      hash_map_node *next = node->next;
      free(node);
      node = next;
    }
  }
  free(hm->nodes);
  free(hm);
}

static void hash_map_resize(hash_map *hm) {
  size_t new_size = hm->size * SIZE_MULTIPLIER;
  assert(!(new_size & (new_size - 1))); // must be power of 2
  hash_map_node **new_nodes = (hash_map_node **)calloc(new_size, sizeof(hash_map_node *));
  for (size_t i = 0; i < hm->size; i++) {
    hash_map_node *node = hm->nodes[i];
    while (node != NULL) {
      hash_map_node *next    = node->next;
      size_t         new_idx = hm->hf(node->key) & (new_size - 1);
      node->next             = new_nodes[new_idx];
      new_nodes[new_idx]     = node;
      node                   = next;
    }
  }
  free(hm->nodes);
  hm->nodes = new_nodes;
  hm->size  = new_size;
}

static hash_map_node *hash_map_find(const hash_map *hm, const void *key) {
  size_t         idx  = hm->hf(key) & (hm->size - 1);
  hash_map_node *node = hm->nodes[idx];
  while (node != NULL) {
    if (hm->cf(node->key, key)) {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

void *hash_map_get(const hash_map *hm, const void *key, bool *hasValue) {
  hash_map_node *node = hash_map_find(hm, key);
  *hasValue           = node != NULL; // So we could store a NULL value
  return node == NULL ? NULL : node->value;
}
void hash_map_put(hash_map *hm, void *key, void *value) {
  hash_map_node *node = hash_map_find(hm, key);
  if (node == NULL) {
    size_t idx     = hm->hf(key) & (hm->size - 1);
    node           = NEW_HASH_MAP_NODE();
    node->key      = key;
    node->next     = hm->nodes[idx];
    hm->nodes[idx] = node;
    hm->count++;
    if ((f32)hm->count / (f32)hm->size > SIZE_COUNT_THRESHOLD) {
      hash_map_resize(hm);
    }
  }
  node->value = value;
}

bool hash_map_has(const hash_map *hm, const void *key) { return hash_map_find(hm, key) != NULL; }
bool hash_map_remove(hash_map *hm, void *key) {
  size_t         idx  = hm->hf(key) & (hm->size - 1);
  hash_map_node *node = hm->nodes[idx];
  hash_map_node *prev = NULL;
  while (node != NULL) {
    if (hm->cf(node->key, key)) {
      if (prev == NULL) {
        hm->nodes[idx] = node->next;
      } else {
        prev->next = node->next;
      }
      free(node);
      hm->count--;
      return true;
    }
    prev = node;
    node = node->next;
  }
  return false;
}

size_t hash_str(const void *key) {
  // https://en.wikipedia.org/wiki/Jenkins_hash_function
  size_t val = 0;
  char  *str = (char *)key;
  while (*str != '\0') {
    val += *str;
    val += (val << 10);
    val ^= (val >> 6);
    str++;
  }
  val += (val << 3);
  val ^= (val >> 11);
  return val;
}

size_t hash_int(const void *key) {
  // simple bit-mixing hash
  // https://en.wikipedia.org/wiki/MurmurHash
  u64 val = (u64)key;
  val ^= val >> 33;
  val *= 0xff51afd7ed558ccd;
  val ^= val >> 33;
  val *= 0xc4ceb9fe1a85ec53;
  val ^= val >> 33;
  return val;
}
