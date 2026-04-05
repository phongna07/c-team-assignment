#ifndef GUI_H_
#define GUI_H_

#include <gtk/gtk.h>

#include "logic.h"

extern Course *course_head;
extern ActivityNode *activity_top;
extern GtkListStore *store;
extern GtkWidget *gpa_label;
extern GtkWidget *tree_view;
extern GtkWidget *main_window;

void activate(GtkApplication *app, gpointer user_data);

#endif