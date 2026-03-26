#include "file_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void TrimNewline(char* text) {
  size_t len = strlen(text);

  if (len == 0) {
    return;
  }

  if (text[len - 1] == '\n') {
    text[len - 1] = '\0';
  }
}

int LoadCoursesFromFile(const char* filename, Course** head) {
  FILE* file = fopen(filename, "r");
  char line[512];

  if (file == NULL) {
    return 0;
  }

  while (fgets(line, sizeof(line), file) != NULL) {
    char* course_name = NULL;
    char* credits_text = NULL;
    char* grade = NULL;
    char* save_ptr = NULL;
    int credits = 0;

    TrimNewline(line);

    course_name = strtok_r(line, "|", &save_ptr);
    credits_text = strtok_r(NULL, "|", &save_ptr);
    grade = strtok_r(NULL, "|", &save_ptr);

    if (course_name == NULL || credits_text == NULL || grade == NULL) {
      continue;
    }

    credits = atoi(credits_text);
    AddCourse(head, course_name, credits, grade);
  }

  fclose(file);
  return 1;
}

int SaveCoursesToFile(const char* filename, const Course* head) {
  FILE* file = fopen(filename, "w");
  const Course* current = head;

  if (file == NULL) {
    return 0;
  }

  while (current != NULL) {
    fprintf(file, "%s|%d|%s\n", current->name, current->credits, current->grade);
    current = current->next;
  }

  fclose(file);
  return 1;
}
