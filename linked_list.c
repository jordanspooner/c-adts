#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct list {
  struct list_elem *header;
  struct list_elem *footer;
};

struct list_elem {
  int value;
  struct list_elem *prev;
  struct list_elem *next;
};

typedef struct list_elem* list_iter;

struct list_elem *list_alloc_elem(void) {
  struct list_elem *elem = malloc(sizeof(struct list_elem));
  if (elem == NULL) {
    perror("list_alloc_elem");
    exit(EXIT_FAILURE);
  }
  return elem;
}

struct list_elem *list_alloc_elem(void);
void list_free_elem(struct list_elem *elem);
void list_init(struct list *l);
list_iter list_begin(struct list *l);
list_iter list_end(struct list *l);
void list_insert(struct list *l, list_iter iter, int value);
list_iter list_iter_next(list_iter iter);
int list_iter_value(list_iter iter);
int list_is_internal(list_iter iter);
void list_insert_front(struct list *l, int value);
void list_insert_back(struct list *l, int value);
void list_destroy(struct list *l);
int main(void);

void list_free_elem(struct list_elem *elem) {
  free(elem);
}

void list_init(struct list *l) {
  l->header = list_alloc_elem();
  l->footer = list_alloc_elem();
  l->header->prev = NULL;
  l->footer->next = NULL;
  l->header->next = l->footer;
  l->footer->prev = l->header;
}

list_iter list_begin(struct list *l) {
  return l->header->next;
}

list_iter list_end(struct list *l) {
  return l->footer;
}

void list_insert(struct list *l, list_iter iter, int value) {
  struct list_elem *new_elem = list_alloc_elem();
  new_elem->value = value;
  new_elem->prev = iter->prev;
  new_elem->next = iter;
  iter->prev->next = new_elem;
  iter->prev = new_elem;
}

list_iter list_iter_next(list_iter iter) {
  return iter->next;
}

int list_iter_value(list_iter iter) {
  assert(list_is_internal(iter));
  return iter->value;
}

int list_is_internal(list_iter iter) {
  return iter->prev != NULL && iter->next != NULL;
}

void list_insert_front(struct list *l, int value) {
  list_insert(l, list_begin(l), value);
}

void list_insert_back(struct list *l, int value) {
  list_insert(l, list_end(l), value);
}

void list_destroy(struct list *l) {
  struct list_elem *elem = l->header;
  while (elem != NULL) {
    struct list_elem *next = elem->next;
    list_free_elem(elem);
    elem = next;
  }
}

int main(void) {
  struct list l;
  list_init(&l);

  list_insert_front(&l, 1);
  list_insert_front(&l, 2);
  list_insert_back(&l, 1);
  list_insert_back(&l, 2);

  for(list_iter iter = list_begin(&l);
      iter != list_end(&l);
      iter = list_iter_next(iter)) {
    printf("%i\n", list_iter_value(iter));
  }
  list_destroy(&l);
  return 0;
}
