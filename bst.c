#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*bst_compare_t)(void *val1, void *val2);

struct bst_elem {
  struct bst_elem *left;
  struct bst_elem *right;
  void *value;
};

struct bst {
  bst_compare_t compare;
  struct bst_elem *tree;
};

int string_compare(void *val1, void *val2);
struct bst_elem *bst_alloc_elem(void);
void bst_free_elem(struct bst_elem *elem);
void bst_init(struct bst *handle, bst_compare_t compare);
void bst_insert(struct bst *handle, void *value);
struct bst_elem *bst_insert_elem(struct bst_elem *const elem,
  bst_compare_t compare, void *value);
void bst_delete(struct bst *handle, void *value);
struct bst_elem *bst_delete_elem(struct bst_elem *const elem,
  bst_compare_t compare, void *value);
struct bst_elem *bst_sift_down_right(struct bst_elem *root, 
  struct bst_elem *right);
void bst_for_each(struct bst *handle, void (*func)(void*));
void bst_for_each_elem(struct bst_elem *elem, void (*func)(void*));
void bst_print_string(void *value);
void bst_destroy(struct bst *handle);
void bst_destroy_elem(struct bst_elem *elem);
int main(void);

int string_compare(void *val1, void *val2) {
  return strcmp((const char*) val1, (const char*) val2);
}

struct bst_elem *bst_alloc_elem(void) {
  struct bst_elem *elem = malloc(sizeof(struct bst_elem));
  if (elem == NULL) {
    perror("bst_alloc_elem");
    exit(EXIT_FAILURE);
  }
  return elem;
}

void bst_free_elem(struct bst_elem *elem) {
  free(elem);
}

void bst_init(struct bst *handle, bst_compare_t compare) {
  handle->compare = compare;
  handle->tree = NULL;
}

void bst_insert(struct bst *handle, void *value) {
  handle->tree = bst_insert_elem(handle->tree, handle->compare, value);
}

struct bst_elem *bst_insert_elem(struct bst_elem *const elem,
  bst_compare_t compare, void *value) {
  if (elem == NULL) {
    struct bst_elem *new_elem = bst_alloc_elem();
    new_elem->left = new_elem->right = NULL;
    new_elem->value = value;
    return new_elem;
  } else {
    const int comparison = compare(value, elem->value);
    if (comparison < 0) {
      elem->left = bst_insert_elem(elem->left, compare, value);
    } else if (comparison > 0) {
      elem->right = bst_insert_elem(elem->right, compare, value);
    }
    return elem;
  }
}

void bst_delete(struct bst *handle, void *value) {
  handle->tree = bst_delete_elem(handle->tree, handle->compare, value);
}

struct bst_elem *bst_delete_elem(struct bst_elem *const elem,
  bst_compare_t compare, void *value) {
  if (elem != NULL) {
    printf("Currently checking: %s\n", (char *)elem->value);
    const int comparison = compare(value, elem->value);
    if (comparison == 0) {
      printf("Found match!\n");
      if (elem->left == NULL && elem->right == NULL) {
        printf("Removing this node.\n");
        bst_free_elem(elem);
        return NULL;
      } else if (elem->left == NULL) {
        printf("Replacing right child.\n");
        struct bst_elem *right = elem->right;
        memcpy(elem, right, sizeof(struct bst_elem));
        bst_free_elem(right);
      } else if (elem->right == NULL) {
        printf("Replacing left child.\n");
        struct bst_elem *left = elem->left;
        memcpy(elem, left, sizeof(struct bst_elem));
        bst_free_elem(left);
      } else {
        printf("Sifting down and replacing.\n");
        struct bst_elem *left = elem->left;
        struct bst_elem *right = elem->right;
        memcpy(elem, left, sizeof(struct bst_elem));
        bst_sift_down_right(left, right);
        bst_free_elem(left);
      }
    } else if (comparison < 0) {
      printf("Trying left child...\n");
      elem->left = bst_delete_elem(elem->left, compare, value);
    } else if (comparison > 0) {
      printf("Trying right child...\n");
      elem->right = bst_delete_elem(elem->right, compare, value);
    }
  } else {
    printf("Couldn't find a match.");
  }
  return elem;
}

struct bst_elem *bst_sift_down_right(struct bst_elem *root, 
  struct bst_elem *right) {
  if (root->right == NULL) {
    root->right = right;
  } else {
    root->right = bst_sift_down_right(root->right, right);
  }
  return root;
}

void bst_for_each(struct bst *handle, void (*func)(void*)) {
  bst_for_each_elem(handle->tree, func);
}

void bst_for_each_elem(struct bst_elem *elem, void (*func)(void*)) {
  if (elem == NULL) {
    return;
  }
  bst_for_each_elem(elem->left, func);
  func(elem->value);
  bst_for_each_elem(elem->right, func);
}

void bst_print_string(void *value) {
  const char *str = (const char*) value;
  printf("%s\n", str);
}

void bst_destroy(struct bst *handle) {
  bst_destroy_elem(handle->tree);
}

void bst_destroy_elem(struct bst_elem *elem) {
  if (elem == NULL) {
    return;
  }
  bst_destroy_elem(elem->left);
  bst_destroy_elem(elem->right);
  bst_free_elem(elem);
}

int main(void) {
  char s1[] = "George";
  char s2[] = "Bob";
  char s3[] = "Charlie";
  char s4[] = "Ingrid";
  char s5[] = "David";
  char s6[] = "Eve";
  char s7[] = "Fiona";
  char s8[] = "Alice";
  char s9[] = "Hugh";
  struct bst tree;
  bst_init(&tree, &string_compare);
  bst_insert(&tree, s1);
  bst_insert(&tree, s2);
  bst_insert(&tree, s3);
  bst_insert(&tree, s4);
  bst_insert(&tree, s5);
  bst_insert(&tree, s6);
  bst_insert(&tree, s7);
  bst_insert(&tree, s8);
  bst_insert(&tree, s9);
  bst_delete(&tree, s1);
  bst_delete(&tree, s4);
  bst_delete(&tree, s8);
  bst_for_each(&tree, &bst_print_string);
  bst_destroy(&tree);
  return 0;
}
