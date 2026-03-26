#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_manager.h"
#include "logic.h"

static void TrimNewline(char* text) {
  size_t len = strlen(text);
  if (len > 0 && text[len - 1] == '\n') {
    text[len - 1] = '\0';
  }
}

static int ReadInt(const char* prompt, int* value) {
  char buffer[64];

  printf("%s", prompt);
  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    return 0;
  }

  if (sscanf(buffer, "%d", value) != 1) {
    return 0;
  }

  return 1;
}

static int ReadText(const char* prompt, char* output, size_t size) {
  printf("%s", prompt);
  if (fgets(output, size, stdin) == NULL) {
    return 0;
  }

  TrimNewline(output);
  return 1;
}

static void HandleAddCourse(Course** head, ActivityNode** activity_top) {
  char name[256];
  char grade[32];
  int credits = 0;
  char activity[320];

  if (!ReadText("Course name: ", name, sizeof(name))) {
    return;
  }
  if (!ReadInt("Credits: ", &credits)) {
    printf("Invalid credits.\n");
    return;
  }
  if (!ReadText("Grade (e.g., A, B+, C): ", grade, sizeof(grade))) {
    return;
  }

  if (AddCourse(head, name, credits, grade)) {
    snprintf(activity, sizeof(activity), "Added %s", name);
    PushActivity(activity_top, activity);
    SaveCoursesToFile("data.txt", *head);
    printf("Course added successfully.\n");
  } else {
    printf("Could not add course.\n");
  }
}

static void HandleEditCourse(Course* head, ActivityNode** activity_top) {
  int index = 0;
  int credits = 0;
  char name[256];
  char grade[32];
  char activity[320];

  if (head == NULL) {
    printf("No courses to edit.\n");
    return;
  }

  PrintCourses(head);
  if (!ReadInt("Enter course index to edit: ", &index)) {
    printf("Invalid index.\n");
    return;
  }
  if (!ReadText("New course name: ", name, sizeof(name))) {
    return;
  }
  if (!ReadInt("New credits: ", &credits)) {
    printf("Invalid credits.\n");
    return;
  }
  if (!ReadText("New grade: ", grade, sizeof(grade))) {
    return;
  }

  if (EditCourseByIndex(head, index, name, credits, grade)) {
    snprintf(activity, sizeof(activity), "Edited %s", name);
    PushActivity(activity_top, activity);
    SaveCoursesToFile("data.txt", head);
    printf("Course edited successfully.\n");
  } else {
    printf("Course index not found.\n");
  }
}

static void HandleDeleteCourse(Course** head, ActivityNode** activity_top) {
  int index = 0;
  char* deleted_name = NULL;
  char activity[320];

  if (*head == NULL) {
    printf("No courses to delete.\n");
    return;
  }

  PrintCourses(*head);
  if (!ReadInt("Enter course index to delete: ", &index)) {
    printf("Invalid index.\n");
    return;
  }

  if (DeleteCourseByIndex(head, index, &deleted_name)) {
    if (deleted_name != NULL) {
      snprintf(activity, sizeof(activity), "Removed %s", deleted_name);
      PushActivity(activity_top, activity);
    } else {
      PushActivity(activity_top, "Removed a course");
    }
    SaveCoursesToFile("data.txt", *head);
    printf("Course removed successfully.\n");
  } else {
    printf("Course index not found.\n");
  }

  free(deleted_name);
}

int main(void) {
  Course* course_head = NULL;
  ActivityNode* activity_top = NULL;
  Course* course_current = NULL;
  Course* course_next = NULL;
  ActivityNode* activity_current = NULL;
  ActivityNode* activity_next = NULL;
  char input[32];

  LoadCoursesFromFile("data.txt", &course_head);

  while (1) {
    printf("\nWelcome to VinUni GPA management system!\n");
    printf("1. Add a new course (Name, Credits, Grade).\n");
    printf("2. Display all courses and the calculated cumulative GPA.\n");
    printf("3. Edit an existing course's details.\n");
    printf("4. Remove a course from the list.\n");
    printf("5. Activity Log: View the recent actions from the Stack.\n");
    printf("Pick an action from the list above (Enter number 1->5 or q to quit): ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }
    TrimNewline(input);

    if (strcmp(input, "q") == 0) {
      break;
    }

    if (strcmp(input, "1") == 0) {
      HandleAddCourse(&course_head, &activity_top);
    } else if (strcmp(input, "2") == 0) {
      DisplayCoursesAndGpa(course_head);
    } else if (strcmp(input, "3") == 0) {
      HandleEditCourse(course_head, &activity_top);
    } else if (strcmp(input, "4") == 0) {
      HandleDeleteCourse(&course_head, &activity_top);
    } else if (strcmp(input, "5") == 0) {
      PrintActivities(activity_top);
    } else {
      printf("Invalid option. Please choose 1-5 or q.\n");
    }
  }

  course_current = course_head;
  while (course_current != NULL) {
    course_next = course_current->next;
    free(course_current->name);
    free(course_current->grade);
    free(course_current);
    course_current = course_next;
  }
  course_head = NULL;

  activity_current = activity_top;
  while (activity_current != NULL) {
    activity_next = activity_current->next;
    free(activity_current->action);
    free(activity_current);
    activity_current = activity_next;
  }
  activity_top = NULL;

  return 0;
}
