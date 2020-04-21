#include "IOMngr.h"
// do getNextSourceChar with a listing file
// int main (int argc, char * argv[]) {
//   // char c;
//   // if (argc > 2) {
//   //   listing_mode = 1;
//   //   openFiles(argv[1], argv[2]);
//   // } else {
//   //   listing_mode = 0;
//   //   openFiles(argv[1], 0);
//   // }
//   //
//   // while ((c = getNextSourceChar()) != EOF) {
//   //   if (c == 'f') {
//   //     writeIndicator(column_number);
//   //     writeMessage("This character should not be here");
//   //   } else {
//   //   }
//   // }
//   return 0;
// }

/*
 * Open the source file whose name is given in sourceName
 * If listingName is not NULL, open the listing file whose name is given in listingName
 */

int openFiles(char *source_name, char *listing_name) {
    if (listing_name != NULL) {
      listing_mode = 1;
    } else {
      listing_mode = 0;
    }
  error_message = NULL;
  line_number = 1;
  current_line[0] = 0;
  current_char = 0;
  column_of_error = -1;
  extras_count = 0;
  if (listing_mode == 0) {
    source_fp = fopen(source_name, "r");
    if (source_fp == NULL) {
      printf("Error opening up %s.\n", source_name);
      return 0;
    }
  } else {
    source_fp = fopen(source_name, "r");
    listing_fp = fopen(listing_name, "w");
    if (source_fp == NULL) {
      printf("Error opening up %s.\n", source_name);
      return 0;
    } else if (listing_fp == NULL) {
      printf("Error opening up %s.\n", listing_name);
      return 0;
    }
  }
  return 1;
}

void closeFiles() {
  fclose(source_fp);
  if (listing_mode == 1) {
    fclose(listing_fp);
  }
}

char getNextSourceChar() {
  if (strlen(current_line) == 0) {
    if (fgets(current_line, MAXLINE, source_fp) == NULL) {
      printf("Error.");
      return EOF;
    }
  } else if (current_char > strlen(current_line) - 1) {
    if (listing_mode == 0) {
      int i = 0;
      if (fgets(current_line, MAXLINE, source_fp) == NULL) {
        printf("End of the file\n");
        return EOF;
      }
      line_number++;
      column_number++;
      current_char = 0;
      // return '\n';
    } else {
      int i = 0;

      if (fgets(current_line, 50, source_fp) == NULL) {
        return EOF;
      }
      line_number++;
      column_number++;
      current_char = 0;
      return '\n';
    }
  }
  if (current_char == 0 && listing_mode == 1) {
    fprintf(listing_fp, "%d. %s", line_number, current_line);
  }
  column_number = current_char;
  current_char++;
  // printf("char: %d : %c\n", current_line[current_char-1], current_line[current_char-1]);
  return current_line[current_char - 1];
}

void writeIndicator(int column) {
  // printf("\nAdding a write indicator!\n");
  char *str;
  str = malloc(column + 5);
  int i = 0;
  int j = 0;
  free(extras[extras_count]);
  for (i = 0; i < column + 3; ++i) {
    str[i] = ' ';
    j = i;
  }
  j++;
  str[j] = '^';
  j++;
  str[j+1] = '\0';
  extras[extras_count] = malloc(sizeof(str) + 2);
  for (i = 0; i < strlen(str) + 2; ++i) {
    extras[extras_count] = NULL;
  }
  extras[extras_count] = str;

  extras_count++;
  if (listing_fp != NULL) {
    fprintf(listing_fp, "    ");
    for (i = 0; i < column_number; ++i) {
      fprintf(listing_fp, " ");
    }
    fprintf(listing_fp, "^\n");
  } else {
    printf("    ");
    for (i = 0; i < column_number; ++i) {
      printf(" ");
    }
    printf("^\n");
  }
  // printf("%s", extras[extras_count-1]);
}

void writeMessage(char *message) {
  int i = 0;
  // extras[extras_count] = malloc(sizeof(message));
  extras[extras_count] = message;
  extras_count++;
  if (listing_fp != NULL) {
    fprintf(listing_fp, "%s\n", message);
  } else {
    printf("%s\n", message);
  }
  // printf("%s", extras[extras_count-1]);

}

int getCurrentLineNum() {

  return line_number;
}

int getCurrentColumnNum() {

  return column_number;
}
