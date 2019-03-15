#ifndef HASH_MAP_H
#define HASH_MAP_H

#include<stdbool.h>
#include<stdio.h> 
#include<stdlib.h>
#include<limits.h>
#include<assert.h>
#include<string.h>

/**
 * `foreach`-loop for a hashmap data structure.
 * 
 * Performs an operation for each key-value pair `(k, v)`
 * that is contained in `hashmap`.
 */
#define foreach_hash(k, v, hashmap) \
  for (int _bkt = 0; _bkt < hashmap->num_buckets; _bkt++)    \
    for (                                                    \
      void * _hshc = (void *) hashmap->buckets[_bkt], *k, *v;\
      _hshc                                                  \
      && (k = ((hash_cons) _hshc)->key, true)                \
      && (v = ((hash_cons) _hshc)->value, true);             \
      _hshc = ((hash_cons) _hshc)->next                      \
    )                                                        \

typedef int (*key_hashcode)(void *);
typedef bool (*key_equals)(void *, void *);

struct hash_cons;
typedef struct hash_cons * hash_cons;

struct hash {
  key_hashcode h;
  key_equals eq;
  unsigned int num_buckets;
  unsigned int size;
  hash_cons * buckets;
};

typedef struct hash * hash;

/**
 * Allocates a new hashmap structure with the specified 
 * initial number of buckets.
 *
 * @param h hash function
 * @param eq the equality for keys
 * @param num_buckets initial number of buckets (must be at positive)
 * @return pointer to freshly allocated and initialized hashmap structure.
 */
hash hash_new_buckets(key_hashcode h, key_equals eq, int num_buckets);

/**
 * Allocates a new hashmap structure with a default 
 * initial number of buckets.
 *
 * @param h hash function
 * @param eq the equality for keys
 * @return pointer to freshly allocated and initialized hashmap structure.
 */
hash hash_new(key_hashcode h, key_equals eq);

void hash_free(hash h);

/** 
 * Inserts a key-value pair into the hashmap.
 * If the key is already present, overrides the old value.
 *
 * @param hm the hashmap
 * @param key key
 * @param value value
 * @return void
 */
void hash_put(hash hm, void *key, void *value);

/**
 * Looks up a key in a hashmap. Sets the `containsKeyFlag` to `true` if
 * the key is actually present in the map. Sets the value pointed at by
 * `containsKeyFlag` to false if the key is not contained in the map.
 * In this way, it is possible to tell apart whether the key is 
 * not contained in the map, or deliberately mapped to NULL.
 *
 * @param hm hashmap
 * @param key key
 * @param containsKeyFlag pointer to a bool that is set to `true` if the key is
 *   contained in the map.
 * @return either the value or NULL if the key is not contained in the map.
 */
void * hash_get_flag(hash hm, void *key, bool * containsKeyFlag);

/**
 * Performs a lookup in the hashmap.
 *
 * This version cannot differentiate between "value is not present" and 
 * "value == NULL".
 * If you need this differentiation, use `hash_get_flag` instead.
 *
 * @param hm the hashmap
 * @key the key
 * @return either the value, or the NULL pointer, if the key is not in the 
 * hashmap.
 */
void * hash_get(hash const hm, void * const key);

/**
 * Checks whether the hashmap contains a key.
 *
 * @param hm the hash map
 * @param key the key
 * @return `true` iff the key is contained in the map.
 */
bool hash_contains(hash const hm, void * const key);

/**
 * @return `true` if the entry has actually been removed, `false` otherwise.
 */
bool hash_remove(hash const hm, void *key);

/**
 * Draws an ascii-histogram that shows how many entries are in each bin.
 * Could be useful to get a feeling for whether the hash function
 * produces too many collisions.
 * 
 * @param hm hash map
 * @return void
 */
void hash_print_ascii_histogram(hash hm);

/**
 * A simple hash function that works for any
 * integer-like values which are cast into a void pointer.
 *
 * Does not dereference the pointer, so the pointer can
 * hold small integers and characters.
 */
int voidptr_hashcode(void *i);

/**
 * Tests two void pointers for equality.
 *
 * Does not dereference the pointers, so the pointer can
 * hold small integers and characters.
 */
bool voidptr_equals(void *a, void * b);

int string_hashcode(void *strvptr);
bool string_equals(void *a, void *b);

#endif
