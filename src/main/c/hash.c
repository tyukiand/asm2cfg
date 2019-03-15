#include "hash.h"

#define MIN_BUCKETS 16
#define MAX_BUCKETS (INT_MAX/2)
#define MAX_LOAD_FACTOR 0.8
#define MIN_LOAD_FACTOR 0.2

#define foreach_hash_cons(i, list) \
  for (hash_cons i = list; i; i = i->next)

struct hash_cons {
  void* key;
  void* value;
  int cached_hash;
  struct hash_cons * next; 
};

typedef struct hash_cons * hash_cons;

static hash_cons hash_cons_new(
  int const cachedKeyHash, 
  void * const key, 
  void * const value
) {
  hash_cons res = (hash_cons) malloc(sizeof(struct hash_cons));
  *res = (struct hash_cons) {
    .key = key,
    .value = value,
    .cached_hash = cachedKeyHash,
    .next = NULL
  };
  return res;
}

/**
 * Frees the hash_cons struct that is pointed at, and 
 * all it's successors.
 *
 * If you want to delete a single hash_cons node without deleting all it's
 * successors, you have to set the `hc->next` to NULL explicitly.
 */
static void hash_cons_free(hash_cons hc) {
  if (hc) {
    hash_cons nextToFree = hc->next;
    free(hc);
    hash_cons_free(nextToFree);
  }
}

/*
 * Inserts key value pair into a linked list 
 * of `hash_cons`es, returns the size change
 * of the list.
 */
static int hash_cons_insert(
  hash_cons * headPtr, 
  int keyHash, 
  void* key, 
  void* value,
  key_equals eq
) {
  /*
   * The `hash_cons` is passed as a pointer,
   * so that we can deal with the NULL (empty list)
   * case uniformly.
   */
  if (*headPtr == NULL) {
    *headPtr = hash_cons_new(keyHash, key, value);
    return +1;
  } else {
    if (
      (*headPtr)->cached_hash == keyHash && 
      eq((*headPtr)->key, key)
    ) {
      (*headPtr)->value = value;
      return 0;
    } else {
      return hash_cons_insert(&((*headPtr)->next), keyHash, key, value, eq);
    }
  }
}


static void * hash_cons_get_flag(
  hash_cons head, 
  int keyHash, 
  void* key, 
  key_equals eq, 
  bool *containsKeyFlag
) {
  if(head == NULL) {
    *containsKeyFlag = false;
    return NULL;
  } else {
    if (head->cached_hash == keyHash && eq(head->key, key)) {
      *containsKeyFlag = true;
      return head->value;
    } else {
      return hash_cons_get_flag(head->next, keyHash, key, eq, containsKeyFlag);
    }
  }
}

/**
 * Seeks and deletes the key-value pair associated with 
 * the specified key.
 *
 * @return 0 if key not found, -1 if the key has been found and deleted
 */
static int hash_cons_remove(
  hash_cons * hcp, 
  int keyHash, 
  void* key, 
  key_equals eq
) {
  if (*hcp == NULL) {
    return 0;
  } else {
    hash_cons hc = *hcp;
    if (hc->cached_hash == keyHash && eq(key, hc->key)) {
      *hcp = hc->next;
      free(hc); // free a single node, do _not_ free all successors!
      return -1;
    } else {
      return hash_cons_remove(&(hc->next), keyHash, key, eq);
    }
  }
}

static int mod(int hash, int size) {
  int res = hash % size;
  if (res < 0) {
    return res + size;
  } else {
    return res;
  }
}

/**
 * Allocates a new hashmap structure with the specified 
 * initial number of buckets.
 *
 * @param h hash function
 * @param eq the equality for keys
 * @param num_buckets initial number of buckets (must be at positive)
 * @return pointer to freshly allocated and initialized hashmap structure.
 */
hash hash_new_buckets(key_hashcode h, key_equals eq, int num_buckets) {
  assert(num_buckets > 0);
  hash res = (hash) malloc(sizeof(struct hash));
  *res = (struct hash){
    .h = h,
    .eq = eq,
    .num_buckets = num_buckets,
    .buckets = (hash_cons *) malloc(sizeof(hash_cons) * num_buckets),
    .size = 0
  };
  memset(res->buckets, 0, sizeof(hash_cons) * num_buckets);
  return res;
}

/**
 * Allocates a new hashmap structure with a default 
 * initial number of buckets.
 *
 * @param h hash function
 * @param eq the equality for keys
 * @return pointer to freshly allocated and initialized hashmap structure.
 */
hash hash_new(key_hashcode h, key_equals eq) {
  return hash_new_buckets(h, eq, MIN_BUCKETS);
}

void hash_free(hash h) {
  for (int b = 0; b < h->num_buckets; b++) {
    hash_cons_free(h->buckets[b]);
  }
  free(h->buckets);
  free(h);
}

/**
 * Ensures that the load factor is within bounds,
 * creates new bucket array, if necessary.
 */
static void hash_rehash(hash hm) {
  float loadFactor = hm->size / (float) hm->num_buckets;
  int newNumBuckets = -1;
  if (loadFactor > MAX_LOAD_FACTOR && hm->num_buckets < MAX_BUCKETS) {
    newNumBuckets = hm->num_buckets * 2;
  } else if (loadFactor < MIN_LOAD_FACTOR && hm->num_buckets > MIN_BUCKETS) {
    newNumBuckets = hm->num_buckets / 2;
  }

  // newNumBuckets is set to sentinel value -1 if no change is required
  if (newNumBuckets > 0) {
    hash_cons * newBuckets = 
      (hash_cons *) malloc(sizeof(hash_cons) * newNumBuckets);
    memset(newBuckets, 0, sizeof(hash_cons) * newNumBuckets);
    for (int b = 0; b < hm->num_buckets; b++) {
      foreach_hash_cons(i, hm->buckets[b]) {
        int bIdx = mod(i->cached_hash, newNumBuckets);
        hash_cons_insert(
          &(newBuckets[bIdx]),
          i->cached_hash,
          i->key,
          i->value,
          hm->eq
        );
      }
      hash_cons_free(hm->buckets[b]);
    }
    free(hm->buckets);
    hm->buckets = newBuckets;
    hm->num_buckets = newNumBuckets;
  }
}

/** 
 * Inserts a key-value pair into the hashmap.
 * If the key is already present, overrides the old value.
 *
 * @param hm the hashmap
 * @param key key
 * @param value value
 * @return void
 */
void hash_put(hash hm, void *key, void *value) {
  int keyHash = hm->h(key);
  int bucketIdx = mod(keyHash, hm->num_buckets);
  int dSize = hash_cons_insert(
    &(hm->buckets[bucketIdx]), 
    keyHash, 
    key, 
    value, 
    hm->eq
  );
  if (dSize > 0) {
    hm->size += dSize;
    hash_rehash(hm);
  }
}

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
void * hash_get_flag(hash hm, void *key, bool * containsKeyFlag) {
  int keyHash = hm->h(key);
  int bIdx = mod(keyHash, hm->num_buckets);
  return hash_cons_get_flag(
    hm->buckets[bIdx], 
    keyHash, 
    key, 
    hm->eq, 
    containsKeyFlag
  );
}

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
void * hash_get(hash const hm, void * const key) {
  bool flag = false;
  return hash_get_flag(hm, key, &flag);
}

/**
 * Checks whether the hashmap contains a key.
 *
 * @param hm the hash map
 * @param key the key
 * @return `true` iff the key is contained in the map.
 */
bool hash_contains(hash const hm, void * const key) {
  bool res = false;
  hash_get_flag(hm, key, &res);
  return res;
}

/**
 * @return `true` if the entry has actually been removed, `false` otherwise.
 */
bool hash_remove(hash const hm, void *key) {
  int code = hm->h(key);
  int bkt = mod(code, hm->num_buckets);
  int dSize = hash_cons_remove(&(hm->buckets[bkt]), code, key, hm->eq);
  hm->size += dSize;
  hash_rehash(hm);
  return (dSize == 0) ? false : true;
}

/**
 * Draws an ascii-histogram that shows how many entries are in each bin.
 * Could be useful to get a feeling for whether the hash function
 * produces too many collisions.
 * 
 * @param hm hash map
 * @return void
 */
void hash_print_ascii_histogram(hash hm) {
  printf("Size = %d, #buckets = %d\n", hm->size, hm->num_buckets);
  for (int i = 0; i < hm->num_buckets; i++) {
    foreach_hash_cons(hc, hm->buckets[i]) {
      printf("|");
    }
    printf("\n");
  }
  printf("----");
}

/**
 * A simple hash function that works for any
 * integer-like values which are cast into a void pointer.
 *
 * Does not dereference the pointer, so the pointer can
 * hold small integers and characters.
 */
int voidptr_hashcode(void *i) {
  long j = (long) i;
  return ((4999 * j) << 8) + ((31 * j) << 4) + (19 * j);
}

/**
 * Tests two void pointers for equality.
 *
 * Does not dereference the pointers, so the pointer can
 * hold small integers and characters.
 */
bool voidptr_equals(void *a, void * b) {
  return a == b;
}

int string_hashcode(void *strvptr) {
  char *str = strvptr;
  int hash = 5381;

  while (*str) {
    hash = ((hash << 5) + hash) + *(str++);
  }

  return hash;
}

bool string_equals(void *a, void *b) {
  char* stra = a;
  char* strb = b;
  return (strcmp(stra, strb) == 0);
}

/*
int main(int argc, char** args) {
  hash h = hash_new(&voidptr_hashcode, &voidptr_equals);
  char * foo = "foo";
  char * bar = "bar";
  char * baz = "baz";
  hash_put(h, (void *)42, (void *)foo);
  hash_put(h, (void *)57, (void *)bar);
  printf("h[42] = %s\n", (char *) hash_get(h, (void *)42));
  printf("h[57] = %s\n", (char *) hash_get(h, (void *)57));
  hash_put(h, (void *)42, (void *)baz);
  printf("h[42] = %s\n", (char *) hash_get(h, (void *)42));

  hash_print_ascii_histogram(h);
  char * nix = "nix";
  const int TESTMAX = 1000;
  for (int i = 0; i <= TESTMAX; i++) {
    long ii = i;
    hash_put(h, (void *)ii, (void *)nix);
    if (i % 100 == 0) {
      hash_print_ascii_histogram(h);
      foreach_hash(k, v, h) {
        printf("%5d -> %s\n", (int)(long)k, (char*)v);
      }
    }
  }

  for (int i = 0; i <= TESTMAX; i++) {
    long ii = i;
    printf("removing %d\n", i);
    hash_remove(h, (void *)ii);
    if (i % 100 == 0) {
      hash_print_ascii_histogram(h);
      foreach_hash(k, v, h) {
        printf("%5d -> %s\n", (int)(long)k, (char*)v);
      }
    }
  }

  hash_free(h);
}*/