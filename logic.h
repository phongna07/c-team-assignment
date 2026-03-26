#ifndef LOGIC_H_
#define LOGIC_H_

#include <stddef.h>

typedef struct Course {
  char* name;
  int credits;
  char* grade;
  struct Course* next;
} Course;

typedef struct ActivityNode {
  char* action;
  struct ActivityNode* next;
} ActivityNode;

int AddCourse(Course** head, const char* name, int credits, const char* grade);
int EditCourseByIndex(Course* head, int index, const char* name, int credits,
                      const char* grade);
int DeleteCourseByIndex(Course** head, int index, char** deleted_name);

void DisplayCoursesAndGpa(const Course* head);
void PrintCourses(const Course* head);
double CalculateCumulativeGpa(const Course* head);

void PushActivity(ActivityNode** top, const char* action);
void PrintActivities(const ActivityNode* top);

#endif  // LOGIC_H_
