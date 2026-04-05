#define _POSIX_C_SOURCE 200809L

#include "file_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LoadCoursesFromFile(const char *filename, Course **head) {
  FILE *file = fopen(filename, "r"); // Requirement 5: File I/O read
  char line[512];

  if (file == NULL) {
    return 0;
  }

  while (fgets(line, sizeof(line), file) != NULL) {
    char *course_name = NULL;
    char *credits_text = NULL;
    char *grade = NULL;
    char *save_ptr = NULL;
    int credits = 0;
    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

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

int SaveCoursesToFile(const char *filename, const Course *head) {
  FILE *file = fopen(filename, "w"); // Requirement 5: File I/O write/update
  const Course *current = head;

  if (file == NULL) {
    return 0;
  }

  while (current != NULL) {
    fprintf(file, "%s|%d|%s\n", current->name, current->credits,
            current->grade);
    current = current->next;
  }

  fclose(file);
  return 1;
}
