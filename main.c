#include "gui.h"

Course *course_head = NULL;
ActivityNode *activity_top = NULL;
GtkListStore *store = NULL;
GtkWidget *gpa_label = NULL;
GtkWidget *tree_view = NULL;
GtkWidget *main_window = NULL;

int main(int argc, char **argv) {
  GtkApplication *app =
      gtk_application_new("org.vinuni.gpa", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
