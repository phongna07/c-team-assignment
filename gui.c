#include "file_manager.h"
#include "logic.h"
#include <gtk/gtk.h>
#include <stdlib.h>

// Global pointers
Course *course_head = NULL;
ActivityNode *activity_top = NULL;
GtkListStore *store;
GtkWidget *gpa_label;
GtkWidget *tree_view;
GtkWidget *main_window;

// --- CSS Styling (Endeavor Blue & Mahogany Red Theme) ---
void apply_style() {
  GtkCssProvider *provider = gtk_css_provider_new();
  const char *css =
      "window { background-color: #2E548A; }\n" /* Endeavor Blue */

      /* CRITICAL FIX: background-image: none; removes the default GTK white
         gradient */
      "button {"
      "  background-image: none;"
      "  background-color: #C4232A;" /* Mahogany Red */
      "  color: #FFFFFF;"
      "  border-radius: 8px;"
      "  padding: 10px;"
      "  margin: 3px;"
      "  font-weight: bold;"
      "  border: none;"
      "}\n"

      "button:hover {"
      "  background-image: none;"
      "  background-color: #D82B33;" /* Brighter red on hover */
      "}\n"

      /* Target the text explicitly inside the buttons */
      "button label { color: #FFFFFF; }\n"

      /* Global Labels */
      "label { color: #FFFFFF; font-weight: bold; font-size: 14px; }\n"
      "#welcome { font-size: 18px; color: #FFFFFF; margin-bottom: 10px; }\n"

      /* Treeview (Table) */
      "treeview { background-color: #35426E; color: #FFFFFF; border: 2px solid "
      "#C4232A; }\n"

      /* Text inputs */
      "entry {"
      "  background-image: none;"
      "  background-color: #FFFFFF;"
      "  color: #000000;"
      "  padding: 5px;"
      "  margin-bottom: 5px;"
      "  border-radius: 5px;"
      "}\n";

  gtk_css_provider_load_from_data(provider, css, -1);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_USER);
}

// --- Helper: GUI Pop-up Message ---
void show_message(GtkWindow *parent, const char *message, const char *title) {
  GtkWidget *dialog = gtk_window_new();
  gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_title(GTK_WINDOW(dialog), title);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 250, 100);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_start(box, 15);
  gtk_widget_set_margin_end(box, 15);
  gtk_widget_set_margin_top(box, 15);
  gtk_widget_set_margin_bottom(box, 15);
  gtk_window_set_child(GTK_WINDOW(dialog), box);

  GtkWidget *label = gtk_label_new(message);
  gtk_box_append(GTK_BOX(box), label);

  GtkWidget *btn = gtk_button_new_with_label("OK");
  g_signal_connect_swapped(btn, "clicked", G_CALLBACK(gtk_window_destroy),
                           dialog);
  gtk_widget_set_halign(btn, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(box), btn);

  gtk_window_present(GTK_WINDOW(dialog));
}

// --- Refresh UI ---
void refresh_list() {
  gtk_list_store_clear(store);
  Course *curr = course_head;
  GtkTreeIter iter;
  while (curr) {
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, curr->name, 1, curr->credits, 2,
                       curr->grade, -1);
    curr = curr->next;
  }
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "Cumulative GPA: %.2f",
           CalculateCumulativeGpa(course_head));
  gtk_label_set_text(GTK_LABEL(gpa_label), buffer);
}

// --- Dialog Data Structure ---
typedef struct {
  GtkWidget *window;
  GtkWidget *entry_name;
  GtkWidget *entry_credits;
  GtkWidget *entry_grade;
  int edit_index;
} DialogData;

// --- Callback: Save Button Inside the Add/Edit Pop-up ---
void on_dialog_save_clicked(GtkButton *btn, gpointer user_data) {
  DialogData *data = (DialogData *)user_data;

  const char *name = gtk_editable_get_text(GTK_EDITABLE(data->entry_name));
  const char *credits_str =
      gtk_editable_get_text(GTK_EDITABLE(data->entry_credits));
  const char *grade = gtk_editable_get_text(GTK_EDITABLE(data->entry_grade));
  int credits = atoi(credits_str);

  char activity[320];

  if (data->edit_index == 0) {
    if (AddCourse(&course_head, name, credits, grade)) {
      snprintf(activity, sizeof(activity), "Added %s", name);
      PushActivity(&activity_top, activity);
      show_message(GTK_WINDOW(main_window), "Course added successfully!",
                   "Success");
    } else {
      show_message(GTK_WINDOW(main_window), "Could not add course.", "Error");
    }
  } else {
    if (EditCourseByIndex(course_head, data->edit_index, name, credits,
                          grade)) {
      snprintf(activity, sizeof(activity), "Edited %s", name);
      PushActivity(&activity_top, activity);
      show_message(GTK_WINDOW(main_window), "Course edited successfully!",
                   "Success");
    }
  }

  SaveCoursesToFile("data.txt", course_head);
  refresh_list();

  gtk_window_destroy(GTK_WINDOW(data->window));
  free(data);
}

// --- Helper: Show Add/Edit Pop-up Window ---
void show_input_dialog(GtkWindow *parent, int edit_index, const char *def_name,
                       int def_cred, const char *def_grade) {
  DialogData *data = malloc(sizeof(DialogData));
  data->edit_index = edit_index;

  data->window = gtk_window_new();
  gtk_window_set_transient_for(GTK_WINDOW(data->window), parent);
  gtk_window_set_modal(GTK_WINDOW(data->window), TRUE);
  gtk_window_set_title(GTK_WINDOW(data->window),
                       edit_index == 0 ? "Add New Course" : "Edit Course");
  gtk_window_set_default_size(GTK_WINDOW(data->window), 250, 200);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_margin_start(vbox, 10);
  gtk_widget_set_margin_end(vbox, 10);
  gtk_widget_set_margin_top(vbox, 10);
  gtk_window_set_child(GTK_WINDOW(data->window), vbox);

  data->entry_name = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(data->entry_name), "Course Name");
  gtk_box_append(GTK_BOX(vbox), data->entry_name);

  data->entry_credits = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(data->entry_credits),
                                 "Credits (e.g., 3)");
  gtk_box_append(GTK_BOX(vbox), data->entry_credits);

  data->entry_grade = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(data->entry_grade),
                                 "Grade (e.g., A, B+)");
  gtk_box_append(GTK_BOX(vbox), data->entry_grade);

  if (edit_index > 0) {
    char cred_str[16];
    snprintf(cred_str, sizeof(cred_str), "%d", def_cred);
    gtk_editable_set_text(GTK_EDITABLE(data->entry_name), def_name);
    gtk_editable_set_text(GTK_EDITABLE(data->entry_credits), cred_str);
    gtk_editable_set_text(GTK_EDITABLE(data->entry_grade), def_grade);
  }

  GtkWidget *save_btn = gtk_button_new_with_label("Save");
  g_signal_connect(save_btn, "clicked", G_CALLBACK(on_dialog_save_clicked),
                   data);
  gtk_box_append(GTK_BOX(vbox), save_btn);

  gtk_window_present(GTK_WINDOW(data->window));
}

// --- Menu Actions ---
void on_add_clicked(GtkButton *btn, gpointer user_data) {
  show_input_dialog(GTK_WINDOW(main_window), 0, "", 0, "");
}

void on_edit_clicked(GtkButton *btn, gpointer user_data) {
  GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  GtkTreeModel *model;
  GtkTreeIter iter;

  if (gtk_tree_selection_get_selected(sel, &model, &iter)) {
    GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
    int index = gtk_tree_path_get_indices(path)[0] + 1;
    gtk_tree_path_free(path);

    char *name, *grade;
    int credits;
    gtk_tree_model_get(model, &iter, 0, &name, 1, &credits, 2, &grade, -1);

    show_input_dialog(GTK_WINDOW(main_window), index, name, credits, grade);

    g_free(name);
    g_free(grade);
  } else {
    show_message(GTK_WINDOW(main_window),
                 "Please click a course in the list below to edit first.",
                 "Selection Required");
  }
}

void on_delete_clicked(GtkButton *btn, gpointer data) {
  GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  GtkTreeModel *model;
  GtkTreeIter iter;

  if (gtk_tree_selection_get_selected(sel, &model, &iter)) {
    GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
    int index = gtk_tree_path_get_indices(path)[0] + 1;
    gtk_tree_path_free(path);

    char *deleted_name = NULL;
    char activity[320];

    if (DeleteCourseByIndex(&course_head, index, &deleted_name)) {
      if (deleted_name != NULL) {
        snprintf(activity, sizeof(activity), "Removed %s", deleted_name);
        PushActivity(&activity_top, activity);
        free(deleted_name);
      } else {
        PushActivity(&activity_top, "Removed a course");
      }
      SaveCoursesToFile("data.txt", course_head);
      refresh_list();
      show_message(GTK_WINDOW(main_window), "Course removed successfully.",
                   "Success");
    }
  } else {
    show_message(GTK_WINDOW(main_window),
                 "Please click a course in the list below to delete first.",
                 "Selection Required");
  }
}

void on_log_clicked(GtkButton *btn, gpointer user_data) {
  GtkWidget *log_win = gtk_window_new();
  gtk_window_set_transient_for(GTK_WINDOW(log_win), GTK_WINDOW(main_window));
  gtk_window_set_title(GTK_WINDOW(log_win), "Activity Log");
  gtk_window_set_default_size(GTK_WINDOW(log_win), 300, 300);

  GtkWidget *scroll = gtk_scrolled_window_new();
  gtk_window_set_child(GTK_WINDOW(log_win), scroll);

  GString *log_text = g_string_new("");
  ActivityNode *curr = activity_top;
  int i = 1;
  while (curr) {
    g_string_append_printf(log_text, "%d. %s\n", i++, curr->action);
    curr = curr->next;
  }
  if (log_text->len == 0) {
    g_string_assign(log_text, "No activities yet.");
  }

  GtkWidget *label = gtk_label_new(log_text->str);
  gtk_widget_set_margin_start(label, 15);
  gtk_widget_set_margin_top(label, 15);
  gtk_widget_set_halign(label, GTK_ALIGN_START);
  gtk_widget_set_valign(label, GTK_ALIGN_START);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), label);

  g_string_free(log_text, TRUE);
  gtk_window_present(GTK_WINDOW(log_win));
}

// --- Main GUI Setup ---
void activate(GtkApplication *app, gpointer user_data) {
  apply_style();
  LoadCoursesFromFile("data.txt", &course_head);

  main_window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(main_window), "VinUni GPA Manager");
  gtk_window_set_default_size(GTK_WINDOW(main_window), 550, 700);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_start(vbox, 15);
  gtk_widget_set_margin_end(vbox, 15);
  gtk_widget_set_margin_top(vbox, 15);
  gtk_widget_set_margin_bottom(vbox, 15);
  gtk_window_set_child(GTK_WINDOW(main_window), vbox);

  // --- 1. TOP MENU ---
  GtkWidget *welcome =
      gtk_label_new("Welcome to VinUni GPA management system!");
  gtk_widget_set_name(welcome, "welcome");
  gtk_box_append(GTK_BOX(vbox), welcome);

  GtkWidget *menu_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_append(GTK_BOX(vbox), menu_vbox);

  GtkWidget *btn_add =
      gtk_button_new_with_label("1. Add a new course (Name, Credits, Grade).");
  GtkWidget *btn_edit =
      gtk_button_new_with_label("2. Edit an existing course's details.");
  GtkWidget *btn_delete =
      gtk_button_new_with_label("3. Remove a course from the list.");
  GtkWidget *btn_log =
      gtk_button_new_with_label("4. Activity Log: View the recent actions.");

  g_signal_connect(btn_add, "clicked", G_CALLBACK(on_add_clicked), NULL);
  g_signal_connect(btn_edit, "clicked", G_CALLBACK(on_edit_clicked), NULL);
  g_signal_connect(btn_delete, "clicked", G_CALLBACK(on_delete_clicked), NULL);
  g_signal_connect(btn_log, "clicked", G_CALLBACK(on_log_clicked), NULL);

  gtk_box_append(GTK_BOX(menu_vbox), btn_add);
  gtk_box_append(GTK_BOX(menu_vbox), btn_edit);
  gtk_box_append(GTK_BOX(menu_vbox), btn_delete);
  gtk_box_append(GTK_BOX(menu_vbox), btn_log);

  // --- Separator Line ---
  GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_set_margin_top(sep, 10);
  gtk_widget_set_margin_bottom(sep, 10);
  gtk_box_append(GTK_BOX(vbox), sep);

  // --- 2. COURSE DISPLAY TABLE ---
  GtkWidget *list_title =
      gtk_label_new("Select a course below to Edit or Remove:");
  gtk_widget_set_halign(list_title, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(vbox), list_title);

  store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);
  tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  char *columns[] = {"Course Name", "Credits", "Grade"};
  for (int i = 0; i < 3; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(
        columns[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col);
  }

  GtkWidget *scrolled_window = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(scrolled_window, TRUE);
  gtk_widget_set_hexpand(scrolled_window, TRUE);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window),
                                tree_view);
  gtk_box_append(GTK_BOX(vbox), scrolled_window);

  // --- 3. GPA DISPLAY ---
  gpa_label = gtk_label_new("GPA: 0.00");
  gtk_widget_set_margin_top(gpa_label, 10);
  gtk_widget_set_halign(gpa_label, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(vbox), gpa_label);

  refresh_list();
  gtk_window_present(GTK_WINDOW(main_window));
}

int main(int argc, char **argv) {
  GtkApplication *app =
      gtk_application_new("org.vinuni.gpa", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}