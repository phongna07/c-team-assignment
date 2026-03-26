#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include "logic.h"

int LoadCoursesFromFile(const char* filename, Course** head);
int SaveCoursesToFile(const char* filename, const Course* head);

#endif  // FILE_MANAGER_H_
