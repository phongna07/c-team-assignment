#include "logic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int AddCourse(Course** head, const char* name, int credits, const char* grade) {
  Course* node = (Course*)malloc(sizeof(Course));  // Requirement 3: malloc for dynamic data storage
  Course* current = NULL;

  if (node == NULL) {
    return 0;
  }

  node->name = strdup(name);
  node->grade = strdup(grade);
  node->credits = credits;
  node->next = NULL;

  if (node->name == NULL || node->grade == NULL) {
    free(node->name);
    free(node->grade);
    free(node);
    return 0;
  }

  if (*head == NULL) {
    *head = node;
    return 1;
  }

  current = *head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = node;
  return 1;
}

int EditCourseByIndex(Course* head, int index, const char* name, int credits,
                      const char* grade) {
  Course* current = head;
  int current_index = 1;
  char* new_name = NULL;
  char* new_grade = NULL;

  while (current != NULL && current_index < index) {
    current = current->next;
    ++current_index;
  }

  if (current == NULL) {
    return 0;
  }

  new_name = strdup(name);
  new_grade = strdup(grade);
  if (new_name == NULL || new_grade == NULL) {
    free(new_name);
    free(new_grade);
    return 0;
  }

  free(current->name);
  free(current->grade);
  current->name = new_name;
  current->grade = new_grade;
  current->credits = credits;

  return 1;
}

int DeleteCourseByIndex(Course** head, int index, char** deleted_name) {
  Course* current = *head;
  Course* previous = NULL;
  int current_index = 1;

  while (current != NULL && current_index < index) {
    previous = current;
    current = current->next;
    ++current_index;
  }

  if (current == NULL) {
    return 0;
  }

  if (deleted_name != NULL) {
    *deleted_name = strdup(current->name);
  }

  if (previous == NULL) {
    *head = current->next;
  } else {
    previous->next = current->next;
  }

  free(current->name);
  free(current->grade);
  free(current);
  return 1;
}


static double GradeToPoint(const char* grade) {
  if (strcmp(grade, "A") == 0) return 4.0;
  if (strcmp(grade, "A-") == 0) return 3.7;
  if (strcmp(grade, "B+") == 0) return 3.3;
  if (strcmp(grade, "B") == 0) return 3.0;
  if (strcmp(grade, "B-") == 0) return 2.7;
  if (strcmp(grade, "C+") == 0) return 2.3;
  if (strcmp(grade, "C") == 0) return 2.0;
  if (strcmp(grade, "C-") == 0) return 1.7;
  if (strcmp(grade, "D+") == 0) return 1.3;
  if (strcmp(grade, "D") == 0) return 1.0;
  return 0.0;
}

void PrintCourses(const Course* head) {
  const Course* current = head;
  int index = 1;

  if (current == NULL) {
    printf("No courses found.\n");
    return;
  }

  printf("\nCourses:\n");
  while (current != NULL) {
    printf("%d. Name: %s | Credits: %d | Grade: %s\n", index, current->name,
           current->credits, current->grade);
    current = current->next;
    ++index;
  }
}

double CalculateCumulativeGpa(const Course* head) {
  const Course* current = head;
  double quality_points = 0.0;
  int total_credits = 0;

  while (current != NULL) {
    quality_points += GradeToPoint(current->grade) * (double)current->credits;
    total_credits += current->credits;
    current = current->next;
  }

  if (total_credits == 0) {
    return 0.0;
  }

  return quality_points / (double)total_credits;
}

void DisplayCoursesAndGpa(const Course* head) {
  double gpa = 0.0;

  PrintCourses(head);
  gpa = CalculateCumulativeGpa(head);
  printf("Cumulative GPA: %.2f\n", gpa);
}

void PushActivity(ActivityNode** top, const char* action) {
  ActivityNode* node = (ActivityNode*)malloc(sizeof(ActivityNode));  // Requirement 3: malloc for stack node

  if (node == NULL) {
    return;
  }

  node->action = strdup(action);
  if (node->action == NULL) {
    free(node);
    return;
  }

  node->next = *top;  // Requirement 4: push onto stack via singly linked list pointer
  *top = node;
}

void PrintActivities(const ActivityNode* top) {
  const ActivityNode* current = top;
  int index = 1;

  if (current == NULL) {
    printf("No activities yet.\n");
    return;
  }

  printf("\nRecent Activities:\n");
  while (current != NULL) {
    printf("%d. %s\n", index, current->action);
    current = current->next;
    ++index;
  }
}
