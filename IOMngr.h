#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 1024

char current_line[MAXLINE];
FILE *source_fp;
FILE *listing_fp;
int current_char;
int column_number;
int line_number;
int column_of_error;
int listing_mode;
char *error_message;
char *extras[10];
int extras_count;


int openFiles(char *source_name, char *listing_name);

void closeFiles();

char getNextSourceChar();

void writeIndicator(int column);

void writeMessage(char *message);

int getCurrentLineNum();

int getCurrentColumnNum();
