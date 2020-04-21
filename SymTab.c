#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymTab.h"

// int main() {
//   // printf("Starting!\n");
//   // SymTab *table;
//   // table = createSymTab(3);
//   // enterName(table, "a");
//   // enterName(table, "b");
//   // enterName(table, "c");
//   // enterName(table, "d");
//   // enterName(table, "d");
//   // enterName(table, "d");
//   // enterName(table, "d");
//   // enterName(table, "d");
//   // enterName(table, "d");
//   // enterName(table, "e");
//   // enterName(table, "f");
//   // enterName(table, "purple");
//   //
//   // startIterator(table);
//   // setCurrentAttr(table, "Aloha, my first attribute!");
//   //
//   // printf("The attribute of current is: %s\n", getCurrentAttr(table));
//   // printf("The name of current is: %s\n", getCurrentName(table));
//   //
//   // //working on iterator portion
//   // startIterator(table);
//   // printf("%s\n", table->current->name);
//   // while (nextEntry(table)) {
//   //   printf("%s\n", table->current->name);
//   // }
//   //
//   // destroySymTab(table);
//   return 0;
// }

SymTab *createSymTab(int size) {
  SymTab *table = malloc(sizeof(SymTab));
  table->size = size;
  table->contents = (SymEntry**)malloc(sizeof(SymEntry) * size);
  table->current = NULL;
  for (int i = 0; i < size; ++i) {
    table->contents[i] = NULL;
  }
  return table;
}

int getHashValue(SymTab *table, char *name) {
  int index;
  int charSum = 0;
  for (int i = 0; i < strlen(name); ++i) {
    charSum += (int)name[i];
  }
  return (charSum % table->size);
}

int enterName(SymTab *table, char *name) {

  int index;
  int charSum = 0;
  for (int i = 0; i < strlen(name); ++i) {
    charSum += name[i];
  }
  index = getHashValue(table, name);

  SymEntry* cur;
  cur = table->contents[index];
  while (cur != NULL) {
    if (!strcmp(cur->name, name)) {
      table->current = cur;
      return 0;
    }
    cur = cur->next;
  }

  // printf("- %s is going in index %d\n", name, index);
  SymEntry *newSymEntry = malloc(sizeof(SymEntry));
  SymEntry *temp;
  newSymEntry->name = strdup(name);
  newSymEntry->attribute = NULL;
  newSymEntry->next = NULL;
  table->current = newSymEntry;


  if (table->contents[index] == NULL) {
    // table->contents[0] = malloc(sizeof(SymEntry));
    table->contents[index] = newSymEntry;
    return 1;
  } else {
    temp = table->contents[index];
    // Find the last node in the list
    while (temp->next != NULL) {
      temp = temp->next;
    }
    // Add the new SymEntry to the end of the list!
    temp->next = newSymEntry;

    return 1;
  }
}

void setCurrentAttr(SymTab *table, void *attr) {
  if (hasCurrent(table) == 1) {
    table->current->attribute = attr;
  }
}

void * getCurrentAttr(SymTab *table) {
  return table->current->attribute;
}

char * getCurrentName(SymTab *table) {
  return table->current->name;
}

int findName(SymTab *table, char *name) {
  int index = getHashValue(table, name);
  SymEntry *current = table->contents[index];
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      table->current = current;
      return 1;
    }
    current = current->next;

  }
  return 0;
}

int hasCurrent(SymTab *table) {
  if (table->current != NULL) {
    return 1;
  }
  return 0;
}

void destroySymTab(SymTab *table) {
  for (int i = 0; i < table->size; ++i) {
    SymEntry *current = table->contents[i];
    SymEntry *next;
    while (current != NULL) {
      next = current->next;
      free(current);
      current = next;
    }
  }
  free(table);
}

int startIterator(SymTab *table) {
  int i;
  for (i = 0; i < table->size; ++i) {
    if (table->contents[i] != NULL) {
      table->current = table->contents[i];
      return 1;
    }
  }
  return 0;
}

int nextEntry(SymTab *table) {
  int i;
  if (table->current->next == NULL) {
    int currentIndex = getHashValue(table, table->current->name);
    for (i = (currentIndex + 1); i < table->size; ++i) {
      if (table->contents[i] != NULL) {
        table->current = table->contents[i];
        return 1;
      }
    }
    return 0;
  } else {
    table->current = table->current->next;
    return 1;
  }
}
